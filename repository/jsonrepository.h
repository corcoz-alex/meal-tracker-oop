#pragma once
#include "IRepository.h"
#include <vector>
#include <string>

class JSONRepository : public IRepository {
private:
    std::vector<Meal> meals;
    std::string filename;

public:
    JSONRepository(const std::string& filename);

    void addMeal(const Meal& meal) override;
    void removeMeal(int id) override;
    void updateMeal(const Meal& meal) override;
    std::vector<Meal> getAllMeals() const override;
    Meal getMealById(int id) const override;

    void load() override;
    void save() const override;
};
