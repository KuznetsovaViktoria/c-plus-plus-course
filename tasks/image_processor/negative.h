#include "filter.h"
#pragma once

class Negative : public Filter {

public:
    void ApplyFilter(Bmp& image, std::vector<double> args) override;
    ~Negative() override = default;
};