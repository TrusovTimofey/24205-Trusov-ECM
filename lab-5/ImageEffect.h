#pragma once
#include "ImageInterface.h"

class ImageEffect {
private:
    bool _isActive = false;

protected:

    bool isActive() const;

public:
    virtual void apply(Image& image) = 0;
    virtual void switchActive();
};

class Brightness: public  ImageEffect{
private:
    float _bright = 1;
    float _step = 0.1;
    float _max = 2;
    float _min = 0.1f;
public:
    void apply(Image& image) override;
    void switchActive() override;
};

class RedFilter : public  ImageEffect{
public:
    void apply(Image& image) override;
};

class GreenFilter : public  ImageEffect{
public:
    void apply(Image& image) override;
};

class BlueFilter : public  ImageEffect{
public:
    void apply(Image& image) override;
};

class Flip : public ImageEffect{
public:
    void apply(Image& image) override;
};

class Mirror : public ImageEffect{
public:
    void apply(Image& image) override;
};

class ChromaticAberration : public ImageEffect{
private:
    int _strength = 5;
public:
    void apply(Image& image) override;
};

class Pause : public ImageEffect{
private:
    cv::Mat _prev;
    bool _havePrev = false;
public:
    void apply(Image& image) override;
    void switchActive() override;
};

class Trail : public ImageEffect{
private:
    float _strength = 0;

    float _denoiseStength = 0.6f;
    float _lowStength = 1;
    float _mediumStength = 3;
    float _highStength = 10;
    float _extraStength = 50;

    enum class Mode{
        OFF = 0,
        DENOISE = 1,
        LOW = 2,
        MEDIUM,
        HIGH,
        EXTRA,
    };


    Mode _mode = Mode::OFF;

    cv::Mat _prev;
    bool _havePrev = false;
public:
    void apply(Image& image) override;
    void switchActive() override;
};