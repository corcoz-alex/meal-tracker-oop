#pragma once
#include "filterstrategy.h"

class FilterByCaloriesRange : public FilterStrategy {
private:
    int minCal;
    int maxCal;

public:
    FilterByCaloriesRange(int minCal, int maxCal);
    bool matches(const Meal& meal) const override;
};
