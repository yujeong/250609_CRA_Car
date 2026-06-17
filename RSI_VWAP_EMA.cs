#region Using declarations
using System;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Windows.Media;
using NinjaTrader.Data;
using NinjaTrader.NinjaScript;
using NinjaTrader.NinjaScript.Indicators;
using NinjaTrader.NinjaScript.DrawingTools;
#endregion

/*
 * [전략 개요]
 *
 * ▶ 15분봉 (방향 판단)
 *   - 20EMA + VWAP 위 → 상승장 바이어스
 *   - 20EMA + VWAP 아래 → 하락장 바이어스
 *
 * ▶ 3분봉 (진입)
 *   - 20EMA, VWAP, RSI(14)
 *
 * ▶ 롱 진입 조건
 *   1) 15분봉: 종가 > 20EMA AND 종가 > VWAP
 *   2) 3분봉: 전봉 RSI가 LongRsiMin~LongRsiMax 구간 (기본 40~50, 눌림)
 *   3) 3분봉: 현재 RSI > 전봉 RSI (상승 전환)
 *
 * ▶ 숏 진입 조건
 *   1) 15분봉: 종가 < 20EMA AND 종가 < VWAP
 *   2) 3분봉: 전봉 RSI가 ShortRsiMin~ShortRsiMax 구간 (기본 50~60, 반등)
 *   3) 3분봉: 현재 RSI < 전봉 RSI (하락 전환)
 *
 * ▶ 설치 경로
 *   Documents\NinjaTrader 8\bin\Custom\Strategies\RSI_VWAP_EMA.cs
 */

namespace NinjaTrader.NinjaScript.Strategies
{
    public class RSI_VWAP_EMA : Strategy
    {
        // ── 인디케이터 ──────────────────────────────────────────────
        private EMA  ema3;      // 3분봉 20EMA
        private EMA  ema15;     // 15분봉 20EMA
        private VWAP vwap3;     // 3분봉 VWAP
        private VWAP vwap15;    // 15분봉 VWAP
        private RSI  rsi3;      // 3분봉 RSI(14)

        protected override void OnStateChange()
        {
            if (State == State.SetDefaults)
            {
                Description                  = "15min bias (20EMA+VWAP) + 3min RSI pullback/bounce entry";
                Name                         = "RSI_VWAP_EMA";
                Calculate                    = Calculate.OnBarClose;
                EntriesPerDirection          = 1;
                EntryHandling                = EntryHandling.AllEntries;
                IsExitOnSessionCloseStrategy = true;
                ExitOnSessionCloseSeconds    = 30;

                // 파라미터 기본값
                EmaPeriod         = 20;
                RsiPeriod         = 14;
                RsiSmoothing      = 3;
                LongRsiMin        = 40;
                LongRsiMax        = 50;
                ShortRsiMin       = 50;
                ShortRsiMax       = 60;
                UseStopAndTarget  = true;
                StopLossTicks     = 20;
                ProfitTargetTicks = 40;
            }
            else if (State == State.Configure)
            {
                // 15분봉 데이터 추가 (인덱스 1)
                AddDataSeries(BarsPeriodType.Minute, 15);

                if (UseStopAndTarget)
                {
                    SetStopLoss(CalculationMode.Ticks, StopLossTicks);
                    SetProfitTarget(CalculationMode.Ticks, ProfitTargetTicks);
                }
            }
            else if (State == State.DataLoaded)
            {
                ema3   = EMA(Closes[0], EmaPeriod);
                ema15  = EMA(Closes[1], EmaPeriod);
                vwap3  = VWAP(Closes[0]);
                vwap15 = VWAP(Closes[1]);
                rsi3   = RSI(Closes[0], RsiPeriod, RsiSmoothing);
            }
        }

        protected override void OnBarUpdate()
        {
            // 3분봉(기본 시리즈)만 처리
            if (BarsInProgress != 0) return;

            // 인디케이터 초기화 대기
            if (CurrentBars[0] < EmaPeriod + 5 || CurrentBars[1] < EmaPeriod + 5) return;

            // ── 15분봉: 장 방향 판단 ─────────────────────────────
            double close15   = Closes[1][0];
            bool bullishBias = close15 > ema15[0] && close15 > vwap15[0];  // 상승장
            bool bearishBias = close15 < ema15[0] && close15 < vwap15[0];  // 하락장

            // ── 3분봉: RSI 상태 ──────────────────────────────────
            double currRsi = rsi3[0];
            double prevRsi = rsi3[1];

            // 롱 조건: 전봉 RSI가 눌림 구간(40~50)에 있었고, 현재 RSI가 상승 전환
            bool prevInLongZone = prevRsi >= LongRsiMin && prevRsi <= LongRsiMax;
            bool rsiTurnedUp    = currRsi > prevRsi;

            // 숏 조건: 전봉 RSI가 반등 구간(50~60)에 있었고, 현재 RSI가 하락 전환
            bool prevInShortZone = prevRsi >= ShortRsiMin && prevRsi <= ShortRsiMax;
            bool rsiTurnedDown   = currRsi < prevRsi;

            // ── 진입 ─────────────────────────────────────────────
            if (Position.MarketPosition == MarketPosition.Flat)
            {
                // 롱 진입
                if (bullishBias && prevInLongZone && rsiTurnedUp)
                {
                    EnterLong(1, "Long");
                    Draw.ArrowUp(this, "L_" + CurrentBar, false, 0,
                        Low[0] - TickSize * 3, Brushes.LimeGreen);
                }
                // 숏 진입
                else if (bearishBias && prevInShortZone && rsiTurnedDown)
                {
                    EnterShort(1, "Short");
                    Draw.ArrowDown(this, "S_" + CurrentBar, false, 0,
                        High[0] + TickSize * 3, Brushes.Red);
                }
            }
        }

        #region Properties

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "EMA Period", GroupName = "Indicators", Order = 1)]
        public int EmaPeriod { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "RSI Period", GroupName = "Indicators", Order = 2)]
        public int RsiPeriod { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "RSI Smoothing", GroupName = "Indicators", Order = 3)]
        public int RsiSmoothing { get; set; }

        [NinjaScriptProperty]
        [Range(0.0, 100.0)]
        [Display(Name = "Long RSI Min (눌림 하단)", GroupName = "Long Setup", Order = 4)]
        public double LongRsiMin { get; set; }

        [NinjaScriptProperty]
        [Range(0.0, 100.0)]
        [Display(Name = "Long RSI Max (눌림 상단)", GroupName = "Long Setup", Order = 5)]
        public double LongRsiMax { get; set; }

        [NinjaScriptProperty]
        [Range(0.0, 100.0)]
        [Display(Name = "Short RSI Min (반등 하단)", GroupName = "Short Setup", Order = 6)]
        public double ShortRsiMin { get; set; }

        [NinjaScriptProperty]
        [Range(0.0, 100.0)]
        [Display(Name = "Short RSI Max (반등 상단)", GroupName = "Short Setup", Order = 7)]
        public double ShortRsiMax { get; set; }

        [NinjaScriptProperty]
        [Display(Name = "Use Stop & Target", GroupName = "Risk", Order = 8)]
        public bool UseStopAndTarget { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "Stop Loss (Ticks)", GroupName = "Risk", Order = 9)]
        public int StopLossTicks { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "Profit Target (Ticks)", GroupName = "Risk", Order = 10)]
        public int ProfitTargetTicks { get; set; }

        #endregion
    }
}
