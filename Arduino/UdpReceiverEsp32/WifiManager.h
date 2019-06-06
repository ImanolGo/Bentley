///////////////////////////////////////////////////////////////////
// Class controlling the wifi connection
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#define OUTPUT_CHANNEL 0
#define HEADER_SIZE 7

#pragma once
#include "Arduino.h"
#include "Config.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "LedsManager.h"



//The udp library class
WiFiUDP UdpReceive;

bool wifiConnected = false;


class WifiManager
{
  private:
 
  LedsManager* ledsManager;  ///< it sends the commands to the leds manager
  

  public:
    
    WifiManager(LedsManager* ledsManager);
    
    void setup();
    void update();

    static void WiFiEvent(WiFiEvent_t event);

  private:

    void initializeWifi();
    void parseUdp();
    void connectToWiFi(const char * ssid, const char * pwd);
    void connectWifi();
    void checkWifiConnection();

    bool parseHeader(const unsigned char* messagein, int messagelength, unsigned short& port_);

    String ssid;
    String pass;
    bool is_connected;

    //WiFiUDP Udp;
    unsigned char packetBuffer[BUFFER_MAX];
    int receivedUdpLength;

  
};


WifiManager::WifiManager(LedsManager* ledsManager)
{
    this->ledsManager=ledsManager;
    
    ssid = "Don't worry, be happy!";
    pass = "whyistheskysohigh?";

//    ssid     =  "TP-LINK_54E4";
//    pass = "27155332";

    wifiConnected = false;

    receivedUdpLength = 0;
    is_connected = false;
}

void WifiManager::setup()
{
    Serial.println("WifiManager::setup");
    initializeWifi();
   // connectWifi();
}

void WifiManager::initializeWifi()
{
  
    UdpReceive.stop();
    UdpReceive.flush();

  
    Serial.println("WifiManager::connect wifi");
    connectToWiFi(ssid.c_str(), pass.c_str());
}


void WifiManager::connectWifi() {

   wifiConnected = true;
     // attempt to connect to WiFi network:
   //Serial.print("Attempting to connect to SSID: ");
   //Serial.println(ssid);
   WiFi.begin(ssid.c_str(), pass.c_str());

   int attempts = 0;
 
    unsigned long connect_start = millis();
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("WifiManager::WIFI STATUS ");Serial.println(WiFi.status());
      Serial.print("..");
      attempts++;
      //connected = true;
      if(attempts>=15){
        wifiConnected =  false;
        break;
      }
   }

   wifiConnected = true;
   if(wifiConnected){
         Serial.print("\nWifiManager::Connected to SSID: ");
         Serial.println(ssid);
      
         Serial.println("WifiManager::IP address: ");
         Serial.println(WiFi.localIP());
        
         Serial.print("\nWifiManager::Starting connection to UDP port ");
         Serial.println(LOCAL_PORT);

         // if you get a connection, report back via serial:
         UdpReceive.begin(LOCAL_PORT);
         UdpReceive.flush();
    

   }

   else{
        Serial.print("\nUnable to connect to: ");
        Serial.println(ssid);
   }

    Serial.print("\nConnected:  ");
    Serial.println(wifiConnected);
  
    //connected = true;
}


void WifiManager::update()
{
    parseUdp();
}


void WifiManager::parseUdp()
{
     // if there's data available, read a packet
    int packetSize = UdpReceive.parsePacket();
    if (packetSize)
    {   
        //Serial.print("WifiManager::New Message: Size -> ");
        //Serial.println(packetSize);
        UdpReceive.read(packetBuffer,BUFFER_MAX); //read UDP packet
        unsigned short port;;
        
        if(parseHeader(packetBuffer, packetSize, port))
        {
            if(port == DATA_PORT)
            {
                this->ledsManager->parseRGBReceived(packetBuffer, packetSize); //process data function
            }
            else if(port == TIME_PORT)
            {
                this->ledsManager->show();
            }
        }
  
    }
}



bool WifiManager::parseHeader(const unsigned char* messagein, int messagelength, unsigned short& port_) {

  if(messagelength < 32){
      return false;
  }

  unsigned short index = 0;
  unsigned long mnudp_ver = ByteToLong(messagein[index++], messagein[index++],messagein[index++],messagein[index++]);
  unsigned long mncore_ver = ByteToLong(messagein[index++], messagein[index++],messagein[index++],messagein[index++]);
  unsigned long origin = ByteToLong(messagein[index++], messagein[index++],messagein[index++],messagein[index++]);
  unsigned long mbc_hash = ByteToLong(messagein[index++], messagein[index++],messagein[index++],messagein[index++]);
  unsigned long packet_id = ByteToLong(messagein[index++], messagein[index++],messagein[index++],messagein[index++]);
  unsigned long response_time = ByteToLong(messagein[index++], messagein[index++],messagein[index++],messagein[index++]);
  unsigned long endpoint_id = ByteToLong(messagein[index++], messagein[index++],messagein[index++],messagein[index++]);
  
  unsigned short port =  ByteToShort(messagein[index++], messagein[index++]);
  unsigned short payload_size =  ByteToShort(messagein[index++], messagein[index++]);
  
  if ( mnudp_ver == MNUDP_VER && mncore_ver == MNCORE_VER && origin == ORIGIN) 
  { 
      if( messagelength - HEADER_SIZE == payload_size)
      {
        port_ = port;
        return true;
      }
      
   }
    
  return false;
}

void WifiManager::connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("WifiManager::Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handlerpin
  WiFi.onEvent(WiFiEvent);
 // WiFi.config(ip, gateway, subnet);
  WiFi.setAutoReconnect(true);
  WiFi.setAutoConnect(true);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("WifiManager::Waiting for WIFI connection...");
}


void WifiManager::checkWifiConnection(){
   if(WiFi.status() != WL_CONNECTED) {
      Serial.println("WifiManager::WiFi connection lost. Reconnecting...");
      this->connectWifi();
    } else {
      Serial.println("WifiManager::WiFi OK");
    }
}


//wifi event handler
void WifiManager::WiFiEvent(WiFiEvent_t event){
    
    IPAddress ip;
  
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WifiManager::WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          UdpReceive.begin(LOCAL_PORT);
          Serial.print("Listening to port: ");
          Serial.println(WiFi.localIP(), LOCAL_PORT); 
          wifiConnected = true;
          ip = WiFi.localIP();  ip[3] = 255;
 
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WifiManager::WiFi lost connection");
          wifiConnected = false;
          //software_Reset();
          break;
    }
}
