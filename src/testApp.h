#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "haarThread.h"
#include "ofxFensterManager.h"

#define FRAME_MAX_DELAY   10

class haarFinder {
public:
    ofxCvHaarFinder *eyes;
    ofxCvHaarFinder *mouth;
};

enum haarFinderAppType { video, image };

class haarFinderApp : public ofxFenster{

	public:
	    haarFinderApp(haarFinderAppType type, int width, int height);
		void setup();
		void exit();
		void update();
		void draw();

		void keyPressed(int key);

    private:
        haarFinderAppType appType;
        int width;
        int height;

        haarThread threadEyes;
        haarThread threadMouth;
        ofVideoGrabber vidGrabber;
        ofxCvColorImage *colorImages[FRAME_MAX_DELAY];
        haarFinder finder;
};

class testApp : public ofBaseApp {
    public:
        void setup();
		void exit();
		void update();
		void draw();

		haarFinderApp *app_1;
		haarFinderApp *app_2;
};
