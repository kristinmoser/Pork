//
//  SheetSprite.hpp
//  NYUCodebase
//
//  Created by Kristin Moser on 12/16/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp
#include "ShaderProgram.h"

#include <stdio.h>

#endif /* SheetSprite_hpp */

class SheetSprite {
public:
    SheetSprite();
    SheetSprite(GLuint texture, float u, float v, float width, float height, float size);
    void Draw(ShaderProgram program);
    float size;
    GLuint texture;
    float u;
    float v;
    float width;
    float height;
};

