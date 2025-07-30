#ifndef FRUITCLASSIFIER_H
#define FRUITCLASSIFIER_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/kernels/register.h"

class FruitClassifier {
public:
    FruitClassifier(const std::string& model_path, const std::vector<std::string>& class_labels);
    std::pair<std::string, float> classify(const cv::Mat& frame);
    
private:
    cv::Mat preprocess_image(const cv::Mat& image);
    
    std::unique_ptr<tflite::FlatBufferModel> model;
    std::unique_ptr<tflite::Interpreter> interpreter;
    std::vector<std::string> labels;
    int input_index;
};

#endif // FRUITCLASSIFIER_H