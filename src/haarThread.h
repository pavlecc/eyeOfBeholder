#pragma once

#include "ofxCvHaarFinder.h"

#define SCALE_FACTOR 4

enum haarType { eyes, mouth };

class haarThread : public ofThread {

    public:
        void setup(haarType type, int width, int height);
        void threadedFunction();
        ofxCvHaarFinder* getHaar(ofxCvColorImage *image);

    private:
        void setImage(ofxCvColorImage *colorImage);

        ofxCvHaarFinder *finder;
        haarType type;
        bool haarFound;
        bool imageSet;
        ofxCvGrayscaleImage grayImage;
};
