/*
 *  CocoonScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#include "CocoonScene.h"
#include "AppManager.h"

CocoonScene::CocoonScene(): ofxScene("Cocoon"){}

void CocoonScene::setup() {
    ofLogNotice("CocoonScene::setup");
    this->setupText();
    this->setupColor();
}

void CocoonScene::setupText()
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
void CocoonScene::setupColor()
{
    //m_color = ofColor::orange;
    m_color = ofColor::fromHex(0xf39c12);
}
 

void CocoonScene::update()
{
    
}

void CocoonScene::draw() {
    ofBackground(m_color);
    this->drawText();
    //ofBackgroundGradient( ofColor(255,0,0), ofColor(0,255,0), OF_GRADIENT_CIRCULAR );
}

void CocoonScene::drawText() {
    m_text.draw();
}


void CocoonScene::willFadeIn() {
     ofLogNotice("CocoonScene::willFadeIn");
}

void CocoonScene::willDraw() {
    ofLogNotice("CocoonScene::willDraw");
    AppManager::getInstance().getColorManager().setColor(m_color);
}

void CocoonScene::willFadeOut() {
    ofLogNotice("CocoonScene::willFadeOut");
}

void CocoonScene::willExit() {
    ofLogNotice("CocoonScene::willExit");
}
