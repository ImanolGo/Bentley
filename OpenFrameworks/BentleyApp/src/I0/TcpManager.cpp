/*
 *  TcpManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 22/04/19.
 *
 */




#include "TcpManager.h"
#include "AppManager.h"

#ifndef TARGET_WIN32
#include "ofxMyIP.h"
#endif


const int TcpManager::UDP_MESSAGE_LENGHT = 100;

TcpManager::TcpManager(): Manager(), m_connected(false), m_ledsPerChannel(300)
{
    //Intentionally left empty
}

TcpManager::~TcpManager()
{
    ofLogNotice() << "TcpManager::destructor";
}


//--------------------------------------------------------------

void TcpManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupHeaders();
    this->setupIP();
    //this->setupTimer();
    this->sendAutodiscovery();
    this->setupUdpConnection();
    
   
    
    ofLogNotice() <<"TcpManager::initialized" ;
}


void TcpManager::setupHeaders()
{
    m_dataHeader.f1 = 0x10;
    m_dataHeader.f2 = 0x41;
    m_dataHeader.f3 = 0x37;
    m_dataHeader.size = 0;
    m_dataHeader.command = 'd';
    m_dataHeader.channel = 0;
    
    m_connectHeader.f1 = 0x10;
    m_connectHeader.f2 = 0x41;
    m_connectHeader.f3 = 0x37;
    m_connectHeader.size = 1;
    m_connectHeader.command = 'c';
    m_connectHeader.channel = 0;
}

void TcpManager::setupUdpConnection()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    ofLogNotice() <<"TcpManager::setupUdpReceiver -> listening for udp messages on port  " << portReceive;
    
    //m_udpConnection.SetEnableBroadcast(true);
    m_udpConnection.Create(); //create the socket
    m_udpConnection.Bind(portReceive); //and bind to port
    m_udpConnection.SetNonBlocking(true);
    
    
    string ip = AppManager::getInstance().getSettingsManager().getIpAddress();
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    
    m_udpConnection.Connect(m_broadcast.c_str(),portSend);
    //m_udpConnection.Connect(ip.c_str(),portSend);
    m_udpConnection.SetEnableBroadcast(true);
    
    ofLogNotice() <<"TcpManager::setupUdpReceiver -> sending to IP " << m_broadcast <<" to port " << portSend;
    
    m_udpConnection.SetNonBlocking(true);
    
}

void TcpManager::createConnection(string& ip, int send)
{
    ofLogNotice() <<"TcpManager::createConnection -> sending to IP " << ip.c_str() <<" to port " << send;
    
    m_udpConnection.Connect(ip.c_str(),send);
    m_udpConnection.SetNonBlocking(true);
    m_connected = true;
}

void TcpManager::setupTimer()
{
    m_timer.setup( 60000 );
    
    m_timer.start( false ) ;
    ofAddListener( m_timer.TIMER_COMPLETE , this, &TcpManager::timerCompleteHandler ) ;
}

void TcpManager::setupIP()
{
    #ifdef TARGET_WIN32
        system("ipfirst.cmd");
        ofFile file("my.ip");
        file >> m_ip;
        //ofLog() << "My IP: " << m_ip;

    #else
        ofxMyIP myip;
        myip.setup();
        m_ip = myip.getIpAddress();

    #endif

    ofLogNotice() <<"TcpManager::setupIP -> IP address: " << m_ip;

    m_broadcast = "";
    auto stringSplit = ofSplitString(m_ip, ".");

    for(int i=0; i<stringSplit.size(); i++){

        if(i<stringSplit.size()-1){
            m_broadcast += stringSplit[i];
            m_broadcast += ".";
        }

    }

    m_broadcast+="255";

    ofLogNotice() <<"TcpManager::setupIP -> Broadcast IP address: " << m_broadcast;

}
void TcpManager::update()
{
    this->updateReveivePackage();
    this->updatePixels();
   // m_timer.update();
    
}

void TcpManager::updatePixels()
{
    if(!m_connected){
        return;
    }
    
    int ledsPerPixel = 3;

    const auto & colors = AppManager::getInstance().getLedsManager().getColors();
    const int length = colors.size()*ledsPerPixel;
    const char* pixels[length];


   // ofLogNotice() <<"TcpManager::updatePixels -> New Frame " << leds.size();

    
    int numChannels = colors.size()/m_ledsPerChannel + 1;

    //ofLogNotice() <<"TcpManager::updatePixels -> numChannels " << numChannels;

    for(int channel=0; channel<numChannels; channel++){
        int startIndex  = channel*m_ledsPerChannel;
        int endIndex  = (channel+1)*m_ledsPerChannel;

        if(endIndex>colors.size()){
            endIndex = colors.size();
        }

        string message="";
        message+= m_dataHeader.f1; message+= m_dataHeader.f2; message+= m_dataHeader.f3;
        int size = endIndex - startIndex;
        m_dataHeader.size = ledsPerPixel*size;
        unsigned char * s = (unsigned char*)& m_dataHeader.size;
        message+= s[1] ;  message+=  s[0];
        message+=m_dataHeader.command;
        message+=channel;
        
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        

        for(int i=startIndex; i<endIndex; i++){
            if(i>=colors.size()){
                break;
            }
            
            r = colors[i].r * 255;
            g = colors[i].g * 255;
            b = colors[i].b * 255;
            
            message+=r;
            message+=g;
            message+=b;

        }

        m_udpConnection.Send(message.c_str(),message.length());
    }


    
}

void TcpManager::updateReveivePackage()
{
    char udpMessage[UDP_MESSAGE_LENGHT];
    m_udpConnection.Receive(udpMessage,UDP_MESSAGE_LENGHT);
    string message=udpMessage;
    
    if(message!="")
    {
        ofLogNotice() <<"TcpManager::updateReveivePackage -> SIZE " << message.size();
        ofLogNotice() <<"TcpManager::updateReveivePackage -> message " << message;
        
        this->parseMessage(udpMessage, UDP_MESSAGE_LENGHT);
    }
}

bool TcpManager::isMessage(char * buffer, int size)
{
    if(buffer[0] != m_connectHeader.f1  && buffer[1] != m_connectHeader.f2  && buffer[2] != m_connectHeader.f3 ){
        ofLogNotice() <<"TcpManager::isMessage -> FALSE ";
        return false;
    }
    
    
    ofLogNotice() <<"TcpManager::isMessage -> TRUE ";
    return true;
}

void TcpManager::parseMessage(char * buffer, int size)
{
    if(isMessage(buffer, size))
    {
        if(buffer[5] == m_connectHeader.command)
        {
            ofLogNotice() <<"TcpManager::isMessage -> Received Connect Command: " << m_connectHeader.command;
            string ip; int port;
            m_udpConnection.GetRemoteAddr(ip, port);
            int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
            this->createConnection(ip, portSend );
            this->sendConnected();
            m_timer.stop();
        }
        
    }
      
}

void TcpManager::sendColor(ofColor & color)
{
    if(!m_connected){
        return;
    }
    
    int ledsPerPixel = 3;
    int size = 1;
    int channel=0;
    string message="";
    message+= m_dataHeader.f1; message+= m_dataHeader.f2; message+= m_dataHeader.f3;
    m_dataHeader.size = ledsPerPixel*size;
    unsigned char * s = (unsigned char*)& m_dataHeader.size;
    message+= s[1] ;  message+=  s[0];
    message+=m_dataHeader.command;
    message+=channel;

    message+=color.r;
    message+=color.g;
    message+=color.b;

    m_udpConnection.Send(message.c_str(),message.length());
    
}

void TcpManager::timerCompleteHandler( int &args )
{
    m_timer.start(false);
    //  cout<<"TIMER COMPLETED"<<endl;
    this->sendAutodiscovery();
}

void TcpManager::sendConnected()
{
    string message="";
    message+= m_connectHeader.f1; message+= m_connectHeader.f2; message+= m_connectHeader.f3;
    unsigned char * s = (unsigned char*)& m_connectHeader.size;
    message+= s[1] ;  message+=  s[0];
    message+=m_connectHeader.command;
    message+=m_connectHeader.channel;
    message+='c';
    
    m_udpConnection.Send(message.c_str(),message.length());
    
    ofLogNotice() <<"TcpManager::sendConnected -> Send Connected ";
}
void TcpManager::sendAutodiscovery()
{
//    string message="";
//    
//    message+= START_COMMAND;
//    message+= AUTODISCOVERY_COMMAND;
//    message+= END_COMMAND;
//    
//    m_udpConnection.Send(message.c_str(),message.length());
//    
//    ofLogNotice() <<"TcpManager::sendAutodiscovery << " << message;
}
