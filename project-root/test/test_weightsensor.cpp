
#include <gtest/gtest.h>
#include "WeightSensor.h"

TEST(WeightSensorTest, ConstructDestructNoCrash) {
    try {
        WeightSensor sensor("/dev/null", 9600);  // 使用虚假串口设备
        SUCCEED();
    } catch (...) {
        FAIL() << "Construction failed unexpectedly.";
    }
}
