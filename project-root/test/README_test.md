
Unit Testing Guide

This project includes unit tests. The tests validate the behavior and robustness of the following core modules:

- NutritionDatabase
- FruitClassifier
- WeightSensor


Test Descriptions

| Module                        | Executable                       | Description 
| NutritionDatabase     | ./test_nutrition                | Verifies nutrition value calculation based on weight 
| FruitClassifier              | ./test_fruitclassifier         | Confirms class instantiation without runtime errors 
| WeightSensor             | ./test_weightsensor       | Ensures object constructions are stable


Test File Locations

All test source files are located under the `tests/` directory:

- `tests/test_nutrition.cpp`
- `tests/test_fruitclassifier.cpp`
- `tests/test_weightsensor.cpp`
