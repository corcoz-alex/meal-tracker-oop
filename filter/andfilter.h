#pragma once
#include "filterstrategy.h"
#include <vector>
#include <memory>

class AndFilter : public FilterStrategy {
private:
    std::vector<std::shared_ptr<FilterStrategy>> filters;

public:
    explicit AndFilter(std::vector<std::shared_ptr<FilterStrategy>> filters);
    bool matches(const Meal& meal) const override;
};
