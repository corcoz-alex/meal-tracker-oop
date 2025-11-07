#include "andfilter.h"

AndFilter::AndFilter(std::vector<std::shared_ptr<FilterStrategy>> filters)
    : filters(std::move(filters)) {}

bool AndFilter::matches(const Meal& meal) const {
    for (const auto& filter : filters)
        if (!filter->matches(meal))
            return false;
    return true;
}
