/*
 *  UdpManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#include "UdpManager.h"
#include "AppManager.h"
#include "ofxNetworkUtils_.h"


const int UdpManager::UDP_MESSAGE_LENGHT = 100;

UdpManager::UdpManager(): Manager(), m_connected(false), m_ledsPerChannel(300)
{
    //Intentionally left empty
}

UdpManager::~UdpManager()
{
    ofLogNotice() << "UdpManager::destructor";
}


//--------------------------------------------------------------

void UdpManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupHeaders();
    this->setupReceiver();
    this->setupIP();
    this->setupUdpConnection(102);
    
    ofLogNotice() <<"UdpManager::initialized" ;
}


void UdpManager::setupHeaders()
{
   
    m_timeHeader.mnudp_ver = 2;
    m_timeHeader.mncore_ver = 1;
    m_timeHeader.origin  = 0;
    m_timeHeader.mbc_hash  = 0;
    m_timeHeader.packet_id = 0;
    m_timeHeader.response_time = 0;
    m_timeHeader.endpoint_id = 0;
    m_timeHeader.port = 8;
    m_timeHeader.payload_size = 4;
    
    m_tlcSettingsHeader.mnudp_ver = 2;
    m_tlcSettingsHeader.mncore_ver = 1;
    m_tlcSettingsHeader.origin  = 0;
    m_tlcSettingsHeader.mbc_hash  = 0;
    m_tlcSettingsHeader.packet_id = 0;
    m_tlcSettingsHeader.response_time = 0;
    m_tlcSettingsHeader.endpoint_id = 0;
    m_tlcSettingsHeader.port = 9;
    m_tlcSettingsHeader.payload_size = 0;
    
    m_tileDataHeader.mnudp_ver = 2;
    m_tileDataHeader.mncore_ver = 1;
    m_tileDataHeader.origin  = 0;
    m_tileDataHeader.mbc_hash  = 0;
    m_tileDataHeader.packet_id = 0;
    m_tileDataHeader.response_time = 0;
    m_tileDataHeader.endpoint_id = 0;
    m_tileDataHeader.port = 10;
    m_tileDataHeader.payload_size = 0;
    
    m_sensorHeader.mnudp_ver = 2;
    m_sensorHeader.mncore_ver = 1;
    m_sensorHeader.origin  = 0;
    m_sensorHeader.mbc_hash  = 0;
    m_sensorHeader.packet_id = 0;
    m_sensorHeader.response_time = 0;
    m_sensorHeader.endpoint_id = 0;
    m_sensorHeader.port = 11;
    m_sensorHeader.payload_size = 0;
    
    ofLogNotice() <<"UdpManager::setupHeaders-> int size : " << sizeof(unsigned int) ;
    ofLogNotice() <<"UdpManager::setupHeaders-> short size : " << sizeof(unsigned short) ;
    unsigned int x = 1;
    ofLogNotice() <<"UdpManager::little endian -> " << (int) (((char *)&x)[0]);

}

void UdpManager::setupUdpConnection(unsigned char _id)
{
    
    string ip = m_ipRoot + ofToString(int(_id));
    
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    ofxUDPManager connection;
    
    if(connection.Connect(ip.c_str(),portSend)){
        connection.SetNonBlocking(true);
        m_udpConnections[_id] = connection;
        
        ofLogNotice() <<"UdpManager::setupUdpConnection -> connection created : id = " << int(_id) << ", ip = " << ip <<",  port = " << portSend;
    }
    else{
         ofLogNotice() <<"UdpManager::setupUdpConnection ->unable to create : id = " << int(_id)  << ", ip = " << ip <<",  port = " << portSend;
    }

}

void UdpManager::setupReceiver()
{
    void setupUdpConnection(unsigned short _id);
    
    
    int portReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    //ofLogNotice() <<"UdpManager::setupUdpReceiver -> listening for udp messages on port  " << portReceive;
    
    //m_udpConnection.SetEnableBroadcast(true);
    m_udpReceiver.Create(); //create the socket
    if( m_udpReceiver.Bind(portReceive)) //and bind to port
    {
         ofLogNotice() <<"UdpManager::setupReceiver -> listening to port  " << portReceive;
    }
  
    m_udpReceiver.SetNonBlocking(true);
    
}

void UdpManager::setupIP()
{
    Poco::Net::NetworkInterface::List siteLocalInterfaces = ofxNet::NetworkUtils::listNetworkInterfaces(ofxNet::NetworkUtils::SITE_LOCAL);
    
    if(!siteLocalInterfaces.empty()){
        m_ip = siteLocalInterfaces[0].address().toString();
    }
    

    ofLogNotice() <<"UdpManager::setupIP -> IP address: " << m_ip;

    m_broadcast = "";
    auto stringSplit = ofSplitString(m_ip, ".");

    for(int i=0; i<stringSplit.size(); i++){

        if(i<stringSplit.size()-1){
            m_broadcast += stringSplit[i];
            m_broadcast += ".";
        }

    }
    
    m_ipRoot = m_broadcast;
    m_broadcast+="255";

    ofLogNotice() <<"UdpManager::setupIP -> Broadcast IP address: " << m_broadcast;
    ofLogNotice() <<"UdpManager::setupIP -> Root IP address: " << m_ipRoot;

}
void UdpManager::update()
{
    //this->updateReveivePackage();
    //this->updatePixels();
   // m_timer.update();
    
}

void UdpManager::updatePixels()
{
    if(!m_connected){
        return;
    }
    
    int ledsPerPixel = 3;

    const auto & colors = AppManager::getInstance().getLedsManager().getColors();
    const int length = colors.size()*ledsPerPixel;
	//char* pixels = new char[length];
    //const char* pixels[length];


   // ofLogNotice() <<"UdpManager::updatePixels -> New Frame " << leds.size();

    
//    int numChannels = colors.size()/m_ledsPerChannel + 1;
//
//    //ofLogNotice() <<"UdpManager::updatePixels -> numChannels " << numChannels;
//
//    for(int channel=0; channel<numChannels; channel++){
//        int startIndex  = channel*m_ledsPerChannel;
//        int endIndex  = (channel+1)*m_ledsPerChannel;
//
//        if(endIndex>colors.size()){
//            endIndex = colors.size();
//        }
//
//        string message="";
//        message+= m_dataHeader.f1; message+= m_dataHeader.f2; message+= m_dataHeader.f3;
//        int size = endIndex - startIndex;
//        m_dataHeader.size = ledsPerPixel*size;
//        unsigned char * s = (unsigned char*)& m_dataHeader.size;
//        message+= s[1] ;  message+=  s[0];
//        message+=m_dataHeader.command;
//        message+=channel;
//
//        unsigned char r = 0;
//        unsigned char g = 0;
//        unsigned char b = 0;
//
//
//        for(int i=startIndex; i<endIndex; i++){
//            if(i>=colors.size()){
//                break;
//            }
//
//            r = colors[i].r * 255;
//            g = colors[i].g * 255;
//            b = colors[i].b * 255;
//
//            message+=r;
//            message+=g;
//            message+=b;
//
//        }

       // m_udpConnection.Send(message.c_str(),message.length());
    //}
 
}

void UdpManager::updateReveivePackage()
{
//    char udpMessage[UDP_MESSAGE_LENGHT];
//    m_udpConnection.Receive(udpMessage,UDP_MESSAGE_LENGHT);
//    string message=udpMessage;
//
//    if(message!="")
//    {
//        ofLogNotice() <<"UdpManager::updateReveivePackage -> SIZE " << message.size();
//        ofLogNotice() <<"UdpManager::updateReveivePackage -> message " << message;
//
//        this->parseMessage(udpMessage, UDP_MESSAGE_LENGHT);
//    }
}

bool UdpManager::isMessage(char * buffer, int size)
{
//    if(buffer[0] != m_connectHeader.f1  && buffer[1] != m_connectHeader.f2  && buffer[2] != m_connectHeader.f3 ){
//        ofLogNotice() <<"UdpManager::isMessage -> FALSE ";
//        return false;
//    }
    
    
    ofLogNotice() <<"UdpManager::isMessage -> TRUE ";
    return true;
}

void UdpManager::parseMessage(char * buffer, int size)
{
//    if(isMessage(buffer, size))
//    {
//        if(buffer[5] == m_connectHeader.command)
//        {
//            ofLogNotice() <<"UdpManager::isMessage -> Received Connect Command: " << m_connectHeader.command;
//            string ip; int port;
//            m_udpConnection.GetRemoteAddr(ip, port);
//            int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
//            this->createConnection(ip, portSend );
//            this->sendConnected();
//            m_timer.stop();
//        }
//
//    }
    
}



