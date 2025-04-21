#include <iostream>
#include <opencv2/opencv.hpp>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/kernels/register.h"
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <boost/asio.hpp>
#include <thread>  // 包含 std::this_thread
#include <chrono>  // 包含 std::chrono
class WeightSensor {
    public:
        WeightSensor(const std::string& device, int baudRate)
            : io(), serial(io, device) {
            serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
        }
    
        // 读取重量数据
        float readWeight() {
            uint8_t buffer[10];
            boost::asio::read(serial, boost::asio::buffer(buffer, 10));  // 读取 10 字节数据
            // 解析重量数据
            uint32_t weight = (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
            return static_cast<float>(weight);  // 转换为克
        }
    
        // 发送命令函数，支持发送16进制数据
        void sendCommand(const std::vector<uint8_t>& command) {
            size_t sent;
            try {
                boost::asio::write(serial, boost::asio::buffer(command.data(), command.size()));
                std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 等待100毫秒
            } catch (const std::exception& e) {
                std::cerr << "Error sending command: " << e.what() << std::endl;
            }
        }
        // 发送命令函数，支持发送16进制数据
        void sendCommandNoDelay(const std::vector<uint8_t>& command) {
            size_t sent;
            try {
                boost::asio::write(serial, boost::asio::buffer(command.data(), command.size()));
                std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 等待100毫秒
            } catch (const std::exception& e) {
                std::cerr << "Error sending command: " << e.what() << std::endl;
            }
        }
    
    private:
        boost::asio::io_context io;
        boost::asio::serial_port serial;
        float maopi;
    };


// 数据库模块
struct Nutrition {
    float protein;
    float carbohydrates;
    float fat;
    float calories;
};

class NutritionDatabase {
public:
NutritionDatabase(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        Nutrition nutrition;

        // Read the name
        std::getline(ss, token, ',');
        std::string name = token;

        // Read protein
        std::getline(ss, token, ',');
        nutrition.protein = std::stof(token);

        // Read carbohydrates
        std::getline(ss, token, ',');
        nutrition.carbohydrates = std::stof(token);

        // Read fat
        std::getline(ss, token, ',');
        nutrition.fat = std::stof(token);

        // Read calories
        std::getline(ss, token, ',');
        nutrition.calories = std::stof(token);

        // Store in the database
        database[name] = nutrition;

        // Debug print
        std::cerr << "Nutrition for " << name << ": "
                  << "Protein=" << nutrition.protein << ", "
                  << "Carbs=" << nutrition.carbohydrates << ", "
                  << "Fat=" << nutrition.fat << ", "
                  << "Calories=" << nutrition.calories << std::endl;
    }
}
    Nutrition getNutrition(const std::string& name) {
        return database[name];
    }

private:
    std::map<std::string, Nutrition> database;
};

// 营养物质计算模块
struct TotalNutrition {
    float protein;
    float carbohydrates;
    float fat;
    float calories;
};

TotalNutrition calculateNutrition(const Nutrition& nutrition, float weight) {
    return {
        nutrition.protein * weight,
        nutrition.carbohydrates * weight,
        nutrition.fat * weight,
        nutrition.calories * weight
    };
}

TotalNutrition all;

