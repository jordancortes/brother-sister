//
//  Character.h
//  Brother-Sister
//
//  Created by Jordan Cortés Guzmán on 14/10/2013.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#ifndef __Character__
#define __Character__

class Character
{
    float x, y, height, width;
    
public:
    Character();
    Character(float, float, float, float);

public:
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    void setX(float);
    void setY(float);
    void setWidth(float);
    void setHeight(float);
};

#endif /* defined(__Character__) */
