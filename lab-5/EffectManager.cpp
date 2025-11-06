#include "EffectManager.h"

EffectManager::EffectManager() {
    addEffect('r',std::make_unique<RedFilter>());
    addEffect('g',std::make_unique<GreenFilter>());
    addEffect('b',std::make_unique<BlueFilter>());
    addEffect('l',std::make_unique<Brightness>());
    addEffect('f',std::make_unique<Flip>());
    addEffect('m',std::make_unique<Mirror>());
    addEffect('c',std::make_unique<ChromaticAberration>());
    addEffect('p',std::make_unique<Pause>());
    addEffect('t',std::make_unique<Trail>());
}

void EffectManager::addEffect(char key, std::unique_ptr<ImageEffect> effect) {
    _effects.push_back({key,move(effect)});
}

void EffectManager::applyEffects(Image &image) {
    for(int i = 0; i < _effects.size(); ++i){
        _effects[i].second->apply(image);
    }
}

void EffectManager::switchEffect(char key) {
    for(int i = 0; i < _effects.size(); ++i){
        if(_effects[i].first == key){
            _effects[i].second->switchActive();
            break;
        }
    }
}
