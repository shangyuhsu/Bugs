//
//  BabyGrassHopper.hpp
//  Bugs
//
//  Created by Shangyu Hsu on 2/18/17.
//  Copyright © 2017 David Smallberg. All rights reserved.
//

#ifndef BabyGrassHopper_hpp
#define BabyGrassHopper_hpp
#include "Actor.h"
class StudentWorld;

class BabyGrassHopper:public Actor{
public:
    BabyGrassHopper(StudentWorld* s, int x, int y);
    virtual void doSomething();
    bool isDead();
    int moveInDirection();
private:
    void setRandDir();
    int m_hitpoints;
    int m_stunnedturns;
    int m_distance;
    bool m_dead;
   
    
};

#endif /* BabyGrassHopper_hpp */
