#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include <chrono>
#include "FruitClassifier.h"
#include "WeightSensor.h"
#include "NutritionDatabase.h"

// UI 显示模块
void displayResult(cv::Mat& frame, const std::string& fruit, float weight, const TotalNutrition& nutrition) {
    // Title
    std::string title = "Smart Nutrition Calculator";
    cv::putText(frame, title, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

    // Fruit and Weight Information
    std::string fruitText = "Fruit: " + fruit;
    std::string weightText = "Weight: " + std::to_string(static_cast<int>(weight)) + "g";
    cv::putText(frame, fruitText, cv::Point(10, 70), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, weightText, cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

    // Nutrition Information
    std::string proteinText = "Protein: " + std::to_string(nutrition.protein) + "g";
    std::string carbsText = "Carbohydrates: " + std::to_string(nutrition.carbohydrates) + "g";
    std::string fatText = "Fat: " + std::to_string(nutrition.fat) + "g";
    std::string caloriesText = "Calories: " + std::to_string(static_cast<int>(nutrition.calories)) + "kcal";

    cv::putText(frame, proteinText, cv::Point(10, 140), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, carbsText, cv::Point(10, 170), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, fatText, cv::Point(10, 200), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, caloriesText, cv::Point(10, 230), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

    // Separator Line
    cv::line(frame, cv::Point(10, 240), cv::Point(frame.cols - 10, 240), cv::Scalar(0, 255, 0), 2);
}

int main() {
    // 标签定义
    std::vector<std::string> labels = {
        "Apple", "Apple", "Apple", "Banana", "Banana", "Banana",
        "Blueberry", "Carrot", "Onion", "Orange", "Tomato"
    };

    try {
        // 初始化水果分类器
        FruitClassifier classifier("./fruit_model.tflite", labels);
        
        // 打开摄像头
        //cv::VideoCapture cap(0);
        cv::VideoCapture cap("/dev/video16");
        if (!cap.isOpened()) {
            throw std::runtime_error("Failed to open camera.");
        }
        
        // 初始化称重模块
        WeightSensor weightSensor("/dev/ttyAMA0", 9600);
        
        // 初始化数据库
        NutritionDatabase database("fruits.csv");
        
        cv::Mat frame;
        float weight = 0.0f;
        std::string last_fruit;
        TotalNutrition last_nutrition;
        
        // 初始校准
        //weightSensor.sendCommand({0xA5});
        //weightSensor.sendCommand({0xAA});
        //weightSensor.sendCommand({0xA4});
        
        while (true) {
            cap >> frame;
            if (frame.empty()) {
                std::cerr << "Failed to capture frame." << std::endl;
                break;
            }
            
            // 请求重量测量
            weightSensor.requestWeight();
            
            // 获取重量
            weight = weightSensor.getWeight();
            std::cerr << "Measured weight: " << weight << "g" << std::endl;
            
            if (weight > 10) {
                // 使用分类器进行水果识别
                auto [fruit, confidence] = classifier.classify(frame);
                std::cerr << "Predicted: " << fruit << " (Confidence: " << confidence << ")" << std::endl;
                
                // 查询营养成分
                Nutrition nutrition = database.getNutrition(fruit);
                
                // 计算总营养成分
                TotalNutrition total_nutrition = calculateNutrition(nutrition, weight);
                
                // 保存结果用于显示
                last_fruit = fruit;
                last_nutrition = total_nutrition;
                
                // 显示结果
                displayResult(frame, fruit, weight, total_nutrition);
            } else {
                // 显示上一次有效结果（如果有）
                if (!last_fruit.empty()) {
                    displayResult(frame, last_fruit, weight, last_nutrition);
                }
            }
            
            // 显示图像
            cv::imshow("Smart Nutrition Scale", frame);
            
            // 按下 'q' 键退出
            if (cv::waitKey(1) == 'q') {
                break;
            }
        }
        
        // 释放摄像头
        cap.release();
        cv::destroyAllWindows();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}