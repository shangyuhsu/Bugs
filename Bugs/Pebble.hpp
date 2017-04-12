//
//  Pebble.hpp
//  Bugs
//
//  Created by Shangyu Hsu on 2/18/17.
//  Copyright © 2017 David Smallberg. All rights reserved.
//

#ifndef Pebble_hpp
#define Pebble_hpp

#include "Actor.h"
class StudentWorld;

class Pebble:public Actor{
public:
    Pebble(StudentWorld* s, int startX, int startY);
                                                                  
    virtual void doSomething();
};


#endif /* Pebble_hpp */
