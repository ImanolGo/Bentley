/*
 *  ColorScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 16/06/19.
 *
 */


#include "ColorScene.h"
#include "AppManager.h"

ColorScene::ColorScene(): ofxScene("Color"), m_initialized(false)
{
    //Intentionally left empty
}

ColorScene::~ColorScene()
{
    //Intentionally left empty
}


void ColorScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    
    ofLogNotice(getName() + "::setup");
    this->setupRectangle();
    
    m_initialized = true;
}

void ColorScene::setupRectangle()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_rectangle = RectangleVisual(ofPoint(0), width, height, false);
}


void ColorScene::update()
{
    this->updateRectangle();
}

void ColorScene::updateRectangle()
{
    auto floatColor = AppManager::getInstance().getGuiManager().getSolidColor();
    //auto color = ofColor(floatColor.b*255, floatColor.g*255, floatColor.b*255 );
    m_rectangle.setColor(floatColor);
    
}


void ColorScene::draw()
{
    ofBackground(0,0,0);
    this->drawRectangle();
}

void ColorScene::drawRectangle()
{
    m_rectangle.draw();
}

void ColorScene::willFadeIn() {
     ofLogNotice("ColorScene::willFadeIn");
}

void ColorScene::willDraw() {
    ofLogNotice("ColorScene::willDraw");
}

void ColorScene::willFadeOut() {
    ofLogNotice("ColorScene::willFadeOut");
}

void ColorScene::willExit() {
    ofLogNotice("ColorScene::willExit");
}
