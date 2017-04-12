//
//  BabyGrassHopper.cpp
//  Bugs
//
//  Created by Shangyu Hsu on 2/18/17.
//  Copyright © 2017 David Smallberg. All rights reserved.
//

#include "BabyGrassHopper.hpp"
#include "StudentWorld.h"
#include <iostream>
using namespace std;


BabyGrassHopper::BabyGrassHopper(StudentWorld* s, int x, int y): Actor(s, IID_BABY_GRASSHOPPER, x, y, 1)
{
    setRandDir();
    m_hitpoints = 500;
    m_distance = randInt(2,10);
    m_dead = false;
    m_stunnedturns=0;
    
}

int BabyGrassHopper::moveInDirection()
{
    GraphObject::Direction d = getDirection();
    
    switch(d)
    {
        case GraphObject::up:
        {
            if(getStudentWorld()->getField()->getContentsOf(getX(), getY()+1)==Field::FieldItem::rock)
                return -1;
            else
                moveTo(getX(), getY()+1);
            break;
        }
        case GraphObject::down:
        {
            if(getStudentWorld()->getField()->getContentsOf(getX(), getY()-1)==Field::FieldItem::rock)
                return -1;
            else
            {
                moveTo(getX(), getY()-1);
                return 0;
            }
            break;
        }
        case GraphObject::left:
        {
            if(getStudentWorld()->getField()->getContentsOf(getX()-1, getY())==Field::FieldItem::rock)
                return -1;
            else
            {
                moveTo(getX()-1, getY());
                return 0;
            }
            break;
        }
        case GraphObject::right:
        {
            if(getStudentWorld()->getField()->getContentsOf(getX()+1, getY())==Field::FieldItem::rock)
                return -1;
            else
            {
                moveTo(getX()+1, getY());
                return 0;
            }
            break;
        }
        default:
        {
            break;
        }
           
    }
    return -1;
}

void BabyGrassHopper::doSomething()
{
    m_hitpoints--;
    if (m_hitpoints==0)
    {
        //food stuff
        m_dead = true;
        return;
    }
    if (m_stunnedturns>0)
    {
        m_stunnedturns--;
        return;
    }
    if (m_hitpoints>1600)
    {
        //adult grasshopper stuff
    }
    
    //eating stuff
    
    if(m_distance==0)
    {
        m_distance = randInt(2,10);
        setRandDir();
    
    }
    if(moveInDirection()==-1)
        m_distance=0;
    else
        m_distance--;
    m_stunnedturns=2;
    
}

    
   
        
    
    
    


