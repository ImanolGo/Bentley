/*
 *  VideoScene.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 30/04/19.
 *
 */


#include "VideoScene.h"
#include "AppManager.h"

VideoScene::VideoScene(std::string name): ofxScene(name)
{
    //Intentionally left empty
}

VideoScene::~VideoScene()
{
    m_videoPlayer.close();
}


void VideoScene::setup() {
    ofLogNotice(getName() + "::setup");
    this->setupVideo();
}

void VideoScene::setupVideo()
{
    auto& videoPaths = AppManager::getInstance().getVideoManager().getVideoResourcesPath();
    
    if(videoPaths.find(getName())!=videoPaths.end())
    {
        string path = videoPaths.at(getName());
        m_videoPlayer.load(path);
        m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
        m_videoPlayer.play();
    }
    else{
        ofLogNotice() << getName() + "::setupVideo-> Cannot find " << getName();
    }
}


void VideoScene::update()
{
    this->updateVideo();
}

void VideoScene::updateVideo()
{
    if(m_videoPlayer.isInitialized() && m_videoPlayer.isLoaded())
    {
        m_videoPlayer.update();
    }
}

void VideoScene::draw()
{
    ofBackground(0,0,0);
    this->drawVideo();
}

void VideoScene::drawVideo()
{
    if(m_videoPlayer.isInitialized() && m_videoPlayer.isLoaded())
    {
        
        float width = AppManager::getInstance().getSettingsManager().getAppWidth();
        float height = AppManager::getInstance().getSettingsManager().getAppHeight();
        
        m_videoPlayer.draw(0,0,width,height);
    }
    
}

void VideoScene::willFadeIn() {
     ofLogNotice("VideoScene::willFadeIn");
    
    if(m_videoPlayer.isInitialized() && m_videoPlayer.isLoaded())
    {
        m_videoPlayer.play();
    }
}

void VideoScene::willDraw() {
    ofLogNotice("VideoScene::willDraw");
}

void VideoScene::willFadeOut() {
    ofLogNotice("VideoScene::willFadeOut");
}

void VideoScene::willExit() {
    if(m_videoPlayer.isInitialized() && m_videoPlayer.isLoaded())
    {
        m_videoPlayer.stop();
    }
}
