//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

#include <stdio.h>

Entity::Entity(): position(Vector3(0,0,0)), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)), isStatic(false){}

Entity::Entity(Vector3 pos, GLuint texture, float width, float height) :position(pos), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)), width(width), height(height), texture(texture), isStatic(true){}

Entity::Entity(Vector3 pos, Vector3 vel, Vector3 acc, float width, float height) :position(pos), velocity(vel), acceleration(acc), isStatic(false), width(width), height(height){}

void Entity::draw(ShaderProgram program){

    modelMatrix.identity();
        program.setModelMatrix(modelMatrix);
    modelMatrix.Translate(position.x, position.y, position.z);

    float half_width = width / 2.0f;
    float half_height = height / 2.0f;
    float vertices[] =
    {
        position.x - half_width, position.y - half_height,
        position.x + half_width, position.y + half_height,
        position.x - half_width, position.y + half_height,
        position.x - half_width, position.y - half_height,
        position.x + half_width, position.y - half_height,
        position.x + half_width, position.y + half_height
    };
    float texCoords[] = {
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        0.0, 0.0
    };
    glBindTexture(GL_TEXTURE_2D, texture);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

void Entity::update(float elapsed){
    bottom = position.y - height/2;
    top = position.y + height/2;
    left = position.x - width/2;
    right = position.x + width/2;

    velocity.x += acceleration.x * elapsed;
    velocity.y += acceleration.y * elapsed;
    velocity.z += acceleration.z * elapsed;
    
    position.x += velocity.x * elapsed;
    position.y += velocity.y * elapsed;
    position.z += velocity.z * elapsed;
    
    
}

bool Entity::collidedWith(Entity* entity){
    if (!(bottom > entity->top ||  top < entity->bottom || left > entity->right || right < entity->left)){
        std::cout << "collision" << std::endl;
        collidedBottom = true;
         return true;
    }
    return false;
   
}
