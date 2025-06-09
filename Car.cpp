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
			printf("�ڵ����� ���۵��� �ʽ��ϴ�\n");
		}
		else if (isBrokenEngine) {
			printf("������ ���峪�ֽ��ϴ�.\n");
			printf("�ڵ����� �������� �ʽ��ϴ�.\n");
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
		printf("�ڵ��� ��ǰ ���� �׽�Ʈ ��� : %s\n", result.c_str());
		if (!isValid)
		{
			if(brake->getCompany() == "CONTINENTAL")
				printf("Sedan���� Continental������ġ ��� �Ұ�\n");
			if ((brake->getCompany() == "BOSCH") && (steering->getCompany() != "BOSCH")) {
				printf("Bosch������ġ���� %s������ġ ��� �Ұ�\n", steering->getCompany().c_str());
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
		printf("�ڵ��� ��ǰ ���� �׽�Ʈ ��� : %s\n", result.c_str());
		if (!isValid)
		{
			if(engine->getCompany() == "TOYOTA")
				printf("SUV���� TOYOTA���� ��� �Ұ�\n");
			if ((brake->getCompany() == "BOSCH") && (steering->getCompany() != "BOSCH")) {
				printf("Bosch������ġ���� %s������ġ ��� �Ұ�\n", steering->getCompany().c_str());
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
		printf("�ڵ��� ��ǰ ���� �׽�Ʈ ��� : %s\n", result.c_str());
		if (!isValid)
		{
			if (engine->getCompany() == "WIA")
				printf("Truck���� WIA���� ��� �Ұ�\n");
			if (brake->getCompany() == "MANDO")
				printf("Truck���� Mando������ġ ��� �Ұ�\n");
			if ((brake->getCompany() == "BOSCH") && (steering->getCompany() != "BOSCH")) {
				printf("Bosch������ġ���� %s������ġ ��� �Ұ�\n", steering->getCompany().c_str());
			}
		
		}
		return isValid;
	}

	std::string getCarType() override
	{
		return "Truck";
	}
};
