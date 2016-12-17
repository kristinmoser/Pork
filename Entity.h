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





class Entity {
public:
    Entity();
    Entity(Vector3 pos, Vector3 vel, Vector3 acc, Vector3 scale);
    Entity(Vector3 pos, Vector3 scale, GLuint texture);
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 scale;
    float gravity = -2.0f;
    float width = 4.0f;
    float height = 6.0f;
    
    float bottom = (position.y + width) * scale.y;
    float top = (position.y - width) * scale.y;
    float left = (position.x - width) * scale.x;
    float right = (position.x + width) * scale.x;
    
    float rotation;

    bool isStatic;
    int type;
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    GLuint texture;
    Matrix modelMatrix;
    float vertices[12] = {
        -2.0f, -3.0f, //BL
        2.0f, -3.0f, //BR
        2.0f, 3.0f, //TR
        -2.0f, -3.0f, //BL
        2.0f, 3.0f, //TR
        -2.0f, 3.0f //TL
    };

    float texCoords[12] = {
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        0.0, 0.0
    };
    
    void draw(ShaderProgram program);
    void update(float elapsed);
};
