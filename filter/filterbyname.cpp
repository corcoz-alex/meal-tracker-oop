#include "filterbyname.h"
#include <algorithm>

FilterByName::FilterByName(const std::string& name) {
    targetNameLower = name;
    std::transform(targetNameLower.begin(), targetNameLower.end(), targetNameLower.begin(), ::tolower);
}

bool FilterByName::matches(const Meal& meal) const {
    std::string mealName = meal.getName();
    std::transform(mealName.begin(), mealName.end(), mealName.begin(), ::tolower);
    return mealName.find(targetNameLower) != std::string::npos;
}
