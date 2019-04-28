/*
 *  LedsManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */



#pragma once

#include "Manager.h"
#include "Led.h"

//========================== class LedsManager ==============================
//============================================================================
/** \class LedsManager LedsManager.h
 *	\brief Class managing the LEDs
 *	\details It controls the postion and color of the LEDs
 */


class LedsManager: public Manager
{
    static const string LEDS_FOLDER_PATH;
    
    public:
    
        typedef vector<int> IntVector;
        typedef vector< ofPtr<Led> > LedVector;

    
    public:

        //! Constructor
        LedsManager();

        //! Destructor
        ~LedsManager();

        //! Setup the Halo Manager
        void setup();

        //! Update the Led Manager
        void update();
    
        //! Draw the Led Manager
        void draw();
    
        const LedVector& getLeds() const {return m_leds;}
    
        const vector <ofFloatColor>& getColors() const {return m_colors;}
    
        void setPixels(ofPixelsRef pixels);
    
        const ofPoint getMin() const {return m_minPos;}
    
        const ofPoint getMax() const {return m_maxPos;}

        bool isNewFrame() const {return m_isNewFrame;}
    
        void setSize(float& value);
    
        void setLedBrightness(float& value){m_ledsBrightness = value;}
    
        void setLaserBrightness(float& value){m_laserBrightness = value;}
    
    
    private:
    
        void setupLeds();
    
        bool readLeds();
    
        void arrangeLeds();
    
        bool loadPair(string& pathTwoD, string& pathThreeD);
    
        bool addLedPair(string& pathTwoD, string& pathThreeD);
    
        void setupShader();
    
        void createLedPositions();
    
        void sortLeds();
    
        void normalizeLeds();
    
        void normalize3DLeds();
    
        void normalize2DLeds();
    
        void  centreLeds();
    
        void  centre3DLeds();
    
        void  centre2DLeds();
    
        bool loadSubfolder(ofDirectory& dir);
    
        void readLedsPositionFromGroup(const string& groupName, int& id, int numberOfSections);
    
        void readLasersPositionFromGroup(const string& groupName, int& id, vector<int>& sections);
    
        bool parseLedLine(string& line, ofPoint& position);
    
        void createLed(const ofPoint& position, int& id);
    
        void createLedPair(const ofPoint& position2D,const ofPoint& position3D);
    
        void removeCharsFromString( string &str, char* charsToRemove );
    
        bool isValidFile(const string& path);
    
        void setPixelColor(ofPixelsRef pixels, int index);
    
        void clearLeds();

    private:
    
        LedVector          m_leds;
        ofPoint            m_minPos;
        ofPoint            m_maxPos;
        bool               m_isNewFrame;
        bool               m_is3D;
        float              m_ledsBrightness;
        float              m_laserBrightness;
    
    
        ofVbo m_vbo3D;
        ofVbo m_vbo2D;
        ofShader m_shader;
        ofTexture m_texture;
    
        vector <ofVec3f> m_points3D;
        vector <ofVec3f> m_points2D;
        vector <ofVec3f> m_sizes;
        vector <ofFloatColor> m_colors;
        int         m_offset;
    
    
};



