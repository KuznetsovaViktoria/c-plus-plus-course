#include "bmp.h"
#pragma once
#include <iostream>
#include <fstream>

Color::Color(long double r, long double g, long double b) : r(r), g(g), b(b){}

//void Color::operator*(const size_t x) {
//    r*=x;
//    g*=x;
//    b*=x
//};

void Bmp::Read(const char *path) {
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        std::cout << "This file couldn't be read\n";
        return;
    }

    unsigned char file_header[bmp_header_size_];
    f.read(reinterpret_cast<char *>(file_header), bmp_header_size_);

    if (file_header[0] != 'B' || file_header[1] != 'M') {
        std::cout << "The file is not a .bmp file\n";
        f.close();
        return;
    }

    unsigned char information_header[dib_header_size_];
    f.read(reinterpret_cast<char *>(information_header), dib_header_size_);

    file_size_ = file_header[2] + (file_header[3] << 8) + (file_header[4] << 16) + (file_header[5] << 24);

    width_ = information_header[4] + (information_header[5] << 8) + (information_header[6] << 16) +
             (information_header[7] << 24);
    //    std::cout<<"width: " <<width_<<'\n';
    height_ = information_header[8] + (information_header[9] << 8) + (information_header[10] << 16) +
              (information_header[11] << 24);
    //    std::cout<<"height: " <<height_<<'\n';
    colors_.resize(width_ * height_);

    padding_amount_ = ((4 - (width_ * 3) % 4) % 4);

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            unsigned char color[3];
            f.read(reinterpret_cast<char *>(color), 3);

            colors_[y * width_ + x].r = static_cast<long double>(color[0]) / rgb_;
            colors_[y * width_ + x].g = static_cast<long double>(color[1]) / rgb_;
            colors_[y * width_ + x].b = static_cast<long double>(color[2]) / rgb_;
        }
        f.ignore(padding_amount_);
    }
    f.close();
};

void Bmp::Export(const char *path) {
    std::ofstream f(path, std::ios::binary);
    // f.open(path, std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        std::cout << "This file couldn't be opened\n";
        return;
    }
    unsigned char bmp_pad[3] = {0, 0, 0};
    unsigned char bmp_header[14] = {0};
    bmp_header[0] = 'B';
    bmp_header[1] = 'M';
    bmp_header[2] = file_size_;
    bmp_header[3] = file_size_ >> 8;
    bmp_header[4] = file_size_ >> 16;
    bmp_header[5] = file_size_ >> 24;
    bmp_header[10] = bmp_header_size_ + dib_header_size_;

    unsigned char dib_header[40] = {0};
    dib_header[0] = dib_header_size_;
    dib_header[4] = width_;
    dib_header[5] = width_ >> 8;
    dib_header[6] = width_ >> 16;
    dib_header[7] = width_ >> 24;
    dib_header[8] = height_;
    dib_header[9] = height_ >> 8;
    dib_header[10] = height_ >> 16;
    dib_header[11] = height_ >> 24;
    dib_header[12] = 1;
    dib_header[14] = 24;

    f.write(reinterpret_cast<char *>(bmp_header), bmp_header_size_);
    f.write(reinterpret_cast<char *>(dib_header), dib_header_size_);

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            unsigned char color[3];
            color[0] = static_cast<unsigned char>(GetColor(x, y).r * rgb_);
            color[1] = static_cast<unsigned char>(GetColor(x, y).g * rgb_);
            color[2] = static_cast<unsigned char>(GetColor(x, y).b * rgb_);

            f.write(reinterpret_cast<char *>(color), 3);
        }
        f.write(reinterpret_cast<char *>(bmp_pad), padding_amount_);
    }
    f.close();
    std::cout << "file created!\n";
}

Color Bmp::GetColor(int x, int y) const {
    return colors_[y * width_ + x];
}

void Bmp::Crop(int new_width, int new_height) {
    std::vector<Color> new_colors;
    new_colors.resize(new_width * new_height);

    padding_amount_ = ((4 - (new_width * 3) % 4) % 4);

    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            new_colors[y * new_width + x] = GetColor(x, y);
        }
    }
    width_ = new_width;
    height_ = new_height;
    colors_ = new_colors;
    file_size_ = bmp_header_size_ + dib_header_size_ + width_ * height_ * 3 + padding_amount_ * height_;
}

int Bmp::GetHeight() const {
    return height_;
}

int Bmp::GetWidth() const {
    return width_;
}

Color &Bmp::operator[](size_t i) {
    return colors_[i];
};

void Bmp::ChangePrivateVectorOfColors(std::vector<Color> new_colors) {
    colors_ = new_colors;
}