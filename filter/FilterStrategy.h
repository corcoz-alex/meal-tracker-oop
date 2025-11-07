#pragma once
#include "../domain/meal.h"

class FilterStrategy {
public:
    virtual ~FilterStrategy() = default;
    virtual bool matches(const Meal& meal) const = 0;
};
