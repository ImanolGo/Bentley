/*
 *  ReliveScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#include "ReliveScene.h"
#include "AppManager.h"

ReliveScene::ReliveScene(): ofxScene("Re-live"){}

void ReliveScene::setup() {
    ofLogNotice("ReliveScene::setup");
    this->setupText();
    this->setupColor();
}

void ReliveScene::setupText()
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
void ReliveScene::setupColor()
{
    //m_color = ofColor::orange;
    m_color = ofColor::fromHex(0x8e44ad);
}
 

void ReliveScene::update()
{
    
}

void ReliveScene::draw() {
    ofBackground(m_color);
    this->drawText();
    //ofBackgroundGradient( ofColor(255,0,0), ofColor(0,255,0), OF_GRADIENT_CIRCULAR );
}

void ReliveScene::drawText() {
    m_text.draw();
}


void ReliveScene::willFadeIn() {
     ofLogNotice("ReliveScene::willFadeIn");
}

void ReliveScene::willDraw() {
    ofLogNotice("ReliveScene::willDraw");
    AppManager::getInstance().getColorManager().setColor(m_color);
}

void ReliveScene::willFadeOut() {
    ofLogNotice("ReliveScene::willFadeOut");
}

void ReliveScene::willExit() {
    ofLogNotice("ReliveScene::willExit");
}
