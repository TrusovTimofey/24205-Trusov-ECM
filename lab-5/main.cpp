#include "EffectManager.h"
#include <iostream>

using namespace::cv;
using namespace::std;

int main() {
    VideoCapture camera(0);

    if (!camera.isOpened()) {
        cout << "Can`t open camera" << endl;
        return 1;
    }
    Mat frame;

    EffectManager effects;

    while (true) {
        camera >> frame;
        if (frame.empty()) {
            cout << "End of video" << endl;
            break;
        }

        Image img(frame);
        effects.applyEffects(img);

        imshow("Camera", frame);

        int key = waitKey(30);
        if (key){
            if(key == 27) break;
            effects.switchEffect(key);
        }

    }
    return 0;
}