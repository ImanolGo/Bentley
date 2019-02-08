/*
 *  RealSenseManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 07/02/19.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxLibRealSense2.hpp"


//========================== class RealSenseManager ==============================
//============================================================================
/** \class RealSenseManager RealSenseManager.h
 *	\brief Class managing the Intel RealSense camera
 *	\details it reads the camera frames and process them
 */


class RealSenseManager: public Manager
{
    
    
public:
    
    //! Constructor
    RealSenseManager();
    
    //! Destructor
    ~RealSenseManager();
    
    //! Setup the RealSense Manager
    void setup();
    
    //! Update the RealSense Manager
    void update();
    
    //! Draw the RealSense Manager
    void draw();
    
    void drawDepth();
    
    void drawIR();
    
    void drawColor();
    
    ofxGuiGroup *getGui() {return m_realSense.getGui();}

    
private:
    
    void setupCamera();
    
    void updateCamera();
    
    void drawCamera();
    
   
    
private:
    
    
    ofxLibRealSense2 m_realSense;
    
};




