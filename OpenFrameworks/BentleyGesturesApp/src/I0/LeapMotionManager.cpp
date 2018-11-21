/*
 *  LeapMotionManager.cpp
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 19/11/18.
 *
 */



#include "LeapMotionManager.h"
#include "AppManager.h"


LeapMotionManager::LeapMotionManager(): Manager(), m_currentGesture(-1)
{
    //Intentionally left empty
}

LeapMotionManager::~LeapMotionManager()
{
    ofLogNotice() << "LeapMotionManager::destructor";
}


//--------------------------------------------------------------

void LeapMotionManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupLeapMotion();
    this->setupFbos();
    
    ofLogNotice() <<"LeapMotionManager::initialized" ;
}


void LeapMotionManager::setupLeapMotion()
{
    m_leap.open();
    m_leap.setupGestures();   // we enable our gesture detection here
}


void LeapMotionManager::setupFbos()
{
    if(m_cameraFbo.isAllocated()){
        return;
    }
    
    ofImage imRight,imLeft;
    m_leap.getImage(imRight,0);
    m_leap.getImage(imLeft,1);
    
    if(!imRight.isAllocated() ||  !imLeft.isAllocated() ){
        return;
    }
  
    m_cameraFbo.allocate(imLeft.getWidth() + imRight.getWidth(), imLeft.getHeight(), GL_RGB);
    m_cameraFbo.begin(); ofClear(0); m_cameraFbo.end();
    
    ofLogNotice() <<"LeapMotionManager::setupFbos: m_cameraFbo-> width = " <<  m_cameraFbo.getWidth() << ", height = " << m_cameraFbo.getHeight();
}






void LeapMotionManager::update()
{
    this->updateGestures();
    this->processGesture();
    this->updateFbos();
}


void LeapMotionManager::updateGestures()
{
    m_leap.updateGestures();  // check for gesture updates
    m_leap.markFrameAsOld();  //IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
}

void LeapMotionManager::updateFbos()
{
    if(!m_cameraFbo.isAllocated()){
        this->setupFbos();
    }
    else{
        ofImage imRight,imLeft;
        m_leap.getImage(imRight,0);
        m_leap.getImage(imLeft,1);
        
        m_cameraFbo.begin();
            imLeft.draw(0, 0);
            imRight.draw(imLeft.getWidth(), 0);
        m_cameraFbo.end();
    }
   
}


void LeapMotionManager::draw()
{
    this->drawCameraFbo();
}

void LeapMotionManager::drawCameraFbo()
{
    if(!m_cameraFbo.isAllocated()){
        return;
    }
    
    string name = "Camera";
    ofRectangle frame;
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    float ratio = m_cameraFbo.getWidth()/ m_cameraFbo.getHeight();
    frame.height = rect->getHeight();
    frame.width = frame.height*ratio;
    
    if( frame.width > rect->getWidth() ){
        frame.width = rect->getWidth();
        frame.height = frame.width/ratio;
    }
    
    frame.x = rect->getWidth()*0.5 - frame.width*0.5;
    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
    
    //m_cameraFbo.draw(0,0, rect->getWidth(), rect->getHeight());
    m_cameraFbo.draw(frame);
    
//    ofImage imRight,imLeft;
//    m_leap.getImage(imRight,0);
//    m_leap.getImage(imLeft,1);
//    imRight.draw(0, 0, rect->getWidth(), rect->getHeight()) ;
}


void LeapMotionManager::processGesture()
{
    /* Leap iGesture Key
     --------------------------------
     1 = Screen Tap
     2 = Key Tap
     3 = Swipe Right
     4 = Swipe Left
     5 = Swipe Down
     6 = Swipe Up
     7 = Swipe Forward
     8 = Swipe Backward (towards yourself)
     9 = Circle Left (counter-clockwise)
     10 = Circle Right (clockwise)
     --------------------------------
     */
    
    if(m_currentGesture == m_leap.iGestures){
        return;
    }
    
    m_currentGesture = m_leap.iGestures;
    
    switch (m_currentGesture) {
    
        case 3:
            ofLogNotice() <<"LeapMotionManager::processGesture-> Swipe Right" ;
            AppManager::getInstance().getSceneManager().changeScene("Cocoon");
            break;
            
        case 4:
            ofLogNotice() <<"LeapMotionManager::processGesture-> Swipe Left" ;
            AppManager::getInstance().getSceneManager().changeScene("Enhance");
            break;
            
        case 5:
            ofLogNotice() <<"LeapMotionManager::processGesture-> Swipe Down" ;
            AppManager::getInstance().getSceneManager().changeScene("Capture");
            break;
            
        case 6:
            ofLogNotice() <<"LeapMotionManager::processGesture-> Swipe Up" ;
            AppManager::getInstance().getSceneManager().changeScene("Re-live");
            break;
            
        case 7:
            ofLogNotice() <<"LeapMotionManager::processGesture-> Swipe Forward" ;
            break;
            
        case 8:
            ofLogNotice() <<"LeapMotionManager::processGesture-> Swipe Backwards" ;
            break;
            
        default:
            ofLogNotice() <<"LeapMotionManager::processGesture-> Waiting..." ;
            break;
    }
}

