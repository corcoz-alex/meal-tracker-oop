#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../repository/IRepository.h"
#include "../domain/Meal.h"
#include "../filter/FilterStrategy.h"
#include "../command/command.h"
#include <stack>

using namespace std;

class Controller
{
private:
    shared_ptr<IRepository> repository;
    int nextId = 1;

    void syncNextId();

    stack<unique_ptr<Command>> undoStack;
    stack<unique_ptr<Command>> redoStack;

public:
    Controller(unique_ptr<IRepository> repo);
    void addMeal(const string& name, int calories, const string& type, const QDateTime& dateTime);
    void removeMeal(int id);
    void updateMeal(int id, const string& name, int calories, const string& type, const QDateTime& dateTime);
    std::vector<Meal> getAllMeals() const;
    Meal getMealById(int id) const;

    void load();
    void save();

    //Filters
    vector<Meal> filterMeals(const FilterStrategy& strategy) const;

    //Undo + Redo
    void undo();
    void redo();

    void reset();
    void clearAllMealsWithUndo();
};

#endif // CONTROLLER_H
