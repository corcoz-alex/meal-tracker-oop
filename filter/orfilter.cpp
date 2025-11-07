#include "orfilter.h"

OrFilter::OrFilter(std::vector<std::shared_ptr<FilterStrategy>> filters)
    : filters(std::move(filters)) {}

bool OrFilter::matches(const Meal& meal) const {
    for (const auto& filter : filters)
        if (filter->matches(meal))
            return true;
    return false;
}
