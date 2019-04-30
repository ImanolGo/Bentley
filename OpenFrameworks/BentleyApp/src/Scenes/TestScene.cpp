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
    m_videoPlayer.close();
}


void TestScene::setup() {
    
    if(m_initialized){
        return;
    }
    
    ofLogNotice("TestScene::::setup");
    this->setupVideo();
    
    m_initialized = true;
    
}

void TestScene::setupVideo()
{
    auto& videoPaths = AppManager::getInstance().getVideoManager().getVideoResourcesPath();
    
    string path = "data/videos/TestVideo.mov";
    
    ofDisableDataPath();
    ofDirectory dir(path);
    if(m_videoPlayer.load(dir.getAbsolutePath())){
         m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
         m_videoPlayer.play();
         ofLogNotice() << "TestScene::setupVideo-> Loaded " << path;
    }
    else{
        ofLogNotice() <<"TestScene::setupVideo-> Cannot find: " << dir.getAbsolutePath();
    }
    
   
    ofEnableDataPath();
    
   
}


void TestScene::update()
{
    this->updateVideo();
}

void TestScene::updateVideo()
{
    if(m_videoPlayer.isLoaded())
    {
        m_videoPlayer.update();
    }
}

void TestScene::draw()
{
    ofBackground(0,0,0);
    this->drawVideo();
}

void TestScene::drawVideo()
{
    if(m_videoPlayer.isInitialized() && m_videoPlayer.isLoaded())
    {
        
        float width = AppManager::getInstance().getSettingsManager().getAppWidth();
        float height = AppManager::getInstance().getSettingsManager().getAppHeight();
        
        m_videoPlayer.draw(0,0,width,height);
    }
    
}

void TestScene::willFadeIn() {
     ofLogNotice("TestScene::willFadeIn");
    
    if(m_videoPlayer.isLoaded())
    {
        ofLogNotice("TestScene::willFadeIn -> PLAY");
        m_videoPlayer.setFrame(0);
        m_videoPlayer.play();
    }
}

void TestScene::willDraw() {
    ofLogNotice("TestScene::willDraw");
}

void TestScene::willFadeOut() {
    ofLogNotice("TestScene::willFadeOut");
}

void TestScene::willExit() {
    if(m_videoPlayer.isLoaded())
    {
        ofLogNotice("TestScene::willFadeIn -> STOP");
        m_videoPlayer.stop();
    }
}
