#pragma once

#include "ofMain.h"

#include "ofxLeapMotion2.h"

#define NUM_sakura 60
#define NUM_icho 20

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void draw_ninja(int x, int y, int big);
    void draw_icho(int x, int y, int big, ofVec2f m, float t);
    void draw_sakura(int x, int y, int big, ofVec2f m, float t);
    void draw_box1(ofPoint point, float h, float w, float d);
    void draw_box2(ofPoint point, float h, float w, float d);
    void draw_line(ofPoint p_1, ofPoint p_2);
    void draw_gorin();
    
    void exit();
    
    ofxLeapMotion leap;
    vector <ofxLeapMotionSimpleHand> simpleHands;
    ofEasyCam cam;
    ofTrueTypeFont font;
    
    vector <ofVec3f> fingerPos;
    vector <ofVec3f> handPos;
    
    ofVec2f location_ninja;
    ofVec2f move_ninja;
    ofVec2f speed_ninja;
    
    ofVec2f location_sakura[NUM_sakura];
    ofVec2f move_sakura[NUM_sakura];
    float theta_sakura[NUM_sakura];
    
    ofVec2f location_icho[NUM_icho];
    ofVec2f move_icho[NUM_icho];
    float theta_icho[NUM_icho];
    
    ofSoundPlayer mySound;
    float *volume;
    float *fftSmoothed;
    int nBandsToGet;
    
    ofVec2f mouse;
    float size;
//    float r;
    
    float distance;
    int button;
};

