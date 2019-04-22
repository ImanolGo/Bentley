/*
 *  LeapMotionManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
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
    // keep app receiving data from leap motion even when it's in the background
    //m_leap.setReceiveBackgroundFrames(true);
    
    m_cam.disableMouseInput();
    m_cam.setOrientation(ofPoint(-20, 0, 0));
    m_cam.setDistance(800);
   
    
   // glEnable(GL_DEPTH_TEST);
    //glEnable(GL_NORMALIZE);
}


void LeapMotionManager::setupFbos()
{
  
    if(!m_handsFbo.isAllocated()){
        float width = AppManager::getInstance().getSettingsManager().getAppWidth();
        float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
        m_handsFbo.allocate(width, height);
        m_handsFbo.begin(); ofClear(255,0,0); m_handsFbo.end();
    }
   
    
    if(!m_cameraFbo.isAllocated()){
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
}






void LeapMotionManager::update()
{
    this->updateGestures();
    this->processGesture();
    this->updateHands();
    this->updateFbos();
}


void LeapMotionManager::updateGestures()
{
    m_leap.updateGestures();  // check for gesture updates
    m_leap.markFrameAsOld();  //IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
}


void LeapMotionManager::updateHands()
{
    m_fingersFound.clear();
    
    
    m_simpleHands = m_leap.getSimpleHands();
    
    if( m_leap.isFrameNew() && m_simpleHands.size() ){
        
        m_leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        m_leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        m_leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < m_simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = m_simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = m_simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = m_simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = m_simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = m_simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                m_fingersFound.push_back(id);
            }
        }
    }
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
    this->drawCamera();
}

void LeapMotionManager::drawLeapHands()
{
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
    
     m_cam.begin();
    
    ofPushMatrix();
    ofRotateDeg(90, 0, 0, 1);
    ofSetColor(20);
    ofDrawGridPlane(800, 20, false);
    ofPopMatrix();
    
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < m_simpleHands.size(); i++){
        bool isLeft        =  m_simpleHands[i].isLeft;
        ofPoint handPos    =  m_simpleHands[i].handPos;
        ofPoint handNormal =  m_simpleHands[i].handNormal;
        
        ofSetColor(0, 0, 255);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
        ofSetColor(255, 255, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        for (int f=0; f<5; f++) {
            ofPoint mcp =  m_simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip =  m_simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip =  m_simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip =  m_simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            ofSetColor(0, 255, 0);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 12);
            ofDrawSphere(pip.x, pip.y, pip.z, 12);
            ofDrawSphere(dip.x, dip.y, dip.z, 12);
            ofDrawSphere(tip.x, tip.y, tip.z, 12);
            
            ofSetColor(255, 0, 0);
            ofSetLineWidth(20);
            ofDrawLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofDrawLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofDrawLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
        }
    }
     m_cam.end();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);
}



void LeapMotionManager::drawHands()
{
    this->drawLeapHands();
    
//
//    string name = "Camera";
//    ofRectangle frame;
//    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
//    float ratio = m_handsFbo.getWidth()/ m_handsFbo.getHeight();
//    frame.height = rect->getHeight();
//    frame.width = frame.height*ratio;
//
//    if( frame.width > rect->getWidth() ){
//        frame.width = rect->getWidth();
//        frame.height = frame.width/ratio;
//    }
//
//    frame.x = rect->getWidth()*0.5 - frame.width*0.5;
//    frame.y = rect->getHeight()*0.5 - frame.height*0.5;
//
//    m_handsFbo.draw(frame);
    
    
}


void LeapMotionManager::drawCamera()
{
    if(!m_cameraFbo.isAllocated()){
        return;
    }
    
    string name = "Leap";
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

