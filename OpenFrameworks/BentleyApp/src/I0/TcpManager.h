/*
 *  TcpManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 22/04/19.
 *
 */




#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "Manager.h"



//========================== class TcpManager =======================================
//==============================================================================
/** \class TcpManager TcpManager.h
 *    \brief class for managing the tcp connection
 *    \details It writes and send tcp data
 */


class TcpManager: public Manager
{
    
public:
    //! Constructor
    TcpManager();
    
    //! Destructor
    virtual ~TcpManager();
    
    //! setups the udp manager
    void setup();
    
    //! updates the udp manager
    void update();
    
    
private:
    
    void setupTcpConnection();
    
    void updateReveivePackage();
    
    bool isMessage(char * buffer, int size);
    
    void parseMessage(string& msg, int _id);
    
    void sendStatus(int i);
    
    void sendError(int i);
    
    
private:
    
    ofxTCPServer           m_tcp;
    bool                   m_connected;

};

