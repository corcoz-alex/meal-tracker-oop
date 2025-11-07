#include "addcommand.h"

AddCommand::AddCommand(shared_ptr<IRepository> repo, const Meal& meal) :
    repo(repo), meal(meal) {}

void AddCommand::execute() {
    repo->addMeal(meal);
}

void AddCommand::undo() {
    repo->removeMeal(meal.getId());
}
