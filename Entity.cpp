//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

#include <stdio.h>

Entity::Entity(): position(Vector3(0,0,0)), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)), isStatic(false), scale(Vector3(1.0, 1.0, 1.0)){

}

Entity::Entity(Vector3 pos, Vector3 scale, GLuint texture) :position(pos), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)), texture(texture),scale(scale), isStatic(true){}

Entity::Entity(Vector3 pos, Vector3 vel, Vector3 acc, Vector3 scale) :position(pos), velocity(vel), acceleration(acc), scale(scale), isStatic(false){}

void Entity::draw(ShaderProgram program){
    program.setModelMatrix(modelMatrix);
    modelMatrix.identity();
    modelMatrix.Translate(position.x, position.y, position.z);
    modelMatrix.Scale(scale.x, scale.y, scale.z);


    glBindTexture(GL_TEXTURE_2D, texture);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Entity::update(float elapsed){
    std::cout << "POSITION:" << position.y << std::endl;
    bottom = position.y - height/2;
    top = position.y + height/2;
    left = position.x - width/2;
    right = position.x + width/2;

    velocity.x += acceleration.x * elapsed;
    velocity.y -= acceleration.y * elapsed;
    velocity.z += acceleration.z * elapsed;
    
    position.x += velocity.x * elapsed;
    position.y += velocity.y * elapsed;
    position.z += velocity.z * elapsed;
    
    
}
