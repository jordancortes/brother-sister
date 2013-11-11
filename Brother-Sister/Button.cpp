//
//  Button.cpp
//  Brother-Sister
//
//  Created by Jordan Cortes Guzman on 11/10/13.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#include "Button.h"

Button::Button()
{
    Button::x = 0;
    Button::y = 0;
    Button::width = 0;
    Button::height = 0;
    Button::active = false;
}

Button::Button(float x, float y, float width, float height)
{
    Button::x = x;
    Button::y = y;
    Button::width = width;
    Button::height = height;
    Button::active = false;
}

float Button::getX()
{
    return Button::x;
}

float Button::getY()
{
    return Button::y;
}

float Button::getWidth()
{
    return Button::width;
}

float Button::getHeight()
{
    return Button::height;
}

bool Button::isActive()
{
    return Button::active;
}

std::list<Block> Button::getDoors()
{
    return doors;
}

void Button::setX(float x)
{
    Button::x = x;
}

void Button::setY(float y)
{
    Button::y = y;
}

void Button::setWidth(float width)
{
    Button::width = width;
}

void Button::setHeight(float height)
{
    Button::height = height;
}

void Button::setActive(bool active)
{
    Button::active = active;
}

void Button::addDoor(Block door)
{
    doors.push_back(door);
}