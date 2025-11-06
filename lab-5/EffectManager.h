#pragma once

#include "ImageEffect.h"
#include <memory>
#include <vector>

class EffectManager {
private:
    std::vector<std::pair<char, std::unique_ptr<ImageEffect>>> _effects;
public:
    EffectManager();

    void addEffect(char key, std::unique_ptr<ImageEffect> effect);

    void applyEffects(Image& image);

    void switchEffect(char key);
};
