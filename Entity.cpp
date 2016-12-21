//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

#include <stdio.h>

float lerp(float v0, float v1, float t) {
    return (1.0f - t) * v0 + t * v1;
}

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
    float retVal = dstMin + ((value - srcMin)/(srcMax-srcMin) * (dstMax-dstMin));
    if(retVal < dstMin) {
        retVal = dstMin;
    }
    if(retVal > dstMax) {
        retVal = dstMax;
    }
    return retVal;
}

Entity::Entity(): position(Vector3(0,0,0)), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)){}

Entity::Entity(Vector3 pos, GLuint texture, float width, float height, int type) :position(pos), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)), width(width), height(height), texture(texture), type(type){}

Entity::Entity(Vector3 pos, Vector3 vel, Vector3 acc, float width, float height) :position(pos), velocity(vel), acceleration(acc), width(width), height(height){}

void Entity::draw(ShaderProgram program){
    
    modelMatrix.identity();
    program.setModelMatrix(modelMatrix);
    modelMatrix.Translate(position.x, position.y, position.z);
    
    float half_width = width / 2.0f;
    float half_height = height / 2.0f;
    float vertices[] =
    {
        position.x - half_width, position.y - half_height,
        position.x + half_width, position.y - half_height,
        position.x + half_width, position.y + half_height,

        

        position.x - half_width, position.y - half_height,
        position.x + half_width, position.y + half_height,
        position.x - half_width, position.y + half_height



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
    
    velocity.x = lerp(velocity.x, 0.0f, elapsed * friction);
    velocity.y += acceleration.y * elapsed;
    velocity.z += acceleration.z * elapsed;
    
    position.x += velocity.x * elapsed;
    position.y += velocity.y * elapsed;
    position.z += velocity.z * elapsed;
    
}

//checks collisions and if there is
//applies penetration in appropriate direction
void Entity::collide(Entity* entity){
    if (!(bottom > entity->top ||  top < entity->bottom || left > entity->right || right < entity->left)){
        if (entity->type == ENTITY_PLATFORM){
            /*
            if (top > entity->bottom) {
                //its top is colliding with entity
                collidedTop = true;
                float penetration = fabsf((entity->position.y -position.y) - (entity->height/2) - height/2);
                position.y -= penetration + 0.0003;
                velocity.y = 0 ;
                
            }
             */
           if (bottom < entity->top) {
                //its bottom is colliding with entity
                collidedBottom = true;
                float penetration = fabsf((position.y - entity->position.y) - (entity->height/2) - height/2);
                position.y += penetration + .0003;
                velocity.y *= -0.7;
            }

            /*if (left > entity->right) {
                //its left is colliding with entity
                collidedLeft = true;
                float penetration = fabsf((position.x - entity->position.x) - (entity->width/2) - width/2);
                position.x += penetration + .0003;
                velocity.x = 0;
            }
            if (right < entity->left) {
                //its right is colliding with entity
                collidedRight = true;
                float penetration = fabsf((position.x - entity->position.x) - (entity->width/2) - width/2);
                position.x -= penetration + .0003;
                velocity.x = 0;
            }*/
        }
        if (entity->type == ENTITY_NICE){
                winner = true;
                entity->position.x = 100;
            }
        }
        /*if (entity->type == ENTITY_OUCH){
            std::cout << "OUCHHHHHHHHHH" <<std::endl;
            if (!isInvincible){
                isHurt = true;
                std::cout << "THAT HURTS!!!!!!!" <<std::endl;
            }
        }*/
    }



