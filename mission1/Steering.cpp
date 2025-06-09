#pragma once
class Steering
{
public:
	virtual std::string getCompany() = 0;
};

class BoschStreering : public Steering
{
public:
	std::string getCompany() override {
		return "BOSCH";
	}
};

class MobisStreering : public Steering
{
public:
	std::string getCompany() override {
		return "MOBIS";
	}
};