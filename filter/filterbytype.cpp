#include "filterbytype.h"

FilterByType::FilterByType(const std::string& type) : type(type) {}

bool FilterByType::matches(const Meal& meal) const {
    return meal.getType() == type;
}
