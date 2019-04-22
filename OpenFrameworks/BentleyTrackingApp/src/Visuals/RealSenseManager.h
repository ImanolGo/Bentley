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
#include "ofxCv.h"



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
    
    bool isRealSenseActive() const {return m_isRealSenseActive;}
    
    ofParameterGroup& getParameters()  {return m_parameters;}

    
private:
    
    void setupCamera();
    
    void setupCV();
    
    void updateCamera();
    
    void updateCV();
    
    void drawCamera();
   
    
private:
    
    
    ofxLibRealSense2            m_realSense;
    ofxCv::ObjectFinder         m_finder;
    ofFbo                       m_trackerFbo;
    ofxCv::RunningBackground    m_background;
    ofxCv::ContourFinder        m_contourFinder;
    ofImage                     m_thresholded;
    
    ofParameterGroup        m_parameters;
    
    ofParameter<bool>        m_resetBackground;
    ofParameter<float>       m_learningTime;
    ofParameter<float>       m_thresholdValue;
    
    ofParameter<float>       m_minArea;
    ofParameter<float>       m_maxArea;
    ofParameter<float>       m_threshold;
    ofParameter<bool>        m_holes;
    
    bool m_isRealSenseActive;
    
};




