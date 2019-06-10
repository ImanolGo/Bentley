/*
 *  TestScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 30/04/19.
 *
 */


#include "TestScene.h"
#include "AppManager.h"

TestScene::TestScene(): ofxScene("Test"), m_initialized(false)
{
    //Intentionally left empty
}

TestScene::~TestScene()
{
    //Intentionally left empty
}


void TestScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    ofLogNotice("TestScene::::setup");
    this->setupImage();
    
    m_initialized = true;
    
}

void TestScene::setupImage()
{
    m_image.load("images/general/color_spectrum.png");
}


void TestScene::update()
{
    //Intentioanlly empty
}

void TestScene::draw()
{
    ofBackground(0,0,0);
    this->drawImage();
}

void TestScene::drawImage()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    m_image.draw(0,0,width,height);

}

void TestScene::willFadeIn() {
     ofLogNotice("TestScene::willFadeIn");
}

void TestScene::willDraw() {
    ofLogNotice("TestScene::willDraw");
}

void TestScene::willFadeOut() {
    ofLogNotice("TestScene::willFadeOut");
}

void TestScene::willExit() {
}
