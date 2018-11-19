/*
 *  CamManager.cpp
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */



#include "ofMain.h"

#include "AppManager.h"
#include "CamManager.h"




CamManager::CamManager(): Manager()
{
    //m_videoPlayer = new ofxAVFVideoPlayer();
}


CamManager::~CamManager()
{
    ofLogNotice() <<"CamManager::Destructor" ;
}


void CamManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    this->setupCamera();
    
    ofLogNotice() <<"CamManager::initialized" ;
    
}



void CamManager::setupCamera()
{
    m_camWidth = 1280;
    m_camHeight = 720;
    
    //m_camWidth = 960;  // try to grab at this size.
    //m_camHeight =540;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = m_vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << "CamManager::setupCamera-> " << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() <<  "CamManager::setupCamera-> " << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    if(devices.size()>1){
        m_vidGrabber.setDeviceID(1);
    }
    else{
         m_vidGrabber.setDeviceID(0);
    }
   
    //m_vidGrabber.setDesiredFrameRate(60);
    m_vidGrabber.setup(m_camWidth, m_camHeight);
    m_vidGrabber.videoSettings();
}


void CamManager::update()
{
    this->updateCamera();
}


void CamManager::updateCamera()
{
    m_vidGrabber.update();
    
    if(m_vidGrabber.isFrameNew()){
        ///
    }
}



void CamManager::draw()
{
    ofClear(0);
    this->drawCamera();
}

void CamManager::drawCamera()
{
    string name = "Camera";
    ofRectangle frame;
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = m_vidGrabber.getWidth()/ m_vidGrabber.getHeight();
    frame.height = rect->getHeight();
    frame.width = frame.height*ratio;
    
    if( frame.width > rect->getWidth() ){
        frame.width = rect->getWidth();
        frame.height = frame.width/ratio;
    }
    
    frame.x = rect->getWidth()*0.5 + frame.width*0.5;
    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
    
    //video.getWidth(),0,-video.getWidth(),video.getHeight()
    
    m_vidGrabber.draw(frame.x,frame.y, -frame.width, frame.height);

}


