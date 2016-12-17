//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//


#include "Vector3.h"
#include "SheetSprite.hpp"
#include "Entity.h"
#include <stdio.h>

Entity::Entity(){}
Entity::Entity(Vector3 pos, Vector3 vel, Vector3 acc):position(pos), velocity(vel), acceleration(acc){}
