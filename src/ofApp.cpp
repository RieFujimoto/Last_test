#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofToggleFullscreen();
    ofSetVerticalSync(true);
    ofSetCircleResolution(60);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    ofSetVerticalSync(true);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    //    edm
    mySound.load("sakura_edm.mp3");
    mySound.setLoop(true);
    mySound.play();
    fftSmoothed = new float [8192];
    for(int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    nBandsToGet = 360;
    
    //    侍
    move_ninja = ofVec2f(0, 0);
    speed_ninja = ofVec2f(2.4, 2.4);
    //    speed_ninja = ofVec2f(0, 0);
    
    //    イチョウ
    for(int i = 0; i < NUM_icho; i++){
        move_icho[i] = ofVec2f(ofRandom(-50, 50), ofRandom(0, 50));
    }
    for(int i = 0; i < NUM_icho; i++){
        theta_icho[i] = ofRandom(180);
    }
    
    //    桜
    for(int i = 0; i < NUM_sakura; i++){
        move_sakura[i] = ofVec2f(ofRandom(-30, 30), ofRandom(-10, 5));
    }
    for(int i = 0; i < NUM_sakura; i++){
        theta_sakura[i] = ofRandom(90);
    }
    
    //    文字
    ofTrueTypeFont::setGlobalDpi(72);
    //    font.load("Skia.ttf", 40);
    font.load("Phosphate.ttc", 30);
    font.setLetterSpacing(1.0);
    
    leap.open();
}

//--------------------------------------------------------------
void ofApp::update(){
    simpleHands = leap.getSimpleHands();
    if(leap.isFrameNew() && simpleHands.size()){
        
        fingerPos.clear();
        handPos.clear();
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            handPos.push_back(simpleHands[i].handPos);
            for(int f = 0; f < 5; f++){
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;
                fingerPos.push_back(mcp);
                
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;
                fingerPos.push_back(pip);
                
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;
                fingerPos.push_back(dip);
                
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;
                fingerPos.push_back(tip);
            }
        }
    }
    leap.markFrameAsOld();
    
    if(button == 2){
        ofSoundUpdate();
        volume = ofSoundGetSpectrum(nBandsToGet);
        //音量の平均を求める
        for(int i = 0; i < nBandsToGet; i++){
            fftSmoothed[i] *= 0.96f;
            if(fftSmoothed[i] < volume[i]){
                fftSmoothed[i]= volume[i];
            }
        }
    }
    
    if(button == 3){
        move_ninja.x += speed_ninja.x;
        move_ninja.y += speed_ninja.y;
        
        if(move_ninja.x > ofGetWidth()/2 || move_ninja.x < -ofGetWidth()/2){
            speed_ninja.x = (-1) * speed_ninja.x;
        }
        
        if(move_ninja.y > ofGetHeight()/2 || move_ninja.y < -ofGetHeight()/2){
            speed_ninja.y = (-1) * speed_ninja.y;
        }
    }
    
    if(button == 4){
        for(int i = 0; i < NUM_icho; i++){
            move_icho[i].x -= ofRandom(0.02);
            move_icho[i].y -= ofRandom(0.01);
        }
    }
    
    if(button == 5){
        for(int i = 0; i < NUM_sakura; i++){
            move_sakura[i].x -= ofRandom(0.01);
            move_sakura[i].y -= ofRandom(0.005);
        }
    }
}


