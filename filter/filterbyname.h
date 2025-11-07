#pragma once
#include "filterstrategy.h"
#include <string>

class FilterByName : public FilterStrategy {
private:
    std::string targetNameLower;

public:
    explicit FilterByName(const std::string& name);
    bool matches(const Meal& meal) const override;
};