// UI 显示模块
void displayResult(cv::Mat& frame, const std::string& fruit, float weight, const TotalNutrition& nutrition) {
    // Title
    std::string title = "Smart Nutrition Calculator";
    cv::putText(frame, title, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

    // Fruit and Weight Information
    std::string fruitText = "Fruit: " + fruit;
    std::string weightText = "Weight: " + std::to_string(weight) + "g";
    cv::putText(frame, fruitText, cv::Point(10, 70), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, weightText, cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

    // Nutrition Information (in a table-like format)
    std::string proteinText = "Protein: " + std::to_string(nutrition.protein) + "g";
    std::string carbsText = "Carbohydrates: " + std::to_string(nutrition.carbohydrates) + "g";
    std::string fatText = "Fat: " + std::to_string(nutrition.fat) + "g";
    std::string caloriesText = "Calories: " + std::to_string(nutrition.calories) + "kcal";

    cv::putText(frame, proteinText, cv::Point(10, 140), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, carbsText, cv::Point(10, 170), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, fatText, cv::Point(10, 200), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, caloriesText, cv::Point(10, 230), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

    // Separator Line (optional)
    cv::line(frame, cv::Point(10, 240), cv::Point(frame.cols - 10, 240), cv::Scalar(0, 255, 0), 2);
}

std::vector<std::string> labels = {
    "Apple",    "Apple", "Apple",
    "Banana",    "Banana",         "Banana",
    "Blueberry", "Carrot",         "Onion",
    "Orange",    "Tomato"};

// std::vector<std::string> labels = {"apple", "banana", "carrot", "egg",
// "orange", "pepper"};

std::mutex weight_mutex;
float shared_weight = 0.0f;
float shared_maopi_weight = 0.0f;
std::atomic<bool> stop_weight_thread(false);

void readWeightThread(WeightSensor& weightSensor) {
    std::vector<uint8_t> command = {0xA3,0x0,0xA2,0xA4,0xA5};
    while (!stop_weight_thread) {
        weightSensor.sendCommand(command);
        float weight = weightSensor.readWeight();
        if (weight > 0) {
            std::lock_guard<std::mutex> lock(weight_mutex);
            shared_weight = weight;
            std::cerr << "readWeight: " << weight << "g"<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust sleep time as needed
    }
}

int calc_count = 0;
bool is_res = false;

bool is_new = false;

cv::Mat preprocess_image(const cv::Mat& image) {
    cv::Mat resized_image;
    cv::resize(image, resized_image, cv::Size(100, 100));  // 调整图像大小
    resized_image.convertTo(resized_image, CV_32F, 1.0 / 255.0);  // 归一化
    return resized_image;
}

int main() {
    // 加载 TFLite 模型
    std::unique_ptr<tflite::FlatBufferModel> model =
        tflite::FlatBufferModel::BuildFromFile("./fruit_model.tflite");
    if (!model) {
        std::cerr << "Failed to load model." << std::endl;
        return -1;
    }

    // 创建解释器
    tflite::ops::builtin::BuiltinOpResolver resolver;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*model, resolver)(&interpreter);
    if (!interpreter) {
        std::cerr << "无法创建解释器" << std::endl;
        return -1;
    }

    // 分配张量
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        std::cerr << "无法分配张量" << std::endl;
        return -1;
    }

    // 获取输入张量
    int input = interpreter->inputs()[0];
    float* input_data = interpreter->typed_input_tensor<float>(0);


    // 打开摄像头
    //cv::VideoCapture cap("/dev/video0");
	cv::VideoCapture cap(0);
    // cv::VideoCapture cap("libcamerasrc ! video/x-raw,width=640,height=480 ! videoconvert ! appsink", cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera." << std::endl;
        return -1;
    }

    // 初始化称重模块
    // 打开串口 ttyAMA0
    WeightSensor weightSensor("/dev/ttyAMA0", 9600);
    // Start the weight reading thread
    std::thread weight_thread(readWeightThread, std::ref(weightSensor));
    // 发送一个16进制命令
#if 0   //工作方式2
    std::vector<uint8_t> command = {0xA5};
    weightSensor.sendCommand(command);
    std::vector<uint8_t> command2 = {0xAA};
    weightSensor.sendCommand(command2);
    std::vector<uint8_t> command3 = {0xA4};
    weightSensor.sendCommand(command3);
    std::vector<uint8_t> command4 = {0xA4};
    weightSensor.sendCommand(command4);
#endif

    std::cerr << "command send ok" << std::endl;


    // 初始化数据库
    NutritionDatabase database("fruits.csv");

    cv::Mat frame;
    while (true) {
        // 捕获一帧图像
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Failed to capture frame." << std::endl;
            break;
        }
        // 读取重量
        float weight = 0.0f;
        {
            std::lock_guard<std::mutex> lock(weight_mutex);
            weight = shared_weight;  //工作方式2
        }
        if(weight >10 && is_res == false)
        {
            calc_count++;
        }
        
        if(weight <=10 && is_res)
        {
            is_res = false;
            calc_count = 0;
        }


        
        //重量稳定大概5秒
        if(calc_count > 50)
        {
			is_new = true;
            std::cerr << "readWeight: " << weight << "g"<<std::endl;
            is_res = true;
            // 预处理图像
            cv::Mat processed_image = preprocess_image(frame);

            // 将图像数据复制到输入张量
            memcpy(input_data, processed_image.data, interpreter->tensor(input)->bytes);

            // 运行推理
            if (interpreter->Invoke() != kTfLiteOk) {
                std::cerr << "Failed to invoke interpreter." << std::endl;
                break;
            }



            // 获取输出张量
            int output = interpreter->outputs()[0];
            float* output_data = interpreter->typed_output_tensor<float>(0);

            // 获取预测结果
            int predicted_class = std::max_element(output_data, output_data + labels.size()) - output_data;
            float confidence = output_data[predicted_class];
            std::string predicted_label = labels[predicted_class];
        
            // 输出预测结果
            std::cout << "Predicted class: " << predicted_label << " (Confidence: " << confidence << ")" << std::endl;


            // 查询营养成分
            Nutrition nutrition = database.getNutrition(predicted_label);

            // 计算总营养成分
            TotalNutrition total_nutrition = calculateNutrition(nutrition, weight);
			if(is_new)
			{
				is_new = false;
				std::cout << "add new: "<<weight << std::endl;
				all.protein += total_nutrition.protein;
				all.carbohydrates += total_nutrition.carbohydrates;
				all.fat += total_nutrition.fat;
				all.calories += total_nutrition.calories;	
			}

            // 显示结果
            displayResult(frame, predicted_label, weight, all);
        }
        // 显示图像
        cv::imshow("Frame", frame);

        // 按下 'q' 键退出
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    // Stop the weight reading thread
    stop_weight_thread = true;
    weight_thread.join();
    // 释放摄像头
    cap.release();
    cv::destroyAllWindows();

    return 0;
}