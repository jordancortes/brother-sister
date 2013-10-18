//
//  Wall.h
//  Brother-Sister
//
//  Created by Jordan Cortés Guzmán on 18/10/2013.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#ifndef __Wall__
#define __Wall__

class Wall
{
    float x, y, height, width;
    
public:
    Wall();
    Wall(float, float, float, float);
    
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

#endif /* defined(__Wall__) */
