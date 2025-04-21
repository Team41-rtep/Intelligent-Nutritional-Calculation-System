cmake_minimum_required(VERSION 3.10)

project(SmartNutritionCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

find_package(Boost REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(smart_nutrition_calculator
    src/main.cc
    src/weight_sensor.cc
)

target_link_libraries(smart_nutrition_calculator
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    pthread
)
