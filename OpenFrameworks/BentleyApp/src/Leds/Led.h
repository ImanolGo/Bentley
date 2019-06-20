/*
 *  Led.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */


#pragma once

#include "ofMain.h"
#include "BasicVisual.h"


class Led: public BasicVisual {
    
    static const float SIZE;
    
    public:
    
        Led(const ofPoint& position, int id);
    
        virtual ~Led();
    
        //bool operator<(const Led &l1, const Led &l2);
    
        //bool operator<(const Led &led) const { return this->getId() < led.getId(); }
    
       // bool operator<(const Led &led) const;
    
        void draw();
    
        void draw(int width, int height);
    
        int getId() const {return m_id;}
    
        void normalize(const ofRectangle& boundingBox);
    
        void setPixelColor(ofPixelsRef pixels, bool is3D);
    

    private:
            
        int m_id;

};







