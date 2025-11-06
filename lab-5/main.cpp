#include "EffectManager.h"
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace::cv;
using namespace::std;

void displayFPS(Image& img, float fps){
        int lineType = 0;
        int fontType = 0;
        float textScale = 0.9;
        Scalar color(0,255,0);
        int textThickness = 2;
        Point pivot ={0,img.height()-10};

        putText(img.matrix(), "FPS: "s + to_string(fps), pivot, fontType, textScale, 0, textThickness*4, lineType);
        putText(img.matrix(), "FPS: "s + to_string(fps), pivot, fontType, textScale, color, textThickness, lineType);

}

int main() {
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        cout << "Can`t open camera" << endl;
        return 1;
    }

    EffectManager effects;

    Mat frame;
    Image img(frame);

    int key;

    auto prevTicks = getTickCount();
    auto ticks = prevTicks;

    while (true) {
        camera >> frame;
        if (frame.empty()) {
            cout << "End of video" << endl;
            break;
        }

        effects.applyEffects(img);

        ticks = (getTickCount() - prevTicks); //End of timer
        displayFPS(img, getTickFrequency()/ticks);
        prevTicks = getTickCount(); //Start of timer

        imshow("Camera", frame);

        key = waitKey(10);
        if (key){
            if(key == 27) break;
            effects.switchEffect(key);
        }

    }
    return 0;
}