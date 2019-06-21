/*
 *  Brancher.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 16/05/19.
 *
 */


#pragma once

#include "ofMain.h"





class Brancher {
    
    public:
    
        typedef enum  {
            TLC,
            DOTSTAR,
            AI,
            NEOPIXEL
        } LedType;
    
        Brancher(unsigned short _id);
    
        virtual ~Brancher();
    
        unsigned short getId() const {return m_id;}
    
        const vector<ofColor>& getPixels() const {return m_pixels;}
    
        void setPixels(const vector<ofFloatColor>& colors);
    
        void addPixel(unsigned int index);
    
        void addStemPixel(const string& _id, unsigned int index);
    
        int getSize() const {return m_pixels.size();}
    
        const vector<unsigned int>& getIndexes() const {return m_indexes;}
    
        bool getIndexFromStem(const string& _id, unsigned int index, unsigned int& returned_index);
    
        vector<string> getStemIds();
    
        LedType getLedType() const {return m_ledType;}
    
        void setLedType(LedType value) {m_ledType = value;}

    private:
            
        unsigned short m_id;
        vector<ofColor> m_pixels;
        vector<unsigned int> m_indexes;
        map<string, vector<int>>   m_stems;
        LedType                    m_ledType;

};







