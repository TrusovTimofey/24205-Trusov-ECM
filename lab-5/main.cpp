#include "EffectManager.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <string>

using namespace::cv;
using namespace::std;

void displayText(Image& img, const string& txt, Point pivot){
        int lineType = 0;
        int fontType = 0;
        float textScale = 0.7;
        Scalar color(0,255,0);
        int textThickness = 2;

        putText(img.matrix(), txt, pivot, fontType, textScale, 0, textThickness*4, lineType);
        putText(img.matrix(), txt, pivot, fontType, textScale, color, textThickness, lineType);

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

    size_t frameTicks = 0;
    size_t prevFrameTicks = getTickCount();
    size_t inputTicks = 0;
    size_t outputTicks = getTickCount();
    size_t effectsTicks = 0;

    while (true) {

        inputTicks = getTickCount();
        camera >> frame;
        if (frame.empty()) {
            cout << "End of video" << endl;
            break;
        }
        inputTicks = getTickCount() - inputTicks;

        effectsTicks = getTickCount();
        effects.applyEffects(img);
        effectsTicks = getTickCount() - effectsTicks;

        frameTicks = getTickCount() - prevFrameTicks;
        prevFrameTicks = getTickCount();
        displayText(img, "FPS: "s + to_string(getTickFrequency()/frameTicks), Point(0,img.height()-10));
        displayText(img, "Input: "s + to_string(((double)inputTicks)/frameTicks*100)+"%", Point(0,img.height()-40));
        displayText(img, "Output: "s + to_string(((double)outputTicks)/frameTicks*100)+"%", Point(0,img.height()-70));
        displayText(img, "Effects: "s + to_string(((double)effectsTicks)/frameTicks*100)+"%", Point(0,img.height()-100));

        outputTicks = getTickCount();
        imshow("Camera", frame);
        outputTicks = getTickCount() - outputTicks;

        key = waitKey(10);
        if (key){
            if(key == 27) break;
            effects.switchEffect(key);
        }

    }
    return 0;
}