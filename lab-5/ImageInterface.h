#pragma  once

#include <opencv2/highgui.hpp>

class Pixel{
private:
    uchar* _p;
public:
    Pixel(uchar* p);

    uchar& red();
    uchar& green();
    uchar& blue();
    uchar red() const;
    uchar green() const;
    uchar blue() const;

    Pixel& operator=(Pixel other);

};

class Image{
private:
    cv::Mat& _img;
public:
    Image(cv::Mat& img);

    cv::Mat& matrix();

    int size();
    int width();
    int height();

    Pixel at(int i);
    Pixel at(int x, int y);
};
