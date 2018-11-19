/*
 *  DmxManager.cpp
 *  Bentley Gestures App
 *
 *  Created by Imanol Gomez on 19/11/18.
 *
 */


#include "DmxManager.h"
#include "AppManager.h"


DmxManager::DmxManager(): Manager(), m_dmxLightStartChannel(1)
{
    //Intentionally left empty
}

DmxManager::~DmxManager()
{
   ofLogNotice() << "DmxManager::destructor";
   this->exit();
}


//--------------------------------------------------------------

void DmxManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupDmx();
    ofLogNotice() <<"DmxManager::initialized" ;
}



void DmxManager::setupDmx()
{
    int dmxPort = AppManager::getInstance().getSettingsManager().getDmxPort();
    if(dmxPort<0){
        ofLogNotice() <<"DmxManager::setupSerial << Autoconnecting dmx port";
        m_dmx.autoconnect(255);
    }
    else{
        ofLogNotice() <<"DmxManager::setupSerial << Connecting dmx to port " << dmxPort;
        m_dmx.connect(dmxPort,255);
    }
}



void DmxManager::update()
{
    m_dmx.update();
}



void DmxManager::onSetDmxLightChannel(int& value)
{
    m_dmxLightStartChannel = ofClamp(value, 1, 512);
    ofLogNotice() <<"DmxManager::onSetDmxLightChannel << DmxLightStartChannel " << m_dmxLightStartChannel;
}


void DmxManager::onSetDmxLightColor(const ofColor& color)
{
//    m_dmx.setLevel(m_dmxLightStartChannel + 4, color.r);
//    m_dmx.setLevel(m_dmxLightStartChannel + 5, color.g);
//    m_dmx.setLevel(m_dmxLightStartChannel + 6, color.b);
    
    m_dmx.setLevel(m_dmxLightStartChannel + 1, color.r);
    m_dmx.setLevel(m_dmxLightStartChannel + 2, color.g);
    m_dmx.setLevel(m_dmxLightStartChannel + 3, color.b);
    m_dmx.update();
}


void DmxManager::onSetDmxLightWhite(int& value)
{
    m_dmx.setLevel(m_dmxLightStartChannel + 4, value);
    m_dmx.update();
}

void DmxManager::onSetDmxStrobeColor(int value)
{
    value = ofClamp(value,0,255);
    m_dmx.setLevel(m_dmxLightStartChannel + 1, value); //Set strobe color according to value
    m_dmx.update();
}

void DmxManager::onSetDmxLightStrobe()
{
//    m_dmx.setLevel(m_dmxLightStartChannel + 3, 0); //Set master dimming to 0
//    m_dmx.setLevel(m_dmxLightStartChannel, 201); //Set Strobe mode
//    m_dmx.setLevel(m_dmxLightStartChannel+2, 155); //Set Strobe speed
    
    m_dmx.setLevel(m_dmxLightStartChannel, 201); //Set Strobe mode and speed
    m_dmx.update();
}

void DmxManager::onSetDmxLightSolid()
{
//    m_dmx.setLevel(m_dmxLightStartChannel, 0); //Set mode to manual
//    m_dmx.setLevel(m_dmxLightStartChannel + 3, 255); //Set master dimming to full
    
    m_dmx.setLevel(m_dmxLightStartChannel, 240); //Ch2-Ch5 effect
    m_dmx.update();
}

void DmxManager::exit()
{
    int speed = 0;
    this->onSetDmxLightSolid();
    this->onSetDmxLightColor(ofColor::black);
    m_dmx.setLevel(m_dmxLightStartChannel + 3, 0); //Set master dimming to zero
    m_dmx.update();
    ofSleepMillis(300);
}
