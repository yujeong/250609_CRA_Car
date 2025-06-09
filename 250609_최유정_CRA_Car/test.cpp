#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "CarFactory.cpp"
#include "Type.h"

class CarAssemble : public testing::Test
{
public:
    CarFactory factory;
};

TEST_F(CarAssemble, PassCase_Sedan)
{
    Car* car = factory.createCar(SEDAN, GM, MANDO, MOBIS);
    car->run();
    
    bool ret = car->test();
    EXPECT_EQ(ret, true);
}

TEST_F(CarAssemble, PassCase_Suv)
{
    Car* car = factory.createCar(SUV, GM, CONTINENTAL, MOBIS);
    car->run();

    bool ret = car->test();
    EXPECT_EQ(ret, true);
}

TEST_F(CarAssemble, PassCase_Truck)
{
    Car* car = factory.createCar(TRUCK, GM, BOSCH_B, BOSCH_S);
    car->run();

    bool ret = car->test();
    EXPECT_EQ(ret, true);
}

TEST_F(CarAssemble, BrokenEngine)
{
    Car* car = factory.createCar(SUV, 4, BOSCH_B, BOSCH_S);
    bool ret = car->IsBrokenEngine();
    EXPECT_EQ(ret, true);
}

TEST_F(CarAssemble, InvalidCase_Sedan_ContientalBrake)
{
    Car* car = factory.createCar(SEDAN, GM, CONTINENTAL, BOSCH_S);
    bool ret = car->test();
    EXPECT_EQ(ret, false);   
}

TEST_F(CarAssemble, InvalidCase_SUV_ToyotaEngine)
{
    Car* car = factory.createCar(SUV, TOYOTA, CONTINENTAL, BOSCH_S);
    bool ret = car->test();
    EXPECT_EQ(ret, false);
}

TEST_F(CarAssemble, InvalidCase_Truck_WIAEngine)
{
    Car* car = factory.createCar(TRUCK, WIA, CONTINENTAL, BOSCH_S);
    bool ret = car->test();
    EXPECT_EQ(ret, false);
}

TEST_F(CarAssemble, InvalidCase_Truck_MandoBrake)
{
    // wia errorµµ Ãâ·ÂµÊ
    Car* car = factory.createCar(TRUCK, WIA, MANDO, BOSCH_S);
    bool ret = car->test();
    EXPECT_EQ(ret, false);
}

TEST_F(CarAssemble, InvalidCase_BoschEngine_NotBoschSteering)
{
    Car* car = factory.createCar(TRUCK, GM, BOSCH_B, MOBIS);
    bool ret = car->test();
    EXPECT_EQ(ret, false);

    car = factory.createCar(SEDAN, GM, BOSCH_B, MOBIS);
    ret = car->test();
    EXPECT_EQ(ret, false);

    car = factory.createCar(SUV, GM, BOSCH_B, MOBIS);
    ret = car->test();
    EXPECT_EQ(ret, false);
}

#ifdef _DEBUG
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif