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
const int UdpManager::UDP_MTU_ETHERNET = 1450;
const int UdpManager::DATA_HEADER_OVERHEAD = 60;
const double UdpManager::UDP_SEND_TIME = 1.0;


UdpManager::UdpManager(): Manager(), m_packetID(0), m_maxNumPixelsPerPacket(100), m_frameNumber(1), m_streaming(true), m_elapsedTime(0.0), m_nextFrame(false)
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
    
    this->setupIP();
    this->setupHeaders();
    this->setupReceiver();
    
    //this->setupUdpConnection(100);
   // this->sendTest();
    
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
    m_tlcSettingsHeader.payload_size = 4;
    
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
    
    m_maxDataPacketSize = UDP_MTU_ETHERNET;
    m_maxNumPixelsPerPacket = (m_maxDataPacketSize-DATA_HEADER_OVERHEAD)/3;
    
    ofLogNotice() <<"UdpManager::setupHeaders-> int size : " << sizeof(unsigned int) ;
    ofLogNotice() <<"UdpManager::setupHeaders-> short size : " << sizeof(unsigned short) ;
    unsigned int x = 1;
    ofLogNotice() <<"UdpManager::little endian -> " << (int) (((char *)&x)[0]);

}

void UdpManager::setupConnection(unsigned short _id)
{
    
    string ip = m_ipRoot + ofToString(int(_id));
    
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    
    ofxUDPSettings settings;
    settings.sendTo(ip.c_str(),portSend);
    settings.blocking = true;
    
    //ofxUDPManager connection;
    auto connection = std::make_shared<ofxUDPManager>();
    
//connection.Create(); //create the socket
    if(connection->Setup(settings)){
        m_udpConnections[_id] = connection;
        
        ofLogNotice() <<"UdpManager::setupConnection -> udp socket created : id = " << int(_id) << ", ip = " << ip <<",  port = " << portSend;
    }
    else{
         ofLogNotice() <<"UdpManager::setupConnection ->unable to create udp socket : id = " << int(_id)  << ", ip = " << ip <<",  port = " << portSend;
    }

}

void UdpManager::setupReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    //ofLogNotice() <<"UdpManager::setupUdpReceiver -> listening for udp messages on port  " << portReceive;
    
    //m_udpConnection.SetEnableBroadcast(true);
    m_udpConnection.Create(); //create the socket
    if( m_udpConnection.Bind(portReceive)) //and bind to port
    {
         ofLogNotice() <<"UdpManager::setupReceiver -> listening to port  " << portReceive;
    }
    
//    if(m_udpConnection.Connect(m_ip.c_str(),portSend))
//    {
//        ofLogNotice() <<"UdpManager::setupReceiver -> sending to " << m_ip << ", port = " << portSend;
//    }
//    else{
//         ofLogNotice() <<"UdpManager::setupReceiver -> could not connect to " << m_ip;
//    }

    
    m_udpConnection.SetNonBlocking(true);
    
}

void UdpManager::setupIP()
{
    Poco::Net::NetworkInterface::List siteLocalInterfaces = ofxNet::NetworkUtils::listNetworkInterfaces(ofxNet::NetworkUtils::SITE_LOCAL);
    
    if(!siteLocalInterfaces.empty()){
        m_ip = siteLocalInterfaces[0].address().toString();
    }
    

    ofLogNotice() <<"UdpManager::setupIP -> IP address: " << m_ip;
    // m_ip = "100.100.100.190";

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
   // this->updateElapsedTime();
    this->updateReveivePackage();
    this->updatePixels();
}

void UdpManager::updateElapsedTime()
{
    m_elapsedTime+=ofGetLastFrameTime();
}



void UdpManager::sendTest()
{
    ofLogNotice() <<"UdpManager::sendTest ";
    
    vector<ofColor> pixels;
    pixels.push_back(ofColor(0)); pixels.push_back(ofColor(127)); pixels.push_back(ofColor(255));
    string message = this->getDataHeader(pixels.size());
    message+=this->getDataPayload(100, 0, pixels.size(), pixels);
    m_udpConnection.Send(message.c_str(), message.length());
    
    this->updateTime();
    
}


void UdpManager::updatePixels()
{
//    if(m_streaming && m_elapsedTime<UDP_SEND_TIME){
//        return;
//    }
//    
//    m_elapsedTime = 0;
    
    if(!m_streaming && !m_nextFrame){
        return;
    }
    
    m_nextFrame = false;
    
    const auto & branches = AppManager::getInstance().getLedsManager().getBranchers();
    
    for(auto& branch: branches){
        unsigned short id = branch.second->getId();
         //ofLogNotice() <<"UdpManager::updatePixels -> brancher " << id;
        if(m_udpConnections.find(id)!=m_udpConnections.end()){
           // ofLogNotice() <<"UdpManager::updatePixels -> brancher FOUND " << id;
            int bytesPerLed = 3;
            auto& pixels = branch.second->getPixels();
            unsigned short division = pixels.size()/m_maxNumPixelsPerPacket;
            unsigned int remainder = pixels.size()%m_maxNumPixelsPerPacket;
            unsigned int offset = 0;
            
           
            for(int i=0; i<division; i++){
                
                string message = this->getDataHeader(m_maxNumPixelsPerPacket);
                message+=this->getDataPayload(id, offset,m_maxNumPixelsPerPacket,pixels);
                m_udpConnections[id]->Send(message.c_str(), message.length());
                offset+=m_maxNumPixelsPerPacket;
                //this->printHex(message);
            }
            
            if(remainder!=0)
            {
                string message = this->getDataHeader(remainder);
                message+=this->getDataPayload(id, offset,remainder,pixels);
                m_udpConnections[id]->Send(message.c_str(), message.length());
                //this->printHex(message);
            }
        }
    }
    
    
    this->updateTime();
    
}

