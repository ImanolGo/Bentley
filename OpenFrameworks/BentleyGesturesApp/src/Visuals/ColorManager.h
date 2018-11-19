/*
 *  ColorManager.h
 *  TheJourneyOfColorApp
 *
 *  Created by Imanol Gomez on 10/08/18.
 *
 */


#pragma once

#include "Manager.h"



//========================== class ColorManager ==============================
//============================================================================
/** \class ColorManager ColorManager.h
 *	\brief Class managing the color palettes
 *	\details it reads from the camera feed
 */


class ColorManager: public Manager
{
    
    static const int NUM_COLORS;
    
    public:

        //! Constructor
        ColorManager();

        //! Destructor
        ~ColorManager();

        //! Setup the Color Manager
        void setup();
    
        //! Update the Color Manager
        void update();
    
        //! Draw the Color Manager
        void draw();
    
        void setColor(ofColor& color);
    
        ofColor getColor() {return m_currentColor;}
    
    private:
    
        void updateColor();
    
        void setupColors();
    
    private:
    
    ofColor m_targetColor;
    ofColor m_previousColor;
    
    ofColor m_currentColor;
    float m_alpha;
 
};




