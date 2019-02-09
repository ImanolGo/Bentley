/*
 *  RealSenseManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 07/02/19.
 *
 */


#include "ofMain.h"

#include "AppManager.h"
#include "RealSenseManager.h"

using namespace ofxCv;
using namespace cv;



RealSenseManager::RealSenseManager(): Manager(), m_isRealSenseActive(0)
{
    //m_videoPlayer = new ofxAVFVideoPlayer();
}


RealSenseManager::~RealSenseManager()
{
    ofLogNotice() <<"RealSenseManager::Destructor" ;
    m_realSense.exit();
}


void RealSenseManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    this->setupCamera();
    this->setupCV();
    
    ofLogNotice() <<"RealSenseManager::initialized" ;
    
}



void RealSenseManager::setupCamera()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    int fps = 30;
    
    m_isRealSenseActive = m_realSense.setupDevice(0);
    m_realSense.setupColor(width, height, fps);
    m_realSense.setupIR(width, height, fps);
    m_realSense.setupDepth(width, height, fps);
    m_realSense.startPipeline(true);

}

void RealSenseManager::setupCV()
{
    m_finder.setup("xmls/haarcascade_frontalface_default.xml");
    m_finder.setPreset(ObjectFinder::Fast);
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_trackerFbo.allocate(width, height);
    m_trackerFbo.begin(); ofClear(0,0,0,0); m_trackerFbo.end();
    
    m_parameters.setName("OpenCV");
    m_parameters.add(m_resetBackground.set("Reset Background", false));
    m_parameters.add(m_learningTime.set("Learning Time", 30, 0, 30));
    m_parameters.add(m_thresholdValue.set("Threshold Value", 10, 0, 255));
    m_parameters.add(m_minArea.set("Min area", 10, 1, 100));
    m_parameters.add(m_maxArea.set("Max area", 200, 1, 500));
    m_parameters.add(m_threshold.set("Threshold", 128, 0, 255));
    m_parameters.add(m_holes.set("Holes", false));
}





void RealSenseManager::update()
{
    if(!m_isRealSenseActive){
        return;
    }
    
    this->updateCamera();
}


void RealSenseManager::updateCamera()
{
    m_realSense.update();
    
    if(m_resetBackground) {
        m_background.reset();
        m_resetBackground = false;
    }
    
    if(m_realSense.isFrameNew()){
        this->updateCV();
    }
}


void RealSenseManager::updateCV()
{
   
    ofImage image;
    ofPixels pixels;
    m_realSense.getIrTex()->readToPixels(pixels);
    image.setFromPixels(pixels);
   
    
    m_background.setLearningTime(m_learningTime);
    m_background.setThresholdValue(m_thresholdValue);
    m_background.update(image, m_thresholded);
    m_thresholded.update();
    
    m_contourFinder.setMinAreaRadius(m_minArea);
    m_contourFinder.setMaxAreaRadius(m_maxArea);
    m_contourFinder.setThreshold(m_threshold);
    m_contourFinder.findContours(m_thresholded);
    m_contourFinder.setFindHoles(m_holes);
    
     //m_finder.update(image);
   
    m_trackerFbo.begin();
    ofClear(0,0,0,0);
        m_contourFinder.draw();
        //m_finder.draw();
    m_trackerFbo.end();
}

void RealSenseManager::draw()
{
    ofClear(0);
    if(!m_isRealSenseActive){
        return;
    }
    
    this->drawDepth();
}


void RealSenseManager::drawIR()
{
    if(!m_isRealSenseActive){
        return;
    }
    
    string name = "Camera";
    ofRectangle frame;
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = width /height;
    frame.height = rect->getHeight();
    frame.width = frame.height*ratio;
    
    if( frame.width > rect->getWidth() ){
        frame.width = rect->getWidth();
        frame.height = frame.width/ratio;
    }
    
    frame.x = rect->getWidth()*0.5 + frame.width*0.5;
    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
    
    //video.getWidth(),0,-video.getWidth(),video.getHeight()
    
    ofEnableAlphaBlending();
    if(m_realSense.irEnabled()) {
        m_realSense.getIrTex()->draw(frame.x,frame.y, -frame.width, frame.height);
        m_trackerFbo.draw(frame.x,frame.y, -frame.width, frame.height);
    }
     ofDisableAlphaBlending();
}

void RealSenseManager::drawColor()
{
    if(!m_isRealSenseActive){
        return;
    }
    
    string name = "Camera";
    ofRectangle frame;
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = width /height;
    frame.height = rect->getHeight();
    frame.width = frame.height*ratio;
    
    if( frame.width > rect->getWidth() ){
        frame.width = rect->getWidth();
        frame.height = frame.width/ratio;
    }
    
    frame.x = rect->getWidth()*0.5 + frame.width*0.5;
    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
    
    //video.getWidth(),0,-video.getWidth(),video.getHeight()
    
    if(m_realSense.colorEnabled()) {
        m_realSense.getColorTex()->draw(frame.x,frame.y, -frame.width, frame.height);
        
    }
    
}

void RealSenseManager::drawDepth()
{
    if(!m_isRealSenseActive){
        return;
    }
    
    string name = "Camera";
    ofRectangle frame;
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height = AppManager::getInstance().getSettingsManager().getAppHeight();
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = width /height;
    frame.height = rect->getHeight();
    frame.width = frame.height*ratio;
    
    if( frame.width > rect->getWidth() ){
        frame.width = rect->getWidth();
        frame.height = frame.width/ratio;
    }
    
    frame.x = rect->getWidth()*0.5 + frame.width*0.5;
    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
    
    //video.getWidth(),0,-video.getWidth(),video.getHeight()
    
    if(m_realSense.depthEnabled()) {
        m_realSense.getDepthTex()->draw(frame.x,frame.y, -frame.width, frame.height);
        
    }
    
   

}


