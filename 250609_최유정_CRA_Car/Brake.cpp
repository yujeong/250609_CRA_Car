#pragma once
class Brake
{
public:
	virtual std::string getCompany() = 0;

};

class MandoBrake : public Brake
{
public:
	std::string getCompany() override
	{
		return "MANDO";
	}
};

class ContinentalBrake : public Brake
{
public:
	std::string getCompany() override
	{
		return "CONTINENTAL";
	}
};

class BoschBrake : public Brake
{
public:
	std::string getCompany() override
	{
		return "BOSCH";
	}
};