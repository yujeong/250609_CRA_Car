#pragma once
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

#include "CarFactory.cpp"
#include "Type.h"

#define CLEAR_SCREEN "\033[H\033[2J"

void delay(int ms);
void printMenu(int step);
int inputMenu(int& answer);
void HandleStep(int& step, int answer);
bool IsInputNumber(char* buf, int& answer);

void selectCarType(int answer, int& step);
void selectEngine(int answer, int& step);
void selectBrake(int answer, int& step);
void selectSteering(int answer, int& step);
void selectRunOrTest(int answer, int& step);
Car* makeCar(int carType, int engineType, int brakeType, int steeringType);

char carTypeStr[4][15] = {"", "SEDAN", "SUV", "TRUCK"};
char engineeStr[5][15] = { "", "GM", "TOYOTA", "WIA", "���峭"};
char brakeSystemStr[4][15] = { "", "MANDO", "CONTINENTAL", "BOSCH_B" };
char steeringSystemStr[3][15] = { "", "BOSCH_S", "MOBIS" };

int carType;
int engineType;
int brakeType;
int steeringType;
Car* car;

#ifndef _DEBUG
void delay(int ms)
{
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            for (int t = 0; t < ms; t++)
            {
                sum++;
            }
        }
    }
}

int main()
{    
    int answer;
    int step = CarType_Q;

    while (1)
    {
        printMenu(step);

        int result = inputMenu(answer);
        if (result == -1) break;
        if (result == -2) continue;

        HandleStep(step, answer);
    }
}

void HandleStep(int& step, int answer)
{
    switch (step)
    {
    case CarType_Q:
        selectCarType(answer, step);
        break;

    case Engine_Q:
        selectEngine(answer, step);        
        break;

    case brakeSystem_Q:
        selectBrake(answer, step);
        break;

    case SteeringSystem_Q:
        selectSteering(answer, step);
        break;

    case Run_Test:
        car = makeCar(carType, engineType, brakeType, steeringType);
        selectRunOrTest(answer, step);
        break;
    }
}

void selectCarType(int answer, int& step)
{
    if (!(answer >= 1 && answer <= 3))
    {
        printf("ERROR :: ���� Ÿ���� 1 ~ 3 ������ ���� ����\n");
        delay(800);
        return;
    }

    printf("���� Ÿ������ %s�� �����ϼ̽��ϴ�.\n", carTypeStr[answer]);
    delay(800);
    step = Engine_Q;
    carType = answer;
}

void selectEngine(int answer, int& step)
{
    if (!(answer >= 0 && answer <= 4))
    {
        printf("ERROR :: ������ 1 ~ 4 ������ ���� ����\n");
        delay(800);
        return;;
    }
    if (answer == 0) {
        step--;
        return;
    }

    printf("%s ������ �����ϼ̽��ϴ�.\n", engineeStr[answer]);
    delay(800);
    step = brakeSystem_Q;
    engineType = answer;
}


void selectBrake(int answer, int& step)
{
    if (!(answer >= 0 && answer <= 3))
    {
        printf("ERROR :: ������ġ�� 1 ~ 3 ������ ���� ����\n");
        delay(800);
        return;
    }
    if (answer == 0)
    {
        step--;
        return;
    }
    printf("%s ������ġ�� �����ϼ̽��ϴ�.\n", brakeSystemStr[answer]);
    delay(800);
    step = SteeringSystem_Q;
    brakeType = answer;
}


void selectSteering(int answer, int& step)
{
    if (!(answer >= 0 && answer <= 2))
    {
        printf("ERROR :: ������ġ�� 1 ~ 2 ������ ���� ����\n");
        delay(800);
        return;
    }
    if (answer == 0)
    {
        step--;
        return;
    }
    delay(800);
    step = Run_Test;
    steeringType = answer;
}

Car* makeCar(int carType, int engineType, int brakeType, int steeringType)
{
    CarFactory carFactory;
    return carFactory.createCar(carType, engineType, brakeType, steeringType);
}

void selectRunOrTest(int answer, int& step)
{
    if (answer == 0)
    {
        step = CarType_Q;
        return;
    }
    else if (answer == 1) {
        car->run();
        delay(2000);
    }
    else if (answer == 2) {
        printf("Test...\n");
        delay(1500);
        car->test();
        delay(2000);
    }
    else
    {
        printf("ERROR :: Run �Ǵ� Test �� �ϳ��� ���� �ʿ�\n");
        delay(800);
        return;
    }
}

void printMenu(int step)
{
    if (step == CarType_Q)
    {
        printf(CLEAR_SCREEN);
        printf("        ______________\n");
        printf("       /|            | \n");
        printf("  ____/_|_____________|____\n");
        printf(" |                      O  |\n");
        printf(" '-(@)----------------(@)--'\n");
        printf("===============================\n");
        printf("� ���� Ÿ���� �����ұ��?\n");
        printf("1. Sedan\n");
        printf("2. SUV\n");
        printf("3. Truck\n");
    }
    else if (step == Engine_Q)
    {
        printf(CLEAR_SCREEN);
        printf("� ������ ž���ұ��?\n");
        printf("0. �ڷΰ���\n");
        printf("1. GM\n");
        printf("2. TOYOTA\n");
        printf("3. WIA\n");
        printf("4. ���峭 ����\n");
    }
    else if (step == brakeSystem_Q)
    {
        printf(CLEAR_SCREEN);
        printf("� ������ġ�� �����ұ��?\n");
        printf("0. �ڷΰ���\n");
        printf("1. MANDO\n");
        printf("2. CONTINENTAL\n");
        printf("3. BOSCH\n");
    }
    else if (step == SteeringSystem_Q)
    {
        printf(CLEAR_SCREEN);
        printf("� ������ġ�� �����ұ��?\n");
        printf("0. �ڷΰ���\n");
        printf("1. BOSCH\n");
        printf("2. MOBIS\n");
    }
    else if (step == Run_Test)
    {
        printf(CLEAR_SCREEN);
        printf("���� ������ �ϼ��Ǿ����ϴ�.\n");
        printf("� ������ �ұ��?\n");
        printf("0. ó�� ȭ������ ���ư���\n");
        printf("1. RUN\n");
        printf("2. Test\n");
    }
    printf("===============================\n");

    printf("INPUT > ");    
}

int inputMenu(int& answer)
{       
    char buf[100];
    fgets(buf, sizeof(buf), stdin);

    char* context = nullptr;
    strtok_s(buf, "\r", &context);
    strtok_s(buf, "\n", &context);

    if (!strcmp(buf, "exit"))
    {
        printf("���̹���\n");
        return -1;
    }

    char* checkNumber;
    answer = strtol(buf, &checkNumber, 10); 

    if (*checkNumber != '\0')
    {
        printf("ERROR :: ���ڸ� �Է� ����\n");
        delay(800);
        return -2;
    }

    return 0;
}
#endif