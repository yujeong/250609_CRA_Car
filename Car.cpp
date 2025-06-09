#pragma once
#include "Engine.cpp"
#include "Brake.cpp"
#include "Steering.cpp"
#include "Type.h"
#include <string>

class Car
{
public:
	virtual bool test() = 0;
	virtual std::string getCarType() = 0;

	void run() {
		if (!isValid) {
			printf("자동차가 동작되지 않습니다\n");
		}
		else if (isBrokenEngine) {
			printf("엔진이 고장나있습니다.\n");
			printf("자동차가 움직이지 않습니다.\n");
		}
		else {
			printf("Car Type : %s\n", this->getCarType().c_str());
			printf("Engine : %s\n", engine->getCompany().c_str());
			printf("Brake System : %s\n", brake->getCompany().c_str());
			printf("SteeringSystem : %s\n", steering->getCompany().c_str());
		}
	}

	std::string getEngineCompany() {
		return engine->getCompany();
	}

	std::string getBrakeCompany() {
		return brake->getCompany();
	}
	std::string getSteeringCompany() {
		return steering->getCompany();
	}

protected:
	bool isValid = true;
	bool isBrokenEngine = false;

	Engine* engine;
	Brake* brake;
	Steering* steering;

	void checkBoschSpecific() {
		if ((brake->getCompany() == "BOSCH") && (steering->getCompany() != "BOSCH")) {
			isValid = false;
		}
	}
	void checkBrokenEngine() {
		if (engine->getCompany() == "BROKEN") {
			isBrokenEngine = true;
		}
	}
};

class Sedan : public Car
{
public:
	Sedan(Engine* e, Brake* b, Steering* s)
	{
		engine = e;
		brake = b;
		steering = s;

		if (b->getCompany() == "CONTINENTAL")
		{
			isValid = false;
		}

		checkBoschSpecific();
		checkBrokenEngine();

	}

	bool test() override {
		std::string result = isValid ? "PASS" : "FAIL";
		printf("자동차 부품 조합 테스트 결과 : %s\n", result.c_str());
		if (!isValid)
		{
			if(brake->getCompany() == "CONTINENTAL")
				printf("Sedan에는 Continental제동장치 사용 불가\n");
			if ((brake->getCompany() == "BOSCH") && (steering->getCompany() != "BOSCH")) {
				printf("Bosch제동장치에는 %s조향장치 사용 불가\n", steering->getCompany().c_str());
			}
		}
		return isValid;
	}

	std::string getCarType() override
	{
		return "Sedan";
	}
};

class Suv : public Car
{
public:
	Suv(Engine* e, Brake* b, Steering* s)
	{
		engine = e;
		brake = b;
		steering = s;

		if (e->getCompany() == "TOYOTA")
		{
			isValid = false;
		}

		checkBoschSpecific();
		checkBrokenEngine();
	}

	bool test() override {
		std::string result = isValid ? "PASS" : "FAIL";
		printf("자동차 부품 조합 테스트 결과 : %s\n", result.c_str());
		if (!isValid)
		{
			if(engine->getCompany() == "TOYOTA")
				printf("SUV에는 TOYOTA엔진 사용 불가\n");
			if ((brake->getCompany() == "BOSCH") && (steering->getCompany() != "BOSCH")) {
				printf("Bosch제동장치에는 %s조향장치 사용 불가\n", steering->getCompany().c_str());
			}
		}
		return isValid;
	}

	std::string getCarType() override
	{
		return "Suv";
	}
};



class Truck : public Car
{
public:
	Truck(Engine* e, Brake* b, Steering* s)
	{
		engine = e;
		brake = b;
		steering = s;

		if (e->getCompany() == "WIA" || b->getCompany() == "MANDO")
		{
			isValid = false;
		}

		checkBoschSpecific();
		checkBrokenEngine();
	}

	bool test() override {
		std::string result = isValid ? "PASS" : "FAIL";
		printf("자동차 부품 조합 테스트 결과 : %s\n", result.c_str());
		if (!isValid)
		{
			if (engine->getCompany() == "WIA")
				printf("Truck에는 WIA엔진 사용 불가\n");
			if (brake->getCompany() == "MANDO")
				printf("Truck에는 Mando제동장치 사용 불가\n");
			if ((brake->getCompany() == "BOSCH") && (steering->getCompany() != "BOSCH")) {
				printf("Bosch제동장치에는 %s조향장치 사용 불가\n", steering->getCompany().c_str());
			}
		
		}
		return isValid;
	}

	std::string getCarType() override
	{
		return "Truck";
	}
};
