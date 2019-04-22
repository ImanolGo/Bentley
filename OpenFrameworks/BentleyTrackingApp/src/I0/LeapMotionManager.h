/*
 *  LeapMotionManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxLeapMotion2.h"


//========================== class LeapMotionManager =======================================
//==============================================================================
/** \class LeapMotionManager LeapMotionManager.h
 *    \brief class for managing the leap connection and gestures
 *    \details  It connects the Leap Motion controller, process the gestures and triggers the proper actions
 */


class LeapMotionManager: public Manager
{
   
public:
    //! Constructor
    LeapMotionManager();
    
    //! Destructor
    virtual ~LeapMotionManager();
    
    //! setups the leap motion
    void setup();
    
    //! updates the leap motion
    void update();
    
    void draw();
    
    void drawHands();
    
    void drawCamera();
    
    
private:
    
    void setupLeapMotion();
    
    void setupFbos();
    
    void updateGestures();
    
    void updateHands();
    
    void updateFbos();
    
    void processGesture();
    
    void drawLeapHands();
    
private:
    
    ofxLeapMotion   m_leap;
    ofFbo           m_cameraFbo;
    ofFbo           m_handsFbo;
    int             m_currentGesture;
    
    vector <ofxLeapMotionSimpleHand> m_simpleHands;
    
    vector <int>                    m_fingersFound;
    ofEasyCam                       m_cam;
    
};


