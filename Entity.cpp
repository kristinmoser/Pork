//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//


#include "Vector3.h"
#include <stdio.h>
class Entity{
public:
    
    Entity() : position(Vector3(0,0,0)), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)){}
    Entity(Vector3 pos, Vector3 vel, Vector3 acc): position(pos), velocity(vel), acceleration(acc){}
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float gravity = -2.0f;
    void calcBTLR(){
        bottom = position.y - (sprite.height/2);
        top = position.y + (sprite.height/2);
        left = position.x - (sprite.width / 2);
        right = position.x + (sprite.width/2);
    }
    float bottom;
    float top;
    float left;
    float right;
    float rotation;
    SheetSprite sprite;
};
