#include "matrixFilter.h"
#pragma once

class EdgeDetection : public MatrixFilter {

public:
    ~EdgeDetection() override = default;
    const std::vector<int8_t>& GetMatrix() const override;
    void CorrectColors(Bmp& image) const;
    void ApplyFilter(Bmp& image, std::vector<double> args) override;

private:
    double threshold_;
    const std::vector<int8_t> matrix_ = {0, -1, 0, -1, 4, -1, 0, -1, 0};
};