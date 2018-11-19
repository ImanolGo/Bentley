/*
 *  BentleyGesturesApp.cpp
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 19/11/18.
 *
 */



#include "AppManager.h"

#include "BentleyGesturesApp.h"

//--------------------------------------------------------------
void BentleyGesturesApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void BentleyGesturesApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void BentleyGesturesApp::draw(){
    AppManager::getInstance().draw();
}

void BentleyGesturesApp::exit()
{
    ofLogNotice() <<"BentleyGesturesApp::exit";

}

//--------------------------------------------------------------
void BentleyGesturesApp::keyPressed(int key){

}

//--------------------------------------------------------------
void BentleyGesturesApp::keyReleased(int key){

}

//--------------------------------------------------------------
void BentleyGesturesApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void BentleyGesturesApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void BentleyGesturesApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void BentleyGesturesApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void BentleyGesturesApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);


}

//--------------------------------------------------------------
void BentleyGesturesApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void BentleyGesturesApp::dragEvent(ofDragInfo dragInfo){

}
