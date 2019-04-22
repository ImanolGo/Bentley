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
#include "ofxSimpleTimer.h"
#include "ofxNetworkUtils.h"


//========================== class TcpManager =======================================
//==============================================================================
/** \class TcpManager TcpManager.h
 *    \brief class for managing the tcp connection
 *    \details It writes and send tcp data
 */


class TcpManager: public Manager
{
    static const int UDP_MESSAGE_LENGHT; ///Defines the Udp"s message length
    
    struct udp_header {
        unsigned char f1;
		unsigned char f2;
		unsigned char f3;
        short size;
		unsigned char command;
		unsigned char  channel;
    };
    
public:
    //! Constructor
    TcpManager();
    
    //! Destructor
    virtual ~TcpManager();
    
    //! setups the udp manager
    void setup();
    
    //! updates the udp manager
    void update();
    
    void timerCompleteHandler( int &args ) ;
    
    void setLedsPerChannel(int& value) {m_ledsPerChannel=value;}
    
    void sendColor(ofColor & color);
    
    
private:
    
    void setupUdpConnection();
    
    void setupHeaders();
    
    void setupTimer();
    
    void setupIP();
    
    void updateReveivePackage();
    
    bool isMessage(char * buffer, int size);
    
    void parseMessage(char * buffer, int size);
    
    void receivedIp(char _id);
    
    void receivedHeartbeat(char _id, char val1, char val2);
    
    void createConnection(string& ip, int port);
    
    void sendAutodiscovery();
    
    void sendConnected();
    
    void sendDiscovered();
    
    void updatePixels();
    
    
    
private:
    
    ofxUDPManager m_udpConnection;
    udp_header    m_dataHeader;
    udp_header    m_connectHeader;
    ofxSimpleTimer         m_timer;
    string                 m_broadcast;
    string                 m_ip;
    bool                   m_connected;
    int                    m_ledsPerChannel;
    
};

