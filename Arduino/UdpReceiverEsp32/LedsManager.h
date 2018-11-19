///////////////////////////////////////////////////////////////////
// Class controlling the leds and its effects
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "FastLED.h"
#include "WiFiManager.h"

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_CHANNELS 1
#define NUM_LEDS 100
#define MAX_BRIGHTNESS 100
#define TEST_DELAY 500
#define DATA_PIN    5
#define FPS_CHECK_TIME_MS 2000

const byte channelwidth = 3; //3 channels per pixel


class LedsManager{

  public:
    
    LedsManager();
    
    void setup();
    void update();

    void parseRGBReceived(unsigned char* pbuff, int count);
    void setAllColor(CRGB color);

    void setAllBlack();
    void setColor(CRGB color);
    void setColorPalette(CRGB& color);
    
  private:

    void setupLeds();
    void setupColorPalettes();
    void initTest();
    void checkFps();
    void noise16();

    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;
    
    uint16_t scale;
    uint32_t speed;
    uint8_t maxChanges;

    CRGB leds[NUM_LEDS];
    
};

LedsManager::LedsManager()
{
    this->scale = 500;
    this->speed = 8;
    this->maxChanges = 24; 
}

void LedsManager::setup()
{
    Serial.println("LedsManager::setup");
    this->setupLeds(); 
    this->setupColorPalettes();
}


void LedsManager::setupLeds()
{

    //FastLED.addLeds<WS2812B,32, GRB>(leds[0], NUM_LEDS);
   FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
 
   FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
   //FastLED.setDither( 0 );
   FastLED.clear();  
   this->setAllColor(CRGB::Black); 
   Serial.println("LedsManager::setupLeds");
}

void LedsManager::setupColorPalettes()
{
  this->currentPalette = CRGBPalette16(
                                   CRGB::Black, CRGB::Black, CRGB::Black, CHSV(0, 255,4),
                                   CHSV(0, 255, 8), CRGB::Red, CRGB::Red, CRGB::Red,                                   
                                   CRGB::DarkOrange,CRGB::Orange, CRGB::Orange, CRGB::Orange,
                                   CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);

  this->targetPalette = this->currentPalette;
}


void LedsManager::update()
{
     this->checkFps();
     this->noise16();
    //nblendPaletteTowardPalette( this->currentPalette, this->targetPalette, this->maxChanges);
    FastLED.show();
}

void LedsManager::parseRGBReceived(unsigned char* pbuff, int count) 
{
  //DEBUG_PRINT_LN ("DMX Packet Received");
  int output_channel = pbuff[6];
  int numLeds = count/channelwidth;

//  Serial.print("LedsManager::parseRGBReceived -> outputChannel: ");
//  Serial.println(output_channel);
//
//  Serial.print("LedsManager::parseRGBReceived -> numLeds: ");
//  Serial.println(numLeds);

   if(output_channel < 0 || output_channel>= NUM_CHANNELS){
    return;
  }
  
  if(numLeds > NUM_LEDS){
     numLeds = NUM_LEDS;
  }

    if(numLeds>0){
       CRGB color =  CRGB( pbuff[HEADER_SIZE], pbuff[HEADER_SIZE + 1], pbuff[HEADER_SIZE + 2] );
       this->setColorPalette(color);
       
    }
//    int channel = 0; //reset RGB channel assignment to 0 each time through loop.
//    for (int i = 0; i < numLeds; i++) //loop to assign 3 channels to each pixel
//    {
//        leds[output_channel][i] = CRGB(pbuff[HEADER_SIZE + channel], pbuff[HEADER_SIZE + (channel +1)], pbuff[HEADER_SIZE + (channel +2)]);
//        channel +=channelwidth; //increase last channel number by channel width
//    }
  
  //adjust_gamma();
  
  FastLED.show(); //send data to pixels
}


void LedsManager::setColorPalette(CRGB& color)
{
  // this->targetPalette = CRGBPalette16(pal[0],pal[1],pal[2],pal[3]);
    
  fill_solid( this->targetPalette, 16, CRGB::Black);
  //    // and set every fourth one to a color.
    this->currentPalette[0] = color;
    this->currentPalette[4] = color;
    this->currentPalette[8] = color;
    this->currentPalette[12] = color;

  
   //this->setColor(pal[0]);
}

void LedsManager::checkFps()
{
    EVERY_N_MILLISECONDS(FPS_CHECK_TIME_MS) 
    {
      Serial.print("LedsManager::fsp-> ");
      Serial.println(LEDS.getFPS());                       
   }
}

void LedsManager::noise16()
{
    uint32_t real_z = millis() *this->speed;                          // the z position becomes quickly incremented
    for (uint16_t i = 0; i < NUM_LEDS; i++) 
    {
       uint16_t shift_x = beatsin8(5);                           // the x position of the noise field swings @ 17 bpm
       uint16_t shift_y = millis() / 100;                        // the y position becomes slowly incremented
    

       uint16_t real_x = (i + shift_x)*scale;                    // the x position of the noise field swings @ 17 bpm
       uint16_t real_y = (i + shift_y)*scale;                    // the y position becomes slowly incremented
    
      uint8_t noise = inoise16(real_x, real_y, real_z)>>8;   // get the noise data and scale it down
      uint8_t bri = noise;                           // map LED color based on noise data
      uint8_t index   = noise;
  
      leds[i] = ColorFromPalette(this->currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    
    }
}

void LedsManager::setAllBlack()
{ 
  fill_solid(leds,NUM_LEDS, CRGB::Black);
  FastLED.show();
 
}


void LedsManager::setAllColor(CRGB color) 
{
  fill_solid(leds,NUM_LEDS, color);
  FastLED.show();
}
void LedsManager::initTest() //runs at board boot to make sure pixels are working
{
  FastLED.setBrightness(MAX_BRIGHTNESS);       // set to full power
  
  FastLED.clear(); // on power up, one or more leds can be in a funky state.
  
  
  Serial.println ("LedsManager::Red:");
  this->setAllColor(CRGB::Red);
  
  FastLED.delay(TEST_DELAY);
  
  Serial.println ("LedsManager::Green");
  this->setAllColor(CRGB::Green);
  
  FastLED.delay(TEST_DELAY);
  
  Serial.println ("LedsManager::Blue");
  this->setAllColor(CRGB::Blue);
  
  FastLED.delay(TEST_DELAY);

  Serial.println ("LedsManager::Yellow");
  this->setAllColor(CRGB::Yellow);
  
  FastLED.delay(TEST_DELAY);

  Serial.println ("LedsManager::Violet");
  this->setAllColor(CRGB::Violet);
 
  FastLED.delay(TEST_DELAY);

  Serial.println ("LedsManager::White  - Check Power!");
  this->setAllColor(CRGB::White);
  
  FastLED.delay(TEST_DELAY);

  Serial.println("LedsManager::Show Time...");
  FastLED.clear();
  this->setAllColor(CRGB::Black);
  //FastLED.setBrightness(MAX_BRIGHTNESS);
}

    
