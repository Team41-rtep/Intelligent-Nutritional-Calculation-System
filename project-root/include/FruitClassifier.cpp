#include "FruitClassifier.h"
#include <algorithm>
#include <stdexcept>

FruitClassifier::FruitClassifier(const std::string& model_path, const std::vector<std::string>& class_labels) 
    : labels(class_labels) {
    // 加载模型
    model = tflite::FlatBufferModel::BuildFromFile(model_path.c_str());
    if (!model) {
        throw std::runtime_error("Failed to load model.");
    }
    
    // 创建解释器
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*model, resolver)(&interpreter);
    if (!interpreter) {
        throw std::runtime_error("无法创建解释器");
    }
    
    // 分配张量
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        throw std::runtime_error("无法分配张量");
    }
    
    // 获取输入张量
    input_index = interpreter->inputs()[0];
}

cv::Mat FruitClassifier::preprocess_image(const cv::Mat& image) {
    cv::Mat resized_image;
    cv::resize(image, resized_image, cv::Size(100, 100));  // 调整图像大小
    resized_image.convertTo(resized_image, CV_32F, 1.0 / 255.0);  // 归一化
    return resized_image;
}

std::pair<std::string, float> FruitClassifier::classify(const cv::Mat& frame) {
    // 预处理图像
    cv::Mat processed_image = preprocess_image(frame);
    
    // 获取输入张量指针
    TfLiteTensor* input_tensor = interpreter->tensor(input_index);
    float* input_data = interpreter->typed_input_tensor<float>(input_index);
    
    // 确保输入张量大小与图像数据匹配
    const size_t num_pixels = processed_image.rows * processed_image.cols * processed_image.channels();
    if (static_cast<size_t>(input_tensor->bytes) != num_pixels * sizeof(float)) {
        throw std::runtime_error("输入张量大小与图像数据不匹配");
    }
    
    // 使用 std::copy 代替 memcpy
    if (processed_image.isContinuous()) {
        // 连续内存块可以直接复制
        const float* src = processed_image.ptr<float>(0);
        std::copy(src, src + num_pixels, input_data);
    } else {
        // 非连续内存需要逐行复制
        for (int i = 0; i < processed_image.rows; ++i) {
            const float* row_ptr = processed_image.ptr<float>(i);
            std::copy(row_ptr, row_ptr + processed_image.cols * processed_image.channels(), 
                      input_data + i * processed_image.cols * processed_image.channels());
        }
    }
    
    // 运行推理
    if (interpreter->Invoke() != kTfLiteOk) {
        throw std::runtime_error("Failed to invoke interpreter.");
    }
    
    // 获取输出张量
    int output_index = interpreter->outputs()[0];
    float* output_data = interpreter->typed_output_tensor<float>(output_index);
    const int num_classes = labels.size();
    
    // 获取预测结果
    int predicted_class = std::distance(
        output_data, 
        std::max_element(output_data, output_data + num_classes)
    );
    float confidence = output_data[predicted_class];
    std::string predicted_label = labels[predicted_class];
    
    return {predicted_label, confidence};
}