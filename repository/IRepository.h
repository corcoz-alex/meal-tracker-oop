#ifndef IREPOSITORY_H
#define IREPOSITORY_H
#include "../domain/Meal.h"
#include <vector>

using std::vector;

class IRepository
{
public:
    virtual ~IRepository() = default;
    virtual void addMeal(const Meal& meal) = 0;
    virtual void removeMeal(int id) = 0;
    virtual void updateMeal(const Meal& meal) = 0;
    virtual vector<Meal> getAllMeals() const = 0;
    virtual Meal getMealById(int id) const = 0;

    // Load and save for CSV and JSON storage
    virtual void load() = 0;
    virtual void save() const = 0;

};

#endif // IREPOSITORY_H
