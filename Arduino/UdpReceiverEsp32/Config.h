#pragma once


/*************************************************************************
 Protocol
 *************************************************************************/

#define CHANNEL 0
#define HEADER_SIZE 32
#define DATA_HEADER_SIZE 10
#define TIME_PORT 8
#define TLC_PORT 9
#define DATA_PORT 10
#define SENSOR_PORT 11
#define MNUDP_VER 2
#define MNCORE_VER 1
#define ORIGIN 0

/*************************************************************************
 Leds
 *************************************************************************/


#define LED_TYPE  WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN_  5
#define NUM_CHANNELS 1
#define NUM_LEDS 50
#define MAX_BRIGHTNESS 255
#define TEST_DELAY 500
#define FPS_CHECK_TIME_MS 2000
#define CHANNEL_WIDTH 3



/*************************************************************************
 WiFi
 *************************************************************************/

#define BUFFER_MAX 1500
#define DISCOVERY_TIMER 3000
#define WIFI_TIMEOUT 3000              // checks WiFi every ...ms. Reset after this time, if WiFi cannot reconnect.
#define NO_DATA_TIMEOUT 10000         // sends autodiscovery if no data is coming after timeout
#define LOCAL_PORT 7000 
#define DISCOVERY_PORT 2390
#define SEND_PORT 7001 


/*************************************************************************
 Fuctions
 *************************************************************************/

unsigned long ByteToLong(byte b1, byte b2, byte b3, byte b4)
{
  unsigned long bignum = 0; 
  bignum = bignum + b1;
  bignum << 8;
  bignum = bignum + b2;
  bignum << 8;
  bignum = bignum + b3;
  bignum << 8;
  bignum = bignum + b4;
  return bignum;
}

unsigned short ByteToShort(byte b1, byte b2)
{
  unsigned short bignum = 0; 
  bignum = bignum + b1;
  bignum << 8;
  bignum = bignum + b2;
  return bignum;
}