////--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    //    Back.draw(0, 0, ofGetWidth(), ofGetHeight());
    cam.begin();
    
    for(int i = 0; i < simpleHands.size(); i++){
        ofSetColor(0, 255, 0);
        ofDrawSphere(handPos.at(i), 20);
        for(int f = 0; f < 5; f++){
            ofSetColor(255);
            ofDrawSphere(fingerPos.at(20*i+4*f+0), 10);
            
            ofSetColor(200);
            ofDrawSphere(fingerPos.at(20*i+4*f+1), 10);
            
            ofSetColor(155);
            ofDrawSphere(fingerPos.at(20*i+4*f+2), 10);
            
            ofSetColor(100);
            ofDrawSphere(fingerPos.at(20*i+4*f+3), 10);
            
            ofDrawLine(fingerPos.at(20*i+4*f+0), fingerPos.at(20*i+4*f+1));
            ofDrawLine(fingerPos.at(20*i+4*f+1), fingerPos.at(20*i+4*f+2));
            ofDrawLine(fingerPos.at(20*i+4*f+2), fingerPos.at(20*i+4*f+3));
        }
    }
    
    ofPushMatrix();
    ofTranslate(0, -30);
    draw_gorin();
    ofPopMatrix();
    
    //        スイッチ
    if(simpleHands.size() == 1){
        //        とりあえず五輪からの距離をとる
        float distance1 = sqrt(pow(handPos.at(0).x + 230, 2) + pow(handPos.at(0).y - 50, 2) + pow(handPos.at(0).z, 2));
        
        float distance2 = sqrt(pow(handPos.at(0).x, 2) + pow(handPos.at(0).y - 50, 2) + pow(handPos.at(0).z, 2));
        
        float distance3 = sqrt(pow(handPos.at(0).x - 230, 2) + pow(handPos.at(0).y - 50, 2) + pow(handPos.at(0).z, 2));
        
        float distance4 = sqrt(pow(handPos.at(0).x + 115, 2) + pow(handPos.at(0).y + 50, 2) + pow(handPos.at(0).z, 2));
        
        float distance5 = sqrt(pow(handPos.at(0).x - 115, 2) + pow(handPos.at(0).y + 50, 2) + pow(handPos.at(0).z, 2));
        
        if(distance1 < 100){
            button = 1;
        }
        if(distance2 < 100){
            button = 2;
        }
        if(distance3 < 100){
            button = 3;
        }
        if(distance4 < 100){
            button = 4;
        }
        if(distance5 < 100){
            button = 5;
        }
        
        //        ネオン街
        if(button == 1){
            int way_width = 600;
            int left = -way_width/2;
            int right = way_width/2;
            //            ofTranslate(0, 0);
            for(int i = -10; i < 10; i++){
                ofSetColor(ofColor::fromHsb(i * 10 % 255, 225, 225));
                draw_box1(ofPoint(left, -200, -200 * i), ofRandom(100, 400), ofRandom(100, 300), 200);
            }
            for(int i = -10; i < 10; i++){
                ofSetColor(ofColor::fromHsb(255 - i * 10 % 255, 225, 225));
                draw_box2(ofPoint(right, -200, -200 * i), ofRandom(100, 400), ofRandom(100, 300), 200);
            }
        }
        
        //        和風edm
        if(button == 2){
            int start_x = 200;
            int base_y = 500;
            float width = (float)(100) / nBandsToGet;
            ofTranslate(0, 20);
            
            for(int i = 0; i < nBandsToGet; i+=5){
                ofPushMatrix();
                ofSetColor(ofColor(ofColor::fromHsb((float)255/nBandsToGet*i, 100, 255)));
                float length = 300 * fftSmoothed[i%nBandsToGet];
                ofRotateZDeg(i);
                ofDrawRectangle(0, 110, 5, length);
                ofPopMatrix();
            }
        }
        
        //        忍者
        if(button == 3){
            draw_ninja(0, 0, 300);
        }
        
        //        イチョウ
        if(button == 4){
            ofSetColor(255, 220, 70, 220);
            for(int i = 0; i < NUM_icho; i++){
                draw_icho(0, 0, 80, move_icho[i], theta_icho[i]);
            }
        }
        
        //        桜
        if(button == 5){
            ofSetColor(240, 200, 220);
            for(int i = 0; i < NUM_sakura; i++){
                draw_sakura(0, 0, 30, move_sakura[i], theta_sakura[i]);
            }
        }
        
    }
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::draw_ninja(int x, int y, int big){
    location_ninja = ofVec2f(move_ninja.x, move_ninja.y);
    
    size = big;
    
    ofPushMatrix();
    ofTranslate(location_ninja);
    
    ofRotateDeg(ofGetFrameNum()*0.7, 0, 0, 1);
    ofPushMatrix();
    
    ofSetColor(0);
    ofDrawCircle(0, 0, 20);
    
    for(int i = 0; i < 4; i++){
        if(i == 0 || i == 3){
            ofSetColor(250, 250, 250, 230);
        }
        if(i == 1 || i == 2){
            ofSetColor(120, 180, 200, 230);
        }
        
        ofBeginShape();
        ofRotateZDeg(90*i);
        ofVertex(size*0.5, size*0.2);
        ofVertex(0, size*0.2);
        ofVertex(0, 0);
        ofVertex(size*0.2, 0);
        ofEndShape();
    }
    
    ofPopMatrix();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw_icho(int x, int y, int big, ofVec2f m, float t){
    
    for(int i = 0; i < NUM_icho; i++){
        location_icho[i] = ofVec2f(x + m.x, y + m.y);
    }
    
    size = big;
    
    ofPushMatrix();
    
    for(int i = 0; i < NUM_icho; i++){
        ofTranslate(location_icho[i]);
    }
    
    for(int i = 0; i < NUM_icho; i++){
        ofRotateDeg(ofGetElapsedTimef()*3 + t, 1, 1, 1);
    }
    
    ofPushMatrix();
    
    ofBeginShape();
    ofVertex(0, size*0.2);
    ofVertex(-size*0.05, size*0.45);
    ofVertex(-size*0.3, size*0.35);
    ofVertex(-size*0.45, size*0.25);
    ofVertex(-size*0.3, size*0.15);
    ofVertex(-size*0.17, 0);
    ofVertex(-size*0.05, -size*0.15);
    ofVertex(0, -size*0.3);
    ofVertex(size*0.05, -size*0.15);
    ofVertex(size*0.17, 0);
    ofVertex(size*0.3, size*0.15);
    ofVertex(size*0.45, size*0.25);
    ofVertex(size*0.3, size*0.35);
    ofVertex(size*0.05, size*0.45);
    ofEndShape();
    
    ofSetLineWidth(2);
    ofDrawLine(0, -size*0.2, 0, -size*0.5);
    
    ofPopMatrix();
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::draw_sakura(int x, int y, int big, ofVec2f m, float t){
    for(int i = 0; i < NUM_sakura; i++){
        location_sakura[i] = ofVec2f(x + m.x, y + m.y);
    }
    size = big;
    
    ofPushMatrix();
    
    for(int i = 0; i < NUM_sakura; i++){
        ofTranslate(location_sakura[i]);
    }
    
    //    float time = ofGetElapsedTimef()*5;
    for(int i = 0; i < NUM_sakura; i++){
        ofRotateDeg(ofGetElapsedTimef() * 3 + t, 1, 1, 1);
    }
    
    ofPushMatrix();
    
    ofBeginShape();
    ofVertex(size*0.4, 0, 0);
    ofVertex(size*0.37, size*0.25, 0);
    ofVertex(size*0.3, size*0.38, 0);
    ofVertex(size*0.2, size*0.5, 0);
    ofVertex(0, size*0.3, 0);
    ofVertex(-size*0.2, size*0.5, 0);
    ofVertex(-size*0.3, size*0.38, 0);
    ofVertex(-size*0.37, size*0.25, 0);
    ofVertex(-size*0.4, 0, 0);
    ofVertex(-size*0.37, -size*0.2, 0);
    ofVertex(-size*0.23, -size*0.4, 0);
    ofVertex(0, -size*0.6, 0);
    ofVertex(size*0.23, -size*0.4, 0);
    ofVertex(size*0.37, -size*0.2, 0);
    ofEndShape();
    
    ofPopMatrix();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw_box1(ofPoint point, float h, float w, float d) {
    
    for (int x = point.x - w; x < point.x; x += 1) {
        int y_1 = point.y;
        int y_2 = point.y + h;
        int z_1 = point.z - d;
        int z_2 = point.z;
        
        draw_line(ofPoint(x, y_1, z_1), ofPoint(x + 1, y_1, z_1));
        draw_line(ofPoint(x, y_1, z_2), ofPoint(x + 1, y_1, z_2));
        draw_line(ofPoint(x, y_2, z_1), ofPoint(x + 1, y_2, z_1));
        draw_line(ofPoint(x, y_2, z_2), ofPoint(x + 1, y_2, z_2));
    }
    
    for (int y = point.y; y < point.y + h; y += 1) {
        int x_1 = point.x - w;
        int x_2 = point.x;
        int z_1 = point.z - d;
        int z_2 = point.z;
        
        draw_line(ofPoint(x_1, y, z_1), ofPoint(x_1, y + 1, z_1));
        draw_line(ofPoint(x_1, y, z_2), ofPoint(x_1, y + 1, z_2));
        draw_line(ofPoint(x_2, y, z_1), ofPoint(x_2, y + 1, z_1));
        draw_line(ofPoint(x_2, y, z_2), ofPoint(x_2, y + 1, z_2));
    }
    
    for (int z = point.z - d; z < point.z; z += 1) {
        int x_1 = point.x - w;
        int x_2 = point.x;
        int y_1 = point.y;
        int y_2 = point.y + h;
        
        draw_line(ofPoint(x_1, y_1, z), ofPoint(x_1, y_1, z + 1));
        draw_line(ofPoint(x_1, y_2, z), ofPoint(x_1, y_2, z + 1));
        draw_line(ofPoint(x_2, y_1, z), ofPoint(x_2, y_1, z + 1));
        draw_line(ofPoint(x_2, y_2, z), ofPoint(x_2, y_2, z + 1));
    }
}

//--------------------------------------------------------------
void ofApp::draw_box2(ofPoint point, float h, float w, float d) {
    
    for (int x = point.x; x < point.x + w; x += 1) {
        int y_1 = point.y;
        int y_2 = point.y + h;
        int z_1 = point.z - d;
        int z_2 = point.z;
        
        draw_line(ofPoint(x, y_1, z_1), ofPoint(x + 1, y_1, z_1));
        draw_line(ofPoint(x, y_1, z_2), ofPoint(x + 1, y_1, z_2));
        draw_line(ofPoint(x, y_2, z_1), ofPoint(x + 1, y_2, z_1));
        draw_line(ofPoint(x, y_2, z_2), ofPoint(x + 1, y_2, z_2));
    }
    
    for (int y = point.y; y < point.y + h; y += 1) {
        int x_1 = point.x;
        int x_2 = point.x + w;
        int z_1 = point.z - d;
        int z_2 = point.z;
        
        draw_line(ofPoint(x_1, y, z_1), ofPoint(x_1, y + 1, z_1));
        draw_line(ofPoint(x_1, y, z_2), ofPoint(x_1, y + 1, z_2));
        draw_line(ofPoint(x_2, y, z_1), ofPoint(x_2, y + 1, z_1));
        draw_line(ofPoint(x_2, y, z_2), ofPoint(x_2, y + 1, z_2));
    }
    
    for (int z = point.z - d; z < point.z; z += 1) {
        int x_1 = point.x;
        int x_2 = point.x + w;
        int y_1 = point.y;
        int y_2 = point.y + h;
        
        draw_line(ofPoint(x_1, y_1, z), ofPoint(x_1, y_1, z + 1));
        draw_line(ofPoint(x_1, y_2, z), ofPoint(x_1, y_2, z + 1));
        draw_line(ofPoint(x_2, y_1, z), ofPoint(x_2, y_1, z + 1));
        draw_line(ofPoint(x_2, y_2, z), ofPoint(x_2, y_2, z + 1));
    }
}

//--------------------------------------------------------------
void ofApp::draw_gorin(){
    
    ofPushStyle();
    ofSetLineWidth(12);
    ofNoFill();
    
    //    青
    ofPushMatrix();
    ofSetColor(80, 150, 210);
    ofDrawCircle(-230, 50, 100);
    if(button == 1){
        ofDrawCircle(-230, 50, ofRandom(100, 120));
    }
    font.drawString("TOKYO", -305, 52);
    ofPopMatrix();
    
    //    黒
    ofSetLineWidth(1);
    ofSetColor(255, 255, 255);
    ofDrawCircle(0, 50, 100);
    ofSetColor(255, 255, 255);
    ofDrawCircle(0, 50, 110);
    ofSetColor(180, 180, 180);
    if(button == 2){
        ofSetLineWidth(12);
        ofDrawCircle(0, 50, ofRandom(100, 120));
    }
    font.drawString("NIPPON", -80, 52);
    
    //    赤
    ofSetLineWidth(12);
    ofSetColor(255, 0, 0);
    ofDrawCircle(230, 50, 100);
    if(button == 3){
        ofDrawCircle(230, 50, ofRandom(100, 120));
    }
    font.drawString("NINJA", 160, 52);
    
    //    黄
    ofSetColor(250, 230, 80);
    ofDrawCircle(-115, -50, 100);
    if(button == 4){
        ofDrawCircle(-115, -50, ofRandom(100, 120));
    }
    font.drawString("AUTUMN", -205, -80);
    
    //    緑
    ofSetColor(90, 180, 130);
    ofDrawCircle(115, -50, 100);
    if(button == 5){
        ofDrawCircle(115, -50, ofRandom(100, 120));
    }
    font.drawString("SPRING", 45, -80);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::draw_line(ofPoint p_1, ofPoint p_2) {
    ofDrawLine(p_1, p_2);
}

//--------------------------------------------------------------
void ofApp::exit(){
}

