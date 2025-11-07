#include "updatecommand.h"

UpdateCommand::UpdateCommand(shared_ptr<IRepository> repo, const Meal& oldMeal, const Meal& newMeal) :
    repo(repo), oldMeal(oldMeal), newMeal(newMeal) {}

void UpdateCommand::execute() {
    repo->updateMeal(newMeal);
}

void UpdateCommand::undo() {
    repo->updateMeal(oldMeal);
}
