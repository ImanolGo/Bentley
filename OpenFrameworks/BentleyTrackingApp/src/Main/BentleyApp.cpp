/*
 *  BentleyApp.cpp
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#include "AppManager.h"

#include "BentleyApp.h"

//--------------------------------------------------------------
void BentleyApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void BentleyApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void BentleyApp::draw(){
    AppManager::getInstance().draw();
}

void BentleyApp::exit()
{
    ofLogNotice() <<"BentleyApp::exit";

}

//--------------------------------------------------------------
void BentleyApp::keyPressed(int key){

}

//--------------------------------------------------------------
void BentleyApp::keyReleased(int key){

}

//--------------------------------------------------------------
void BentleyApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void BentleyApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void BentleyApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void BentleyApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void BentleyApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);


}

//--------------------------------------------------------------
void BentleyApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void BentleyApp::dragEvent(ofDragInfo dragInfo){

}
