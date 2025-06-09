#include "Car.cpp"

class CarFactory
{
public:
	Car* createCar(int carType, int e, int b, int s)
	{
		Engine* engine = createEngine(e);
		Brake* brake = createBrake(b);
		Steering* steering = createSteering(s);

		if (engine == nullptr || brake == nullptr || steering == nullptr)
			return nullptr;

		switch (carType)
		{
		case SEDAN:	
			return new Sedan(engine, brake, steering);

		case SUV:
			return new Suv(engine, brake, steering);

		case TRUCK:
			return new Truck(engine, brake, steering);
		}

		return nullptr;
	}

private:
	Engine* createEngine(int engine)
	{
		switch (engine)
		{
		case GM:
			return new GMEngine();

		case TOYOTA:
			return new ToyotaEngine();

		case WIA:
			return new WIAEngine();

		case BROKEN_ENGINE:
			return new BrokenEngine();
		}

		return nullptr;
	}

	Brake* createBrake(int brake)
	{
		switch (brake)
		{
		case MANDO:
			return new MandoBrake();

		case CONTINENTAL:
			return new ContinentalBrake();

		case BOSCH_B:
			return new BoschBrake();
		}

		return nullptr;
	}

	Steering* createSteering(int steering)
	{
		switch (steering)
		{
		case BOSCH_S:
			return new BoschStreering();

		case MOBIS:
			return new MobisStreering();
		}
		return nullptr;
	}

};