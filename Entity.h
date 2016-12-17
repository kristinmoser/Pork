//
//  Entity.h
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//


#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Vector3.h"
#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif



enum EntityType {ENTITY_PLAYER, ENTITY_OUCH,
    ENTITY_NICE, ENTITY_PLATFORM};

class Entity {
public:
    Entity();
    Entity(Vector3 pos, Vector3 vel, Vector3 acc);
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float gravity = -2.0f;
    float bottom;
    float top;
    float left;
    float right;
    float rotation;
    
    bool isStatic;
    EntityType type;
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    GLuint texture;
};
