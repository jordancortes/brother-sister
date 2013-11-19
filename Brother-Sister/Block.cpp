//
//  Block.cpp
//  Brother-Sister
//
//  Created by Jordan Cortes Guzman on 11/10/13.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#include "Block.h"

/*
 float x, y, height, width;
 int side;
 float repeat_x, repeat_y;
 bool hide; //should hide when a door is hide (spikes)
 
 Block(float, float, float, float);
 Block(float, float, float, float, int);
 Block(float, float, float, float, int, bool);
 Block(float, float, float, float, int, float, float);
 */

Block::Block()
{
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    side = 1;
    repeat_x = 1.0;
    repeat_y = 1.0;
    hide = false;
}

Block::Block(float x, float y, float width, float height)
{
    Block::x = x;
    Block::y = y;
    Block::width = width;
    Block::height = height;
    side = 1;
    repeat_x = 1.0;
    repeat_y = 1.0;
    hide = false;
}

Block::Block(float x, float y, float width, float height, int side)
{
    Block::x = x;
    Block::y = y;
    Block::width = width;
    Block::height = height;
    Block::side = side;
    repeat_x = 1.0;
    repeat_y = 1.0;
    hide = false;
}

Block::Block(float x, float y, float width, float height, int side, bool hide)
{
    Block::x = x;
    Block::y = y;
    Block::width = width;
    Block::height = height;
    Block::side = side;
    repeat_x = 1.0;
    repeat_y = 1.0;
    Block::hide = hide;
}

Block::Block(float x, float y, float width, float height, int side, float repeat_x, float repeat_y)
{
    Block::x = x;
    Block::y = y;
    Block::width = width;
    Block::height = height;
    Block::side = side;
    Block::repeat_x = repeat_x;
    Block::repeat_y = repeat_y;
    hide = false;
}

float Block::getX()
{
    return Block::x;
}

float Block::getY()
{
    return Block::y;
}

float Block::getWidth()
{
    return Block::width;
}

float Block::getHeight()
{
    return Block::height;
}

int Block::getSide()
{
    return Block::side;
}

float Block::getRepeatX()
{
    return Block::repeat_x;
}

float Block::getRepeatY()
{
    return Block::repeat_y;
}

bool Block::getHide()
{
    return Block::hide;
}

void Block::setX(float x)
{
    Block::x = x;
}

void Block::setY(float y)
{
    Block::y = y;
}

void Block::setWidth(float width)
{
    Block::width = width;
}

void Block::setHeight(float height)
{
    Block::height = height;
}

void Block::setSide(int side)
{
    Block::side = side;
}

void Block::setRepeatX(float repeat_x)
{
    Block::repeat_x = repeat_x;
}

void Block::setRepeatY(float repeat_y)
{
    Block::repeat_y = repeat_y;
}

void Block::setHide(bool hide)
{
    Block::hide = hide;
}