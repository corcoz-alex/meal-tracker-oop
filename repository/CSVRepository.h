#ifndef CSVREPOSITORY_H
#define CSVREPOSITORY_H
#include "IRepository.h"

using namespace std;

class CSVRepository : public IRepository
{
private:
    vector<Meal> meals;
    string filename;
public:
    CSVRepository(const string& filename);
    void addMeal(const Meal& meal) override;
    void removeMeal(int id) override;
    void updateMeal(const Meal& meal) override;
    vector<Meal> getAllMeals() const override;
    Meal getMealById(int id) const override;

    virtual void load() override;
    virtual void save() const override;
};

#endif // CSVREPOSITORY_H
