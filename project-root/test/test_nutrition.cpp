
#include <gtest/gtest.h>
#include "NutritionDatabase.h"

TEST(NutritionDatabaseTest, CalculateNutritionAccuracy) {
    Nutrition n = {1.2f, 15.0f, 0.3f, 80.0f};  // 每100g
    float weight = 250.0f;  // g

    TotalNutrition result = calculateNutrition(n, weight);

    EXPECT_NEAR(result.protein, 3.0f, 0.01);
    EXPECT_NEAR(result.carbohydrates, 37.5f, 0.01);
    EXPECT_NEAR(result.fat, 0.75f, 0.01);
    EXPECT_NEAR(result.calories, 200.0f, 0.01);
}
