//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "SheetSprite.hpp"
#include "ShaderProgram.h"

SheetSprite::SheetSprite(){}


void SheetSprite::Draw(ShaderProgram program){
        
        glBindTexture(GL_TEXTURE_2D, texture);
        float texCoords[] = {
            u, v+height,
            u+width, v,
            u, v,
            u+width, v,
            u, v+height,
            u+width, v+height
        };
        
        float aspect = width / height;
        
        float vertices[] = {
            -0.5f * size * aspect, -2.0f * size, //BL
            0.5f * size * aspect, -1.0f * size, //TR
            -0.5f * size * aspect, -1.0f * size, //TL
            0.5f * size * aspect, -1.0f * size, //TR
            -0.5f * size * aspect, -2.0f * size , //BL
            0.5f * size * aspect, -2.0f * size //BR
        };
        
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
    }
