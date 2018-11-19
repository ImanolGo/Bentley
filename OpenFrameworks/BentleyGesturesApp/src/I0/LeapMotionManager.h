/*
 *  LeapMotionManager.h
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 19/11/18.
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
    
    
private:
    
    void setupLeapMotion();
    
    void updateGestures();
    
    void processGesture();

    
private:
    
    ofxLeapMotion   m_leap;
    int             m_currentGesture;
    
};


