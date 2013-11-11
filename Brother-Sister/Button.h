//
//  Button.h
//  Brother-Sister
//
//  Created by Jordan Cortes Guzman on 11/10/13.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#ifndef __Button__
#define __Button__

#include "Block.h"
#include <list>

class Button
{
    float x, y, height, width; //button
    bool active;
    std::list<Block> doors; //doors
    
public:
    Button();
    Button(float, float, float, float);
    
public:
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    bool isActive();
    std::list<Block> getDoors();
    void setX(float);
    void setY(float);
    void setWidth(float);
    void setHeight(float);
    void setActive(bool);
    void addDoor(Block);
};

#endif /* defined(__Brother_Sister__Button__) */
