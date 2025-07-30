#include "NutritionDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>

NutritionDatabase::NutritionDatabase(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开数据库文件: " + filename);
    }
    
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

Nutrition NutritionDatabase::getNutrition(const std::string& name) {
    auto it = database.find(name);
    if (it == database.end()) {
        throw std::runtime_error("未找到营养信息: " + name);
    }
    return it->second;
}

TotalNutrition calculateNutrition(const Nutrition& nutrition, float weight) {
    return {
        nutrition.protein * weight / 100.0f,  // 每100g含量
        nutrition.carbohydrates * weight / 100.0f,
        nutrition.fat * weight / 100.0f,
        nutrition.calories * weight / 100.0f
    };
}