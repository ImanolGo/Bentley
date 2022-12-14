/*
 *  ModelManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#pragma once

#include "Manager.h"
#include "myEasyCam.h"


//========================== class ModelManager ==============================
//============================================================================
/** \class ModelManager ModelManager.h
 *	\brief Class managing the model visuals
 *	\details it creates and updates the 3D model visuals
 */


class ModelManager: public Manager
{
    public:

        //! Constructor
        ModelManager();

        //! Destructor
        ~ModelManager();

        //! Setup the Vase Manager
        void setup();

        //! Update the Vase Manager
        void update();

        //! Draw the Vase Manager
        void draw();

        void resetCamera();

    private:


        //! Draw all Leds
        void drawLeds();

    private:
    
        myEasyCam m_camera; // add mouse controls for camera movement
    
        ofFbo     m_fbo;

    

};
