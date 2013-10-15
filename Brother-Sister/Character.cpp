//
//  Character.cpp
//  Brother-Sister
//
//  Created by Jordan Cortés Guzmán on 14/10/2013.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#include "Character.h"

Character::Character()
{
    x = 0;
    y = 0;
}

Character::Character(float x, float y, float width, float height)
{
    Character::x = x;
    Character::y = y;
    Character::width = width;
    Character::height = height;
}

float Character::getX()
{
    return Character::x;
}

float Character::getY()
{
    return Character::y;
}

float Character::getWidth()
{
    return Character::width;
}

float Character::getHeight()
{
    return Character::height;
}

void Character::setX(float x)
{
    Character::x = x;
}

void Character::setY(float y)
{
    Character::y = y;
}

void Character::setWidth(float width)
{
    Character::width = width;
}

void Character::setHeight(float height)
{
    Character::height = height;
}