/*
 *  EnhanceScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#include "EnhanceScene.h"
#include "AppManager.h"

EnhanceScene::EnhanceScene(): ofxScene("Enhance"){}

void EnhanceScene::setup() {
    ofLogNotice("EnhanceScene::setup");
    this->setupText();
    this->setupColor();
}

void EnhanceScene::setupText()
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
void EnhanceScene::setupColor()
{
    //m_color = ofColor::orange;
    m_color = ofColor::fromHex(0xe74c3c);
}
 

void EnhanceScene::update()
{
    
}

void EnhanceScene::draw() {
    ofBackground(m_color);
    this->drawText();
    //ofBackgroundGradient( ofColor(255,0,0), ofColor(0,255,0), OF_GRADIENT_CIRCULAR );
}

void EnhanceScene::drawText() {
    m_text.draw();
}


void EnhanceScene::willFadeIn() {
     ofLogNotice("EnhanceScene::willFadeIn");
}

void EnhanceScene::willDraw() {
    ofLogNotice("EnhanceScene::willDraw");
    AppManager::getInstance().getColorManager().setColor(m_color);
}

void EnhanceScene::willFadeOut() {
    ofLogNotice("EnhanceScene::willFadeOut");
}

void EnhanceScene::willExit() {
    ofLogNotice("EnhanceScene::willExit");
}
