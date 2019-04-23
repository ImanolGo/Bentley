/*
 *  TcpManager.cpp
 *  Bentley App
 *
 *  Created by Imanol Gomez on 22/04/19.
 *
 */




#include "TcpManager.h"
#include "AppManager.h"


TcpManager::TcpManager(): Manager(), m_connected(false)
{
    //Intentionally left empty
}

TcpManager::~TcpManager()
{
    m_tcp.close();
    ofLogNotice() << "TcpManager::destructor";
}


//--------------------------------------------------------------

void TcpManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupTcpConnection();
    

    ofLogNotice() <<"TcpManager::initialized" ;
}


void TcpManager::setupTcpConnection()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getTcpPortReceive();
    ofLogNotice() <<"TcpManager::setupTcpConnection -> setting up TCP server to listen to " << portReceive;
    
    m_connected = m_tcp.setup(portReceive);
    m_tcp.setMessageDelimiter("\r\n");
    
}



void TcpManager::update()
{
    this->updateReveivePackage();

}

void TcpManager::updateReveivePackage()
{
    if(!m_connected){
        return;
    }
    
    
    for ( int i = 0; i < m_tcp.getLastID(); i++ ) {
        if(m_tcp.isClientConnected(i)) {
            string received = m_tcp.receive(i); // will end with delimiter, so make sure client is sending it
            
            if(received!=""){
                ofLogNotice() <<"TcpManager::updateReveivePackage -> id: " << i << ", received: " << received;
                this->parseMessage(received, i);
            }
        }
    }
}


void TcpManager::parseMessage(string& msg, int id)
{
    auto result = ofSplitString(msg, " ");
    if(result.empty()){
        this->sendError(id);
        return;
    }
    
    auto command = result[0];
    
    if(command == "?"){
        this->sendStatus(id);
    }
    
    else if(command == "on"){
        AppManager::getInstance().getVideoManager().play();
        this->sendStatus(id);
    }
    
    else if(command == "off"){
        AppManager::getInstance().getVideoManager().stop();
        this->sendStatus(id);
    }
    
    else if(command == "mode"){
        if(result.size()>1){
            AppManager::getInstance().getVideoManager().setVideoIndex(ofToInt(result[1]));
            this->sendStatus(id);
        }
        else{
             this->sendError(id);
        }
       
    }
    
    else{
         this->sendError(id);
    }
    
}

void TcpManager::sendStatus(int i)
{
    bool bStatud = AppManager::getInstance().getVideoManager().getCurrentStatus();
    string status =  bStatud ? "on " : "off ";
    int index = AppManager::getInstance().getVideoManager().getCurrentIndex();
    string message = "OK " + status + ofToString(index);
    m_tcp.send(i, message);
    ofLogNotice() <<"TcpManager::sendStatus ->  " << message;
}

void TcpManager::sendError(int i)
{
    string message = "ERROR";
    m_tcp.send(i, message);
    ofLogNotice() <<"TcpManager::sendError ->  " << message;
}


