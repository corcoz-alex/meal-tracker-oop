#include "Controller.h"
#include "../command/addcommand.h"
#include "../command/removecommand.h"
#include "../command/updatecommand.h"
#include "../command/clearcommand.h"


Controller::Controller(unique_ptr<IRepository> repo) {
    repository = shared_ptr<IRepository>(std::move(repo));
}

void Controller::load() {
    repository->load();
    syncNextId();
}

void Controller::save() {
    repository->save();
}

void Controller::syncNextId(){
    int maxId = 0;
    for(const auto& meal : repository->getAllMeals()) {
        if (meal.getId() > maxId)
            maxId = meal.getId();
    }
    nextId = maxId + 1;
}

void Controller::addMeal(const string& name, int calories, const string& type, const QDateTime& dateTime){
    Meal meal(nextId++, name, calories, type, dateTime);
    auto cmd = make_unique<AddCommand>(repository, meal);
    cmd->execute();
    undoStack.push(std::move(cmd));
    redoStack = {};
}

void Controller::removeMeal(int id){
    Meal toRemove = repository->getMealById(id);

    auto cmd = make_unique<RemoveCommand>(repository, toRemove);
    cmd->execute();
    undoStack.push(std::move(cmd));
    redoStack = {};
}

void Controller::updateMeal(int id, const string& name, int calories, const string& type, const QDateTime& dateTime){
    Meal oldMeal = repository->getMealById(id);
    Meal updatedMeal(id, name, calories, type, dateTime);

    auto cmd = make_unique<UpdateCommand>(repository, oldMeal, updatedMeal);
    cmd->execute();
    undoStack.push(std::move(cmd));
    redoStack = {};

}

vector<Meal> Controller::getAllMeals() const{
    return repository->getAllMeals();
}

Meal Controller::getMealById(int id) const{
    return repository->getMealById(id);
}

std::vector<Meal> Controller::filterMeals(const FilterStrategy& strategy) const {
    std::vector<Meal> result;
    for (const auto& meal : getAllMeals()) {
        if (strategy.matches(meal)) {
            result.push_back(meal);
        }
    }
    return result;
    }

void Controller::undo() {
    if (undoStack.empty()) return;

    auto cmd = std::move(undoStack.top());
    undoStack.pop();
    cmd->undo();
    redoStack.push(std::move(cmd));
}

void Controller::redo() {
    if (redoStack.empty()) return;

    auto cmd = std::move(redoStack.top());
    redoStack.pop();
    cmd->execute();
    undoStack.push(std::move(cmd));
}

void Controller::reset() {
    // Remove all meals
    auto meals = getAllMeals();
    for (const auto& meal : meals) {
        repository->removeMeal(meal.getId());
    }
    // Reset nextId
    nextId = 1;
    // Clear undo/redo stacks
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
}

void Controller::clearAllMealsWithUndo() {
    auto meals = getAllMeals();
    auto cmd = std::make_unique<ClearCommand>(repository, meals);
    cmd->execute();
    undoStack.push(std::move(cmd));
    redoStack = {};
    nextId = 1;
}
