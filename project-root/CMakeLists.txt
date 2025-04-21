cmake_minimum_required(VERSION 3.10)

project(SmartNutritionCalculator)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Dependencies
# -----------------------------

# OpenCV
find_package(OpenCV REQUIRED)

# Boost (for asio)
find_package(Boost REQUIRED)

# Google Test (for unit testing)
find_package(GTest REQUIRED)

# -----------------------------
# Include directories
# -----------------------------
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

# -----------------------------
# Main Application
# -----------------------------
add_executable(smart_nutrition_calculator
    src/main.cc
    src/weight_sensor.cc
)

target_link_libraries(smart_nutrition_calculator
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    pthread
)

# -----------------------------
# Unit Testing
# -----------------------------
enable_testing()

add_executable(test_nutrition
    tests/test_nutrition.cpp
)

target_link_libraries(test_nutrition
    GTest::GTest
    GTest::Main
    pthread
)

add_test(NAME NutritionTest COMMAND test_nutrition)

# -----------------------------
# Optional: Print status
# -----------------------------
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "OpenCV found: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
message(STATUS "GTest found: ${GTEST_INCLUDE_DIRS}")

