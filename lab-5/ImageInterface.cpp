#include "ImageInterface.h"

Pixel::Pixel(uchar *p) : _p(p){}

uchar &Pixel::red() {
    return _p[2];
}

uchar &Pixel::green() {
    return _p[1];
}

uchar &Pixel::blue() {
    return _p[0];
}

uchar Pixel::red() const {
    return _p[2];
}

uchar Pixel::green() const {
    return _p[1];
}

uchar Pixel::blue() const {
    return _p[0];
}

Pixel &Pixel::operator=(Pixel other) {
    red() = other.red();
    green() = other.green();
    blue() = other.blue();

    return *this;
}

Image::Image(cv::Mat &img) : _img(img) {}

cv::Mat &Image::matrix() {
    return _img;
}

int Image::size() {
    return _img.size().width * _img.size().height;
}

int Image::width() {
    return _img.size().width;
}

int Image::height() {
    return _img.size().height;
}

Pixel Image::at(int i) {
    return Pixel(&_img.data[i*_img.elemSize()]);
}

Pixel Image::at(int x, int y) {
    return Pixel(&_img.data[(x + y*width())  *_img.elemSize()]);
}
