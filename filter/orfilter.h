#pragma once
#include "filterstrategy.h"
#include <vector>
#include <memory>

class OrFilter : public FilterStrategy {
private:
    std::vector<std::shared_ptr<FilterStrategy>> filters;

public:
    explicit OrFilter(std::vector<std::shared_ptr<FilterStrategy>> filters);
    bool matches(const Meal& meal) const override;
};
