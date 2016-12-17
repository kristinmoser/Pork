//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

#include <stdio.h>

Entity::Entity(): position(Vector3(0,0,0)), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)), isStatic(true){}

Entity::Entity(Vector3 pos, Vector3 vel, Vector3 acc):position(pos), velocity(vel), acceleration(acc){}
