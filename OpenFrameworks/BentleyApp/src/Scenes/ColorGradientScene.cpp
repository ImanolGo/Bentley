/*
 *  ColorGradientScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 30/04/19.
 *
 */


#include "ColorGradientScene.h"
#include "AppManager.h"

ColorGradientScene::ColorGradientScene(): ofxScene("ColorGradient"), m_initialized(false)
{
    //Intentionally left empty
}

ColorGradientScene::~ColorGradientScene()
{
    //Intentionally left empty
}


void ColorGradientScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    ofLogNotice("ColorGradientScene::::setup");
    this->setupImage();
    
    m_initialized = true;
    
}

void ColorGradientScene::setupImage()
{
    m_image.load("images/general/color_spectrum.png");
}


void ColorGradientScene::update()
{
    //Intentioanlly empty
}

void ColorGradientScene::draw()
{
    ofBackground(0,0,0);
    this->drawImage();
}

void ColorGradientScene::drawImage()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    m_image.draw(0,0,width,height);

}

void ColorGradientScene::willFadeIn() {
     ofLogNotice("ColorGradientScene::willFadeIn");
}

void ColorGradientScene::willDraw() {
    ofLogNotice("ColorGradientScene::willDraw");
}

void ColorGradientScene::willFadeOut() {
    ofLogNotice("ColorGradientScene::willFadeOut");
}

void ColorGradientScene::willExit() {
}