string UdpManager::getDataHeader(unsigned int num_pixels)
{
    string message="";
    
    m_tileDataHeader.packet_id = ++m_packetID;
    m_tileDataHeader.payload_size = 10 + num_pixels*3;
    
    unsigned char * s = (unsigned char*)& m_tileDataHeader.mnudp_ver;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tileDataHeader.mncore_ver;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tileDataHeader.origin;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tileDataHeader.mbc_hash;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tileDataHeader.packet_id;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tileDataHeader.response_time;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tileDataHeader.endpoint_id;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tileDataHeader.port;
    message+= s[0];  message+= s[1];
    s = (unsigned char*)& m_tileDataHeader.payload_size;
    message+= s[0];  message+= s[1];
    
    return message;
}

string UdpManager::getDataPayload(unsigned short _id, unsigned int offset, int num_pixels, const vector<ofColor>& pixels)
{
    string message="";
    
    unsigned char * s = (unsigned char*)& m_frameNumber;
    message+= s[0];  message+= s[1];
    s = (unsigned char*)& _id;
    message+= s[0];  message+= s[1];
    s = (unsigned char*)& offset;
    message+= s[0];  message+= s[1];message+= s[2];  message+= s[3];
    s = (unsigned char*)& num_pixels;
    message+= s[0];  message+= s[1];
    
    for(int j=0; j< num_pixels; j++)
    {
        message+=pixels[offset+j].r;
        message+=pixels[offset+j].g;
        message+=pixels[offset+j].b;
    }
    
    return message;
}

void UdpManager::updateTime()
{
    
    m_timeHeader.packet_id = ++m_packetID;
   
    string message="";
    unsigned char * s = (unsigned char*)& m_timeHeader.mnudp_ver;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_timeHeader.mncore_ver;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_timeHeader.origin;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_timeHeader.mbc_hash;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_timeHeader.packet_id;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_timeHeader.response_time;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_timeHeader.endpoint_id;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_timeHeader.port;
    message+= s[0];  message+= s[1];
    s = (unsigned char*)& m_timeHeader.payload_size;
    message+= s[0];  message+= s[1];
    s = (unsigned char*)& m_frameNumber;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    
    for(auto& udpConnection: m_udpConnections){
        udpConnection.second->Send(message.c_str(),message.length());
    }
    
    //m_udpConnection.Send(message.c_str(),message.length());
    
    m_frameNumber++;
    
}


void UdpManager::sendTlcSettings(const unsigned char& bcr, const unsigned char& bcg, const unsigned char& bcb)
{
   
    m_tlcSettingsHeader.packet_id = ++m_packetID;
    
    string message="";
    unsigned char * s = (unsigned char*)& m_tlcSettingsHeader.mnudp_ver;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tlcSettingsHeader.mncore_ver;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tlcSettingsHeader.origin;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tlcSettingsHeader.mbc_hash;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tlcSettingsHeader.packet_id;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tlcSettingsHeader.response_time;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tlcSettingsHeader.endpoint_id;
    message+= s[0];  message+= s[1];  message+= s[2];  message+= s[3];
    s = (unsigned char*)& m_tlcSettingsHeader.port;
    message+= s[0];  message+= s[1];
    s = (unsigned char*)& m_tlcSettingsHeader.payload_size;
    message+= s[0];  message+= s[1];
    
    unsigned char global_settings = 2;
    message+=global_settings;
    message+=bcr;
    message+=bcg;
    message+=bcb;
    
    for(auto& udpConnection: m_udpConnections){
        udpConnection.second->Send(message.c_str(),message.length());
    }

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


void UdpManager::printHex(const string& message)
{
    std::stringstream ss;
    for(int i=0; i<message.length(); ++i){
        ss << std::hex << (int)message[i] << " ";
    }
    std::string mystr = ss.str();
    
    ofLogNotice() <<"UdpManager::printHex ->  hex: " << mystr;
}

void UdpManager::setMaxDataPacketSize(int& value )
{
    m_maxDataPacketSize = (unsigned short) value;
    m_maxNumPixelsPerPacket = (m_maxDataPacketSize-DATA_HEADER_OVERHEAD)/3;
}





