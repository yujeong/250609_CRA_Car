#include "Car.cpp"

class CarFactory
{
public:
	Car* createCar(int carType, int e, int b, int s)
	{
		Engine* engine = createEngine(e);
		Brake* brake = createBrake(b);
		Steering* steering = createSteering(s);
		switch (carType)
		{
		case SEDAN:	
			return new Sedan(engine, brake, steering);
			break;

		case SUV:
			return new Suv(engine, brake, steering);
			break;

		case TRUCK:
			return new Truck(engine, brake, steering);
			break;

		default:
			break;
		}
	}
private:
	Engine* createEngine(int engine)
	{
		switch (engine)
		{
		case GM:
			return new GMEngine();
			break;

		case TOYOTA:
			return new ToyotaEngine();
			break;

		case WIA:
			return new WIAEngine();
			break;

		default:
			return new BrokenEngine();
			break;
		}
	}

	Brake* createBrake(int brake)
	{
		switch (brake)
		{
		case MANDO:
			return new MandoBrake();
			break;

		case CONTINENTAL:
			return new ContinentalBrake();
			break;

		case BOSCH_B:
			return new BoschBrake();
			break;

		default:
			break;
		}
	}

	Steering* createSteering(int steering)
	{

		switch (steering)
		{
		case BOSCH_S:
			return new BoschStreering();
			break;

		case MOBIS:
			return new MobisStreering();
			break;

		default:
			break;
		}
	}

};