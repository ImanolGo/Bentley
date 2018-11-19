/*
 *  CamManager.h
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */


#pragma once

#include "Manager.h"


//========================== class CamManager ==============================
//============================================================================
/** \class CamManager CamManager.h
 *	\brief Class managing the camera processing
 *	\details it reads the freames from the web camera and process them
 */


class CamManager: public Manager
{
    
    
public:
    
    //! Constructor
    CamManager();
    
    //! Destructor
    ~CamManager();
    
    //! Setup the Video Manager
    void setup();
    
    //! Update the Video Manager
    void update();
    
    //! Draw the Video Manager
    void draw();

    
private:
    
    void setupCamera();
    
    void updateCamera();
    
    void drawCamera();
    
   
    
private:
    
    
    ofVideoGrabber      m_vidGrabber;
    int                 m_camWidth;
    int                 m_camHeight;

};




