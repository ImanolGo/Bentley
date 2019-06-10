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
#include "Brancher.h"




//========================== class UdpManager =======================================
//==============================================================================
/** \class UdpManager UdpManager.h
 *    \brief class for managing the udp connection
 *    \details It writes and send udp data
 */


class UdpManager: public Manager
{
    static const int UDP_MESSAGE_LENGHT; ///Defines the Udp"s message length
    static const int UDP_MTU_ETHERNET; ///Defines the Ethernet's maximum transmission unit
    static const int DATA_HEADER_OVERHEAD; ///Defines the data's header overhead
    static const double UDP_SEND_TIME; ///Defines the time to send a new Udp Package
    
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
  
    const string& getIpAddress() const {return m_ip;}
    
    void setupConnection(unsigned short _id);
    
    void updatePixels();
    
    void setStreaming(bool& value){m_streaming = value;}
    
    void nextFrame(){m_nextFrame = true;}
    
    void sendTlcSettings(const unsigned char& bcr, const unsigned char& bcg, const unsigned char& bcb);
    
private:
    
    void setupHeaders();
    
    void setupIP();
    
    void updateTime();
    
    void updateElapsedTime();
    
    void setupReceiver();
    
    void sendTest();
    
    void updateReveivePackage();
    
    bool isMessage(char * buffer, int size);
    
    void parseMessage(char * buffer, int size);
    
    void receivedIp(char _id);
    
    string getDataHeader(unsigned int num_pixels);
    
    string getDataPayload(unsigned short _id, unsigned int offset, int num_pixels, const vector<ofColor>& pixels);
    
private:
    
    typedef std::map< unsigned short, shared_ptr<ofxUDPManager> > UdpConnectionMap;
    
    UdpConnectionMap m_udpConnections;
    ofxUDPManager   m_udpConnection;
    udp_header    m_tileDataHeader;
    udp_header    m_timeHeader;
    udp_header    m_sensorHeader;
    udp_header    m_tlcSettingsHeader;
    unsigned int  m_packetID;
    unsigned short  m_maxNumPixelsPerPacket;
    unsigned int    m_frameNumber;
    bool            m_streaming;
    bool            m_nextFrame;
    double          m_elapsedTime;
    
    
    string                 m_ipRoot;
    string                 m_broadcast;
    string                 m_ip;
    
};

