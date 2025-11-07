#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H
#include "command.h"
#include "../domain/Meal.h"
#include "../repository/IRepository.h"

using namespace std;

class UpdateCommand : public Command
{
private:
    shared_ptr<IRepository> repo;
    Meal oldMeal;
    Meal newMeal;

public:
    UpdateCommand(shared_ptr<IRepository> repo, const Meal& oldMeal, const Meal& newMeal);
    void execute() override;
    void undo() override;
};

#endif // UPDATECOMMAND_H
