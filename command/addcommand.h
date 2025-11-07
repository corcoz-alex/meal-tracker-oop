#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H
#include "command.h"
#include "../domain/Meal.h"
#include "../repository/IRepository.h"

using namespace std;

class AddCommand : public Command
{
private:
    shared_ptr<IRepository> repo;
    Meal meal;

public:
    AddCommand(shared_ptr<IRepository> repo, const Meal& meal);
    void execute() override;
    void undo() override;
};

#endif // ADDCOMMAND_H
