/*
 *  CaptureScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#include "CaptureScene.h"
#include "AppManager.h"

CaptureScene::CaptureScene(): ofxScene("Capture"){}

void CaptureScene::setup() {
    ofLogNotice("CaptureScene::setup");
    this->setupText();
    this->setupColor();
}

void CaptureScene::setupText()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float w = width - width/20;
    float h = height/10;
    
    float x =  width*0.5;
    float y =  height*0.5;
    float size = h;
    ofPoint pos = ofPoint(x, y);
    string text = this->getName();;
    string fontName = LayoutManager::LAYOUT_FONT;
    
    m_text = TextVisual(pos,w,h,true);
    m_text.setText(text, fontName, size, ofColor::white);
}
void CaptureScene::setupColor()
{
    //m_color = ofColor::orange;
    m_color = ofColor::fromHex(0x27ae60);
}
 

void CaptureScene::update()
{
    
}

void CaptureScene::draw() {
    ofBackground(m_color);
    this->drawText();
    //ofBackgroundGradient( ofColor(255,0,0), ofColor(0,255,0), OF_GRADIENT_CIRCULAR );
}

void CaptureScene::drawText() {
    m_text.draw();
}


void CaptureScene::willFadeIn() {
     ofLogNotice("CaptureScene::willFadeIn");
}

void CaptureScene::willDraw() {
    ofLogNotice("CaptureScene::willDraw");
    AppManager::getInstance().getColorManager().setColor(m_color);
}

void CaptureScene::willFadeOut() {
    ofLogNotice("CaptureScene::willFadeOut");
}

void CaptureScene::willExit() {
    ofLogNotice("CaptureScene::willExit");
}
