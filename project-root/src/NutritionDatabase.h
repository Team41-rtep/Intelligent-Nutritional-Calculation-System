#ifndef NUTRITIONDATABASE_H
#define NUTRITIONDATABASE_H

#include <map>
#include <string>

struct Nutrition {
    float protein;
    float carbohydrates;
    float fat;
    float calories;
};

struct TotalNutrition {
    float protein;
    float carbohydrates;
    float fat;
    float calories;
};

class NutritionDatabase {
public:
    NutritionDatabase(const std::string& filename);
    Nutrition getNutrition(const std::string& name);
    
private:
    std::map<std::string, Nutrition> database;
};

TotalNutrition calculateNutrition(const Nutrition& nutrition, float weight);

#endif // NUTRITIONDATABASE_H