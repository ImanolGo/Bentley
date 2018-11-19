/*
 *  DmxManager.h
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 19/11/18.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxDmx4All.h"

//========================== class DmxManager =======================================
//==============================================================================
/** \class DmxManager DmxManager.h
 *	\brief class for managing DMX communication
 *	\details It connects to an DNX 4 All USB DMX controller and sends serial-dmx commands
 */


class DmxManager: public Manager
{

public:
    //! Constructor
    DmxManager();

    //! Destructor
    virtual ~DmxManager();

    //! setups DMX
    void setup();

    void update();

    void onSetDmxLightChannel(int& value);
    
    void onSetDmxLightColor(const ofColor& color);
    
    void onSetDmxLightWhite(int& value);
    
    void onSetDmxLightStrobe();
    
    void onSetDmxStrobeColor(int value);
    
    void onSetDmxLightSolid();

private:
    
    void setupDmx();
    
    void exit();
    
private:
    
	ofxDmx4All m_dmx;
    int m_dmxLightStartChannel;

};

