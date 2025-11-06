#include "ImageEffect.h"

void ImageEffect::switchActive() {
    _isActive = !_isActive;
}

bool ImageEffect::isActive() const {
    return _isActive;
}

void Brightness::apply(Image &image) {
    for (int i = 0; i  <  image.size(); i++) {
        int red = (int)(image.at(i).red() * _bright);
        int green = (int)(image.at(i).green() * _bright);
        int blue = (int)(image.at(i).blue() * _bright);
        image.at(i).red() = red > 255 ? 255 : red;
        image.at(i).green() = green > 255 ? 255 : green;
        image.at(i).blue() = blue > 255 ? 255 : blue;
    }
}

void Brightness::switchActive() {
    _bright+=_step;
    if(_bright>_max)_bright=_min;
}

void RedFilter::apply(Image &image) {
    if(!isActive()) return;
    for (int i = 0; i  <  image.size(); i++) {
        image.at(i).red() = 0;
    }
}

void GreenFilter::apply(Image &image) {
    if(!isActive()) return;
    for (int i = 0; i  <  image.size(); i++) {
        image.at(i).green() = 0;
    }
}

void BlueFilter::apply(Image &image) {
    if(!isActive()) return;
    for (int i = 0; i  <  image.size(); i++) {
        image.at(i).blue() = 0;
    }
}

void Flip::apply(Image &image) {
    if(!isActive()) return;
    for (int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width()/2;++x){
                int r= image.at(x,y).red();
                int g= image.at(x,y).green();
                int b= image.at(x,y).blue();
                image.at(x,y) = image.at(image.width()-x-1,y);
                image.at(image.width()-x-1,y).red() =r;
                image.at(image.width()-x-1,y).green() =g;
                image.at(image.width()-x-1,y).blue() =b;
            }
        }
}

void Mirror::apply(Image &image) {
    if(!isActive()) return;
    for (int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width()/2;++x){
            image.at(x,y) = image.at(image.width()-x-1,y);
        }
    }
}

void ChromaticAberration::apply(Image &image) {
    if(!isActive()) return;
    for (int y = 0; y < image.height(); ++y) {
        for(int x = _strength; x < image.width() - _strength;++x){
            image.at(x - _strength, y).red() = image.at(x,y).red();
            image.at(image.width() - 1 - x ,y).blue() = image.at(image.width() - 1 - x - _strength,y).blue();
        }
    }
}

void Pause::apply(Image &image) {
    if(!isActive()) return;
    if(!_havePrev){
        _havePrev = true;
        _prev = image.matrix().clone();
        return;
    }
    image.matrix() = _prev.clone();
}

void Pause::switchActive() {
    ImageEffect::switchActive();
    _havePrev = false;
}

void Trail::apply(Image &image) {
    if(_mode == Mode::OFF) return;
    if(!_havePrev){
        _havePrev = true;
        _prev = image.matrix().clone();
        return;
    }
    Image prev(_prev);
    for (int i = 0; i < image.size(); ++i) {
        image.at(i).red() = (uchar)((image.at(i).red() + prev.at(i).red()*_strength)/(1+_strength));
        image.at(i).green() = (uchar)((image.at(i).green() + prev.at(i).green()*_strength)/(1+_strength));
        image.at(i).blue() = (uchar)((image.at(i).blue() + prev.at(i).blue()*_strength)/(1+_strength));
    }
    _prev = image.matrix().clone();
}

void Trail::switchActive() {
    _mode = (Mode)((int)_mode + 1);
    switch (_mode) {
        case Mode::DENOISE: _strength = _denoiseStength;
            break;
        case Mode::LOW:  _strength = _lowStength;
            break;
        case Mode::MEDIUM:  _strength = _mediumStength;
            break;
        case Mode::HIGH:  _strength = _highStength;
            break;
        case Mode::EXTRA:  _strength = _extraStength;
            break;
        default: _mode = Mode::OFF;
            _havePrev = false;
    }
}

void Gradient::apply(Image &image) {
    if(!isActive()) return;
    for(int x = 0; x < image.width();++x){
        float normalizedWidth = (float )x / image.width();
        for (int y = 0; y < image.height(); ++y) {
            image.at(x,y).red()=(uchar)(normalizedWidth * image.at(x,y).red());
            image.at(x,y).blue()=(uchar)((1-normalizedWidth) * image.at(x,y).blue());
        }
    }
}

void BlackWhite::apply(Image &image) {
    if(!isActive()) return;
    for(int i = 0; i < image.size();++i){
        int light = image.at(i).red() + image.at(i).blue() + image.at(i).green();
        light /= 3;
        image.at(i).red() = light;
        image.at(i).green() = light;
        image.at(i).blue() = light;
    }
}

void SwapColors::apply(Image &image) {
    if(!isActive()) return;
    for(int i = 0; i < image.size();++i){
        ushort red = image.at(i).red();
        image.at(i).red() = image.at(i).green();
        image.at(i).green() = image.at(i).blue();
        image.at(i).blue() = red;
    }
}

void Negative::apply(Image &image) {
    if(!isActive()) return;
    for(int i = 0; i < image.size();++i){
        image.at(i).red() = 255 - image.at(i).red();
        image.at(i).green() = 255 -image.at(i).green();
        image.at(i).blue() = 255 - image.at(i).blue();
    }
}
