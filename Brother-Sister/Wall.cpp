//
//  Wall.cpp
//  Brother-Sister
//
//  Created by Jordan Cortés Guzmán on 18/10/2013.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#include "Wall.h"

Wall::Wall()
{
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

Wall::Wall(float x, float y, float width, float height)
{
    Wall::x = x;
    Wall::y = y;
    Wall::width = width;
    Wall::height = height;
}

float Wall::getX()
{
    return Wall::x;
}

float Wall::getY()
{
    return Wall::y;
}

float Wall::getWidth()
{
    return Wall::width;
}

float Wall::getHeight()
{
    return Wall::height;
}

void Wall::setX(float x)
{
    Wall::x = x;
}

void Wall::setY(float y)
{
    Wall::y = y;
}

void Wall::setWidth(float width)
{
    Wall::width = width;
}

void Wall::setHeight(float height)
{
    Wall::height = height;
}