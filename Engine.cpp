#pragma once
#include <stdio.h>
#include <string>

class Engine
{
public:
	virtual std::string getCompany() = 0;
};

class GMEngine : public Engine
{
public:
	std::string getCompany() override
	{
		return "GM";
	}
};

class ToyotaEngine : public Engine
{
public:
	std::string getCompany() override
	{
		return "TOYOTA";
	}
};

class WIAEngine : public Engine
{
public:
	std::string getCompany() override
	{
		return "WIA";
	}
};

class BrokenEngine : public Engine
{
public:
	std::string getCompany() override
	{
		return "BROKEN";
	}
};