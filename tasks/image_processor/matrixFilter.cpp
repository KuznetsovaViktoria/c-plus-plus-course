#include "matrixFilter.h"

void MatrixFilter::ApplyFilter(Bmp& image, std::vector<double> args) {
    std::vector<Color> new_colors(image.GetWidth() * image.GetHeight());
    for (size_t y = 0; y < image.GetHeight(); ++y) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            long double r = 0;
            long double g = 0;
            long double b = 0;
            for (size_t i = 0; i < delta_.size(); ++i) {
                if (0 <= x + delta_[i].first && x + delta_[i].first < image.GetWidth() && 0 <= y + delta_[i].second &&
                    y + delta_[i].second < image.GetHeight()) {
                    r += GetCoef(i) * image.GetColor(x + delta_[i].first, y + delta_[i].second).r;
                    g += GetCoef(i) * image.GetColor(x + delta_[i].first, y + delta_[i].second).g;
                    b += GetCoef(i) * image.GetColor(x + delta_[i].first, y + delta_[i].second).b;
                } else {
                    r += GetCoef(i) * image.GetColor(x, y).r;
                    g += GetCoef(i) * image.GetColor(x, y).g;
                    b += GetCoef(i) * image.GetColor(x, y).b;
                }
            }
            new_colors[y * image.GetWidth() + x] = Color(r, g, b);
            NormalizeColor(new_colors[y * image.GetWidth() + x]);
        }
    }
    image.ChangePrivateVectorOfColors(new_colors);
}

int8_t MatrixFilter::GetCoef(size_t i) {
    return 0;
}