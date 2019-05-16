/*
 *  UdpManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "Manager.h"




//========================== class UdpManager =======================================
//==============================================================================
/** \class UdpManager UdpManager.h
 *    \brief class for managing the udp connection
 *    \details It writes and send udp data
 */


class UdpManager: public Manager
{
    static const int UDP_MESSAGE_LENGHT; ///Defines the Udp"s message length
    
    struct udp_header {
        unsigned int mnudp_ver;
		unsigned int mncore_ver;
		unsigned int origin;
        unsigned int mbc_hash;
        unsigned int packet_id;
        unsigned int response_time;
        unsigned int endpoint_id;
        unsigned short port;
        unsigned short payload_size;
        
    };
    
public:
    //! Constructor
    UdpManager();
    
    //! Destructor
    virtual ~UdpManager();
    
    //! setups the udp manager
    void setup();
    
    //! updates the udp manager
    void update();
  
    void setLedsPerChannel(int& value) {m_ledsPerChannel=value;}

    const string& getIpAddress() const {return m_ip;}
    
    void setupUdpConnection(unsigned char _id);
    
private:
    
    void setupHeaders();
    
    void setupIP();
    
    void setupReceiver();
    
    void updateReveivePackage();
    
    bool isMessage(char * buffer, int size);
    
    void parseMessage(char * buffer, int size);
    
    void receivedIp(char _id);
        
    void updatePixels();
    
private:
    
    typedef std::map< unsigned char, ofxUDPManager > UdpConnectionMap;
    
    UdpConnectionMap m_udpConnections;
    ofxUDPManager   m_udpReceiver;
    udp_header    m_tileDataHeader;
    udp_header    m_timeHeader;
    udp_header    m_sensorHeader;
    udp_header    m_tlcSettingsHeader;
    unsigned int  m_packetID;
    
    string                 m_ipRoot;
    string                 m_broadcast;
    string                 m_ip;
    bool                   m_connected;
    int                    m_ledsPerChannel;
    
};

