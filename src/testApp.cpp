#include "testApp.h"

void testApp::setup(){
    app_1 = new haarFinderApp(video, 640, 480);
    app_2 = new haarFinderApp(image, 640, 480);
    ofxFensterManager::get()->setupWindow(app_1, 10, 10, 640, 480);
    ofxFensterManager::get()->setupWindow(app_2, 650, 10, 640, 480);
}

void testApp::update(){
    app_1->update();
    app_2->update();
}

void testApp::draw(){
    app_1->draw();
    app_2->draw();
}

void testApp::exit(){
    app_1->exit();
    app_2->exit();
}

haarFinderApp::haarFinderApp(haarFinderAppType type, int width, int height) {
    this->appType = type;
    this->width = width;
    this->height = height;
}

//--------------------------------------------------------------
void haarFinderApp::setup(){
    switch (appType) {
    case video:
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(width, height);
        break;
    case image:
        break;
    }
    for (int i = 0; i < FRAME_MAX_DELAY; i++) {
        colorImages[i] = new ofxCvColorImage();
        colorImages[i]->allocate(width, height);
    }
    threadEyes.setup(eyes, width, height);
    //threadMouth.setup(mouth, width, height);
    threadEyes.startThread(true, false);
    //threadMouth.startThread(true, false);
}

void haarFinderApp::exit(){
    threadEyes.stopThread();
    //threadMouth.stopThread();
}

//--------------------------------------------------------------
void haarFinderApp::update(){
    bool bNewFrame = false;
    static int frameDelay[2] = {0, 0};
    int frameDelayMax;
    static bool frameDelaySet[2] = {true, false};
    haarFinder finderTmp;

    frameDelayMax = MAX(frameDelay[eyes], frameDelay[mouth]);

    switch (appType) {
    case video:
        vidGrabber.update();
        bNewFrame = vidGrabber.isFrameNew();
        break;
    case image:
        bNewFrame = true;
    }

	if (bNewFrame){
        colorImages[frameDelayMax]->allocate(width, height);
        switch (appType) {
        case video:
            colorImages[frameDelayMax]->setFromPixels(vidGrabber.getPixels(), width, height);
            break;
        case image:
            ofLoadImage(colorImages[frameDelayMax]->getPixelsRef(), "example2.jpg");
            break;
        }
        colorImages[frameDelayMax]->mirror(false, true);

        finderTmp.eyes = threadEyes.getHaar(colorImages[frameDelayMax]);
        finderTmp.mouth = threadMouth.getHaar(colorImages[frameDelayMax]);
        if (finderTmp.eyes) {
            finder.eyes = finderTmp.eyes;
            if (frameDelay[eyes]) {
                frameDelaySet[eyes] = true;
            }
        } else {
            if (!frameDelaySet[eyes] && frameDelay[eyes] < FRAME_MAX_DELAY - 1) {
                frameDelay[eyes]++;
            }
        }
        if (finderTmp.mouth) {
            finder.mouth = finderTmp.mouth;
            if (frameDelay[mouth]) {
                frameDelaySet[mouth] = true;
            }
        } else {
            if (!frameDelaySet[mouth]&& frameDelay[mouth] < FRAME_MAX_DELAY - 1) {
                frameDelay[mouth]++;
            }
        }

        for (int i = 0; i < frameDelayMax; i++) {
            colorImages[i] = colorImages[i + 1];
        }
	}
}

//--------------------------------------------------------------
void haarFinderApp::draw(){
    ofSetColor(255, 255, 255);
    ofSetBackgroundColor(0, 0, 0, 255);
	colorImages[0]->draw(0, 0);
	ofNoFill();
	if (finder.eyes) {
        ofSetColor(255, 0, 0);
        for(unsigned int i = 0; i < finder.eyes->blobs.size(); i++) {
            ofRectangle cur = finder.eyes->blobs[i].boundingRect;
            ofRect(cur.x * SCALE_FACTOR, cur.y * SCALE_FACTOR, cur.width * SCALE_FACTOR, cur.height * SCALE_FACTOR);
        }
	}
	if (finder.mouth) {
        ofSetColor(255, 255, 0);
        for(unsigned int i = 0; i < finder.mouth->blobs.size(); i++) {
            ofRectangle cur = finder.mouth->blobs[i].boundingRect;
            ofRect(cur.x * SCALE_FACTOR, cur.y * SCALE_FACTOR, cur.width * SCALE_FACTOR, cur.height * SCALE_FACTOR);
        }
	}
}

//--------------------------------------------------------------
void haarFinderApp::keyPressed(int key){
    switch (key){
		case ' ':
		    ofSaveImage(colorImages[0]->getPixelsRef(), "slika.png");
		    break;
        default:
            break;
    }
}
