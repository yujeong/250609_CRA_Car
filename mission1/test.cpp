#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "CarFactory.cpp"
#include "Type.h"

class CarAssemble : public testing::Test
{
public:
    const int INVALID_TYPE = 0xFFFFFFFF;
    const int BROKEN_ENGINE = 4;
    CarFactory factory;
    Car* car = factory.createCar(SEDAN, GM, MANDO, MOBIS);

    bool runAndTest() {
        car->run();
        return car->test();
    }
};

TEST_F(CarAssemble, PassCase_Sedan)
{
    car = factory.createCar(SEDAN, GM, MANDO, MOBIS);    
    bool ret = runAndTest();
    EXPECT_EQ(ret, true);

    std::string carType = car->getCarType();
    EXPECT_EQ(carType, "Sedan");
    std::string engine = car->getEngineCompany();
    EXPECT_EQ(engine, "GM");
    std::string brake = car->getBrakeCompany();
    EXPECT_EQ(brake, "MANDO");
    std::string steering = car->getSteeringCompany();
    EXPECT_EQ(steering, "MOBIS");
}

TEST_F(CarAssemble, PassCase_Suv)
{
    car = factory.createCar(SUV, WIA, CONTINENTAL, MOBIS);
    bool ret = runAndTest();
    EXPECT_EQ(ret, true);

    std::string carType = car->getCarType();
    EXPECT_EQ(carType, "Suv");
    std::string engine = car->getEngineCompany();
    EXPECT_EQ(engine, "WIA");
    std::string brake = car->getBrakeCompany();
    EXPECT_EQ(brake, "CONTINENTAL");
    std::string steering = car->getSteeringCompany();
    EXPECT_EQ(steering, "MOBIS");
}

TEST_F(CarAssemble, PassCase_Truck)
{
    car = factory.createCar(TRUCK, TOYOTA, BOSCH_B, BOSCH_S);
    bool ret = runAndTest();

    EXPECT_EQ(ret, true);
    std::string carType = car->getCarType();
    EXPECT_EQ(carType, "Truck");
    std::string engine = car->getEngineCompany();
    EXPECT_EQ(engine, "TOYOTA");
    std::string brake = car->getBrakeCompany();
    EXPECT_EQ(brake, "BOSCH");
    std::string steering = car->getSteeringCompany();
    EXPECT_EQ(steering, "BOSCH");
}

TEST_F(CarAssemble, SUV_BrokenEngine)
{
    car = factory.createCar(SUV, BROKEN_ENGINE, BOSCH_B, BOSCH_S);
    bool ret = runAndTest();
    EXPECT_EQ(ret, true);

    std::string engine = car->getEngineCompany();
    EXPECT_EQ(engine, "BROKEN");
}

TEST_F(CarAssemble, InvalidCase_Sedan_ContientalBrake)
{
    car = factory.createCar(SEDAN, GM, CONTINENTAL, BOSCH_S);
    bool ret = runAndTest();
    EXPECT_EQ(ret, false);   
}

TEST_F(CarAssemble, InvalidCase_SUV_ToyotaEngine)
{
    car = factory.createCar(SUV, TOYOTA, CONTINENTAL, BOSCH_S);
    bool ret = runAndTest();
    EXPECT_EQ(ret, false);
}

TEST_F(CarAssemble, InvalidCase_Truck_WIAEngine)
{
    car = factory.createCar(TRUCK, WIA, CONTINENTAL, BOSCH_S);
    bool ret = car->test();
    EXPECT_EQ(ret, false);
}

TEST_F(CarAssemble, InvalidCase_Truck_MandoBrake)
{
    // wia error도 출력되도록 함
    car = factory.createCar(TRUCK, WIA, MANDO, BOSCH_S);
    bool ret = runAndTest();
    EXPECT_EQ(ret, false);
}

TEST_F(CarAssemble, InvalidCase_BoschEngine_NotBoschSteering)
{
    car = factory.createCar(TRUCK, GM, BOSCH_B, MOBIS);
    bool ret = runAndTest();
    EXPECT_EQ(ret, false);

    car = factory.createCar(SEDAN, GM, BOSCH_B, MOBIS);
    ret = runAndTest();
    EXPECT_EQ(ret, false);

    car = factory.createCar(SUV, GM, BOSCH_B, MOBIS);
    ret = runAndTest();
    EXPECT_EQ(ret, false);
}

TEST_F(CarAssemble, InvalidCarType)
{
    car = factory.createCar(INVALID_TYPE, GM, BOSCH_B, MOBIS);
    bool ret = car == nullptr ? true : false;
    EXPECT_EQ(ret, true);
}

TEST_F(CarAssemble, InvalidEngine)
{
    car = factory.createCar(SUV, INVALID_TYPE, BOSCH_B, MOBIS);
    bool ret = car == nullptr ? true : false;
    EXPECT_EQ(ret, true);
}

TEST_F(CarAssemble, InvalidBrake)
{
    car = factory.createCar(SUV, GM, INVALID_TYPE, MOBIS);
    bool ret = car == nullptr ? true : false;
    EXPECT_EQ(ret, true);
}
TEST_F(CarAssemble, InvalidSteering)
{
    car = factory.createCar(SUV, GM, BOSCH_B, INVALID_TYPE);
    bool ret = car == nullptr ? true : false;
    EXPECT_EQ(ret, true);
}

#ifdef _DEBUG
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif