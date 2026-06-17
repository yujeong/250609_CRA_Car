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
 * [전략 개요] RSI_VWAP_EMA – MNQ 5계약 분할 청산
 *
 * ▶ 15분봉 (방향 판단)
 *   - 종가 > 20EMA AND > VWAP → 상승장
 *   - 종가 < 20EMA AND < VWAP → 하락장
 *
 * ▶ 3분봉 (진입)
 *   - RSI(14) 눌림/반등 구간에서 방향 전환 확인
 *
 * ▶ 롱 진입
 *   1) 15분봉 상승장
 *   2) 전봉 RSI 40~50 (눌림)
 *   3) 현재 RSI 상승 전환
 *
 * ▶ 숏 진입
 *   1) 15분봉 하락장
 *   2) 전봉 RSI 50~60 (반등)
 *   3) 현재 RSI 하락 전환
 *
 * ▶ 청산 구조 (MNQ 5계약, 1pt = 4 ticks = $2/계약)
 *   - 손절: -20pt  → 전체 손실 $200 (5계약 × $2 × 20)
 *   - PT1:  +30pt  → 3계약 청산, 수익 $180  (손익비 1:1.5)
 *   - PT2:  +40pt  → 2계약 청산, 수익 $160  (손익비 1:2)
 *   - 평균 익절 34pt, 평균 손익비 약 1:1.7
 *
 * ▶ 설치 경로
 *   Documents\NinjaTrader 8\bin\Custom\Strategies\RSI_VWAP_EMA.cs
 */

namespace NinjaTrader.NinjaScript.Strategies
{
    public class RSI_VWAP_EMA : Strategy
    {
        // ── 인디케이터 ───────────────────────────────────────────────
        private EMA  ema3;
        private EMA  ema15;
        private VWAP vwap3;
        private VWAP vwap15;
        private RSI  rsi3;

        // PT1 청산 여부 추적 (PT2 손절 브레이크이븐 이동용)
        private bool pt1Hit;
        private double entryPrice;

        protected override void OnStateChange()
        {
            if (State == State.SetDefaults)
            {
                Description                  = "15min bias (20EMA+VWAP) + 3min RSI entry | MNQ 5계약 분할청산";
                Name                         = "RSI_VWAP_EMA";
                Calculate                    = Calculate.OnBarClose;
                EntriesPerDirection          = 2;   // PT1용 + PT2용
                EntryHandling                = EntryHandling.AllEntries;
                IsExitOnSessionCloseStrategy = true;
                ExitOnSessionCloseSeconds    = 30;

                // 인디케이터
                EmaPeriod    = 20;
                RsiPeriod    = 14;
                RsiSmoothing = 3;

                // RSI 구간
                LongRsiMin  = 40;
                LongRsiMax  = 50;
                ShortRsiMin = 50;
                ShortRsiMax = 60;

                // 손익 설정 (포인트 단위, MNQ 기준)
                StopLossPoints = 20;
                PT1Points      = 30;   // 1:1.5
                PT2Points      = 40;   // 1:2
                MoveToBreakEven = true;  // PT1 도달 시 나머지 손절 → 브레이크이븐

                // 수량 (합계 5계약)
                Qty1 = 3;   // PT1에서 청산
                Qty2 = 2;   // PT2에서 청산

                // MNQ: 1포인트 = 4틱 (틱사이즈 0.25)
                TicksPerPoint = 4;
            }
            else if (State == State.Configure)
            {
                AddDataSeries(BarsPeriodType.Minute, 15);

                int slTicks  = StopLossPoints * TicksPerPoint;
                int pt1Ticks = PT1Points      * TicksPerPoint;
                int pt2Ticks = PT2Points      * TicksPerPoint;

                // 신호명별 손절/익절 설정
                SetStopLoss("Long_1",  CalculationMode.Ticks, slTicks,  false);
                SetStopLoss("Long_2",  CalculationMode.Ticks, slTicks,  false);
                SetProfitTarget("Long_1",  CalculationMode.Ticks, pt1Ticks);
                SetProfitTarget("Long_2",  CalculationMode.Ticks, pt2Ticks);

                SetStopLoss("Short_1", CalculationMode.Ticks, slTicks,  false);
                SetStopLoss("Short_2", CalculationMode.Ticks, slTicks,  false);
                SetProfitTarget("Short_1", CalculationMode.Ticks, pt1Ticks);
                SetProfitTarget("Short_2", CalculationMode.Ticks, pt2Ticks);
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
            if (BarsInProgress != 0) return;
            if (CurrentBars[0] < EmaPeriod + 5 || CurrentBars[1] < EmaPeriod + 5) return;

            // ── 15분봉: 장 방향 ──────────────────────────────────────
            double close15   = Closes[1][0];
            bool bullishBias = close15 > ema15[0] && close15 > vwap15[0];
            bool bearishBias = close15 < ema15[0] && close15 < vwap15[0];

            // ── 3분봉: RSI 전환 감지 ──────────────────────────────────
            double currRsi = rsi3[0];
            double prevRsi = rsi3[1];

            bool prevInLongZone  = prevRsi >= LongRsiMin  && prevRsi <= LongRsiMax;
            bool rsiTurnedUp     = currRsi > prevRsi;

            bool prevInShortZone = prevRsi >= ShortRsiMin && prevRsi <= ShortRsiMax;
            bool rsiTurnedDown   = currRsi < prevRsi;

            // ── 브레이크이븐 이동: PT1 청산 후 PT2 손절을 진입가로 ──────
            if (MoveToBreakEven && pt1Hit && Position.MarketPosition != MarketPosition.Flat)
            {
                double bePrice = entryPrice;
                if (Position.MarketPosition == MarketPosition.Long)
                    ExitLongStopMarket(0, true, Qty2, bePrice, "BE_Stop", "Long_2");
                else if (Position.MarketPosition == MarketPosition.Short)
                    ExitShortStopMarket(0, true, Qty2, bePrice, "BE_Stop", "Short_2");
            }

            // ── 진입 ─────────────────────────────────────────────────
            if (Position.MarketPosition == MarketPosition.Flat)
            {
                pt1Hit = false;

                // 롱: 5계약 진입 (3+2 분할)
                if (bullishBias && prevInLongZone && rsiTurnedUp)
                {
                    entryPrice = Close[0];
                    EnterLong(Qty1, "Long_1");
                    EnterLong(Qty2, "Long_2");
                    Draw.ArrowUp(this, "L_" + CurrentBar, false, 0,
                        Low[0] - TickSize * 3, Brushes.LimeGreen);
                    Print(Time[0] + " [LONG] 진입 @ " + Close[0]
                        + " | RSI: " + currRsi.ToString("F1")
                        + " | 15min: EMA=" + ema15[0].ToString("F2")
                        + " VWAP=" + vwap15[0].ToString("F2"));
                }
                // 숏: 5계약 진입 (3+2 분할)
                else if (bearishBias && prevInShortZone && rsiTurnedDown)
                {
                    entryPrice = Close[0];
                    EnterShort(Qty1, "Short_1");
                    EnterShort(Qty2, "Short_2");
                    Draw.ArrowDown(this, "S_" + CurrentBar, false, 0,
                        High[0] + TickSize * 3, Brushes.Red);
                    Print(Time[0] + " [SHORT] 진입 @ " + Close[0]
                        + " | RSI: " + currRsi.ToString("F1")
                        + " | 15min: EMA=" + ema15[0].ToString("F2")
                        + " VWAP=" + vwap15[0].ToString("F2"));
                }
            }
        }

        protected override void OnExecutionUpdate(
            Execution execution, string executionId, double price,
            int quantity, MarketPosition marketPosition,
            string orderId, DateTime time)
        {
            // PT1 청산 감지 → 브레이크이븐 플래그 설정
            if (execution.Name == "Long_1" || execution.Name == "Short_1")
                pt1Hit = true;
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
        [Range(1, int.MaxValue)]
        [Display(Name = "Stop Loss (포인트)", GroupName = "Risk / MNQ", Order = 8)]
        public int StopLossPoints { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "PT1 익절 (포인트) · Qty1 계약 청산", GroupName = "Risk / MNQ", Order = 9)]
        public int PT1Points { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "PT2 익절 (포인트) · Qty2 계약 청산", GroupName = "Risk / MNQ", Order = 10)]
        public int PT2Points { get; set; }

        [NinjaScriptProperty]
        [Display(Name = "PT1 도달 시 나머지 손절 → 브레이크이븐", GroupName = "Risk / MNQ", Order = 11)]
        public bool MoveToBreakEven { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "Qty1 (PT1 계약 수)", GroupName = "Risk / MNQ", Order = 12)]
        public int Qty1 { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "Qty2 (PT2 계약 수)", GroupName = "Risk / MNQ", Order = 13)]
        public int Qty2 { get; set; }

        [NinjaScriptProperty]
        [Range(1, int.MaxValue)]
        [Display(Name = "Ticks Per Point (MNQ=4)", GroupName = "Risk / MNQ", Order = 14)]
        public int TicksPerPoint { get; set; }

        #endregion
    }
}
