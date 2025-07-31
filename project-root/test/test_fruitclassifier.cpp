
#include <gtest/gtest.h>
#include "FruitClassifier.h"

TEST(FruitClassifierTest, ConstructWithoutCrash) {
    std::vector<std::string> labels = {"Apple", "Banana"};
    try {
        FruitClassifier classifier("dummy_model.tflite", labels);
        SUCCEED();
    } catch (...) {
        FAIL() << "Construction failed unexpectedly.";
    }
}
