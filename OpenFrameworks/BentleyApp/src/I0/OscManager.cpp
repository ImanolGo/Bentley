/*
 *  OscManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 22/04/19.
 *
 */



#include "OscManager.h"
#include "SettingsManager.h"
#include "AppManager.h"


const string OscManager::OSC_PARENT_ADDRESS = "/BentleyApp";

OscManager::OscManager(): Manager()
{
    //Intentionally left empty
}

OscManager::~OscManager()
{
   ofLogNotice() << "OscManager::destructor";
}


//--------------------------------------------------------------

void OscManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupOscReceiver();
    //this->setupOscSender();
    this->setupText();
    
    ofLogNotice() <<"OscManager::initialized" ;
}

void OscManager::setupOscReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getOscPortReceive();
   
    if(m_oscReceiver.setup(portReceive))
    {
         ofLogNotice() <<"OscManager::setupOscReceiver -> listening to port " << portReceive;
    }
    else{
        ofLogNotice() <<"OscManager::setupOscReceiver -> could not listen to port  " << portReceive;
    }
    
}

void OscManager::setupOscSender()
{
    int portSend = AppManager::getInstance().getSettingsManager().getOscPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();
    m_oscSender.setup(host, portSend);
    ofLogNotice() <<"OscManager::setupOscSender -> open osc connection " << host << ":" << portSend;
    
}


void OscManager::setupText()
{
    
    int fontSize = 12;
    float width = ofGetWidth() - 6*LayoutManager::MARGIN - AppManager::getInstance().getGuiManager().getWidth();
    float  height = fontSize*3;
    
    ofVec3f pos; pos.x = AppManager::getInstance().getGuiManager().getWidth() + 4*LayoutManager::MARGIN;
    pos.y = ofGetHeight() - 3*LayoutManager::MARGIN - height;
    int portReceive = AppManager::getInstance().getSettingsManager().getOscPortReceive();
    string text ="OSC Receive (port: " +  ofToString(portReceive) + ")";
    
    
    m_receivingInformation = ofPtr<TextVisual>(new TextVisual(pos, width, height));
    m_receivingInformation->setText(text, LayoutManager::LAYOUT_FONT_LIGHT, fontSize);
    m_receivingInformation->setColor(ofColor::white);
}



void OscManager::update()
{
    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(m);
        
        if(m.getAddress() == OSC_PARENT_ADDRESS + "/Scene")
        {
            int value = m.getArgAsInt(0);
            AppManager::getInstance().getGuiManager().onSceneChange(value);
        }
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/Video")
        {
            int value = m.getArgAsInt(0);
            AppManager::getInstance().getGuiManager().onVideoChange(value);
        }
        
        else if(m.getAddress() == OSC_PARENT_ADDRESS + "/TransitionTime")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setSceneTransitionTime(value);
        }
        
        int portReceive = AppManager::getInstance().getSettingsManager().getOscPortReceive();
        string text = "OSC Receive (port: " +  ofToString(portReceive) + ")-> " + this->getMessageAsString(m);
        if(m_receivingInformation){
            
            m_receivingInformation->setText(text);
        }
        
        ofLogNotice() << text;
    }
}

void OscManager::draw()
{
    m_receivingInformation->draw();
}

string OscManager::getMessageAsString(const ofxOscMessage& m) const
{
    string msg_string;
    msg_string = m.getAddress();
    for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += " ";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += m.getArgAsString(i);
        }
        else{
            msg_string += "unknown";
        }
    }
    
    return msg_string;
}









