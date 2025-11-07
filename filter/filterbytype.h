#pragma once
#include "filterstrategy.h"
#include <string>

class FilterByType : public FilterStrategy {
private:
    std::string type;

public:
    explicit FilterByType(const std::string& type);
    bool matches(const Meal& meal) const override;
};
