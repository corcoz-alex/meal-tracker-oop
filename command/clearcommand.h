#pragma once
#include "command.h"
#include <vector>
#include <memory>
#include "../domain/Meal.h"
#include "../repository/IRepository.h"

class ClearCommand : public Command {
    std::shared_ptr<IRepository> repo;
    std::vector<Meal> oldMeals;
public:
    ClearCommand(std::shared_ptr<IRepository> repo, const std::vector<Meal>& meals)
        : repo(repo), oldMeals(meals) {}

    void execute() override {
        for (const auto& meal : repo->getAllMeals()) {
            repo->removeMeal(meal.getId());
        }
    }

    void undo() override {
        for (const auto& meal : oldMeals) {
            repo->addMeal(meal);
        }
    }
}; 
