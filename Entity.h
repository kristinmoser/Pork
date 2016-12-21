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
#include "Matrix.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


enum EntityType {ENTITY_PLAYER, ENTITY_OUCH, ENTITY_NICE, ENTITY_PLATFORM};
enum PowerUP {NOT, ICE_CUBE};


class Entity {
public:
    Entity();
    Entity(Vector3 pos, Vector3 vel, Vector3 acc, float width, float height);
    Entity(Vector3 pos, GLuint texture, float width, float height, int type);
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float width;
    float height;
    
    float bottom;
    float top;
    float left;
    float right;
    
    float rotation;
    
    //bool isStatic;
    bool isHurt; // touched an OUCH entity and will explode when this is true in dramaticDeath()
    int type;
    bool winner;
    //bool isAlive = false;
    bool isInvincible = false;
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    float friction = 2.0;
    GLuint texture;
    Matrix modelMatrix;
    
    
    void draw(ShaderProgram program);
    void update(float elapsed);
    void collide(Entity* entity);
    void dramaticDeath(float elapsed);
    //bool collideX(Entity* entity);
    //bool collideY(Entity* entity);
};
