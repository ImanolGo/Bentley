/*
 *  Brancher.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 16/52/19.
 *
 */



#include "Brancher.h"
#include "AppManager.h"



Brancher::Brancher( unsigned short _id): m_id(_id)
{
    //Intentionaly left empty
}

Brancher::~Brancher()
{
    //Intentionaly left empty
}



void Brancher::setPixels(const vector<ofFloatColor>& colors)
{
    for(int i=0; i< m_indexes.size(); i++){
        auto index = m_indexes[i];
        if(index<colors.size()){
            m_pixels[i].r = colors[index].r * 255;
            m_pixels[i].g = colors[index].g * 255;
            m_pixels[i].b = colors[index].b * 255;
        }
    }
}

void Brancher::addPixel(unsigned int index)
{
    m_indexes.push_back(index);
    m_pixels.push_back(ofColor(0));
}




