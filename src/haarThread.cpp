#include "haarThread.h"

void haarThread::setup(haarType type, int width, int height) {
    haarFound = false;
    imageSet = false;
    finder = new ofxCvHaarFinder();
    switch (type) {
    case eyes: finder->setup("eyesSmall.xml"); break;
    case mouth: finder->setup("mouth.xml"); break;
    default: break;
    }
    this->type = type;
	grayImage.allocate(width / SCALE_FACTOR, height / SCALE_FACTOR);
}

void haarThread::threadedFunction() {

    while (isThreadRunning()) {
        lock();
        if (!haarFound && imageSet) {
            switch (type) {
            case eyes: case mouth:
                finder->findHaarObjects(grayImage);
                haarFound = true;
                imageSet = false;
                break;
            default: break;
            }
        }
        unlock();
    }
}

ofxCvHaarFinder* haarThread::getHaar(ofxCvColorImage *image) {
    ofxCvHaarFinder *tmp;

    if (!imageSet) {
        lock();
        setImage(image);
        unlock();
    }

    if (haarFound) {
        lock();
        haarFound = false;
        tmp = new ofxCvHaarFinder();
        memcpy(tmp, finder, sizeof(ofxCvHaarFinder));
        unlock();
        return tmp;
    } else {
        return NULL;
    }
}

void haarThread::setImage(ofxCvColorImage *colorImage) {
    ofxCvGrayscaleImage tmpImage;
    tmpImage.allocate(colorImage->getWidth(), colorImage->getHeight());
    tmpImage = *colorImage;
    tmpImage.resize(colorImage->getWidth() / SCALE_FACTOR, colorImage->getHeight() / SCALE_FACTOR);
    grayImage = tmpImage;
    imageSet = true;
}
