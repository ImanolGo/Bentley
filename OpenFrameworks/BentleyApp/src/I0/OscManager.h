/*
 *  OscManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 22/04/19.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxOsc.h"
#include "TextVisual.h"
#include "RectangleVisual.h"

//========================== class OscManager =======================================
//==============================================================================
/** \class OscManager OscManager.h
 *	\brief class for managing the OSC events
 *	\details It reads all the OSC events and create the proper application events
 */


class OscManager: public Manager
{
    
    static const string OSC_PARENT_ADDRESS; //Specifies the osc parent address

public:
    //! Constructor
    OscManager();

    //! Destructor
    virtual ~OscManager();

    //! setups the manager
    void setup();

    //! updates the manager
    void update();
    
    //! draw the manager
    void draw();
    
    
private:
    
    //! sets upt the osc receiver
    void setupOscReceiver();
    
    //! sets up the osc sender
    void setupOscSender();
    
    //! sets up the osc text
    void setupText();
    
    //! gets string formatted OSC message
    string getMessageAsString(const ofxOscMessage& m) const;


 private:
    
     ofxOscReceiver m_oscReceiver;          ///< OSC receiver class
     ofxOscSender   m_oscSender;            ///< OSC sender class
    
     ofPtr<TextVisual>          m_sendingInformation;
     ofPtr<TextVisual>          m_receivingInformation;
     ofPtr<RectangleVisual>     m_boundingBox;
    
};

