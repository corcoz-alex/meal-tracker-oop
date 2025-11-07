#include "filterbycaloriesrange.h"

FilterByCaloriesRange::FilterByCaloriesRange(int minCal, int maxCal)
    : minCal(minCal), maxCal(maxCal) {}

bool FilterByCaloriesRange::matches(const Meal& meal) const {
    int c = meal.getCalories();
    return c >= minCal && c <= maxCal;
}
