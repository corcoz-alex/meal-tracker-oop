#ifndef REMOVECOMMAND_H
#define REMOVECOMMAND_H
#include "command.h"
#include "../domain/Meal.h"
#include "../repository/IRepository.h"

using namespace std;

class RemoveCommand : public Command
{
private:
    shared_ptr<IRepository> repo;
    Meal meal;
public:
    RemoveCommand(shared_ptr<IRepository> repo, const Meal& meal);
    void execute() override;
    void undo() override;
};

#endif // REMOVECOMMAND_H
