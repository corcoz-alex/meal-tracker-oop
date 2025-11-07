#include "removecommand.h"

RemoveCommand::RemoveCommand(shared_ptr<IRepository> repo, const Meal& meal) :
    repo(repo), meal(meal) {}

void RemoveCommand::execute() {
    repo->removeMeal(meal.getId());
}

void RemoveCommand::undo() {
    repo->addMeal(meal);
}
