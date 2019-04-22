/*
 *  ColorManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#include "ofMain.h"

#include <time.h>

#include "ColorManager.h"
#include "AppManager.h"



ColorManager::ColorManager(): Manager(), m_alpha(1.0)
{
	//Intentionally left empty
}


ColorManager::~ColorManager()
{
    ofLogNotice() <<"ColorManager::Destructor" ;
}


void ColorManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();
    this->setupColors();
    ofLogNotice() <<"ColorManager::initialized" ;
    
}


void ColorManager::setupColors()
{
    m_currentColor = ofColor(0);
    m_previousColor = ofColor(0);
    m_targetColor = ofColor(0);
}


void ColorManager::update()
{
    this->updateColor();
}

void ColorManager::updateColor()
{
    if(m_alpha <=1.0){
        m_alpha = m_alpha + ofGetLastFrameTime();
        m_currentColor = m_previousColor.getLerped(m_targetColor, m_alpha);
        //AppManager::getInstance().getDmxManager().onSetDmxLightColor(m_currentColor);
        AppManager::getInstance().getUdpManager().sendColor(m_currentColor);
        
        //SendColors
    }

}



void ColorManager::draw()
{
   ///Empty
}


void ColorManager::setColor(ofColor& color)
{
    
    m_targetColor = color;
    m_previousColor = m_currentColor;
    m_alpha = 0.0;
    
    ofLogNotice() <<"ColorManager::setColor -> r = " << int(m_targetColor.r) << ", g = " <<  int(m_targetColor.g) << ", b = " <<  int(m_targetColor.b);
    
}



