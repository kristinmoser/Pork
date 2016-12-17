#include <stdio.h>
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Entity.h"
#include "SheetSprite.hpp"
#include <vector>


#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

SDL_Window* displayWindow;

bool done = false;
enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
enum EntityType {ENTITY_PLAYER, ENTITY_OUCH, ENTITY_NICE, ENTITY_PLATFORM};
enum JumpState {JUMPING_UP, JUMPING_DOWN, STILL };
int jstate = STILL;
int state = STATE_GAME_LEVEL;
bool moveLeft = false;
bool moveRight = false;
bool jump = false;
float lastFrameTicks = 0.0f;
float fixedElapsed;
float ticks;
float elapsed;
GLuint background;
GLuint font;


Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;


GLuint LoadTexture(const char * image_path) {
    SDL_Surface* surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void draw(GLuint texture, float vertices[], ShaderProgram program, float texCoords[]){
    glBindTexture(GL_TEXTURE_2D, texture);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}


std::vector<Entity *> entities;
Entity * player = new Entity(Vector3(0,0,0), Vector3(0,1.0,0), Vector3(0,6.0,0), Vector3(1.0, 1.0, 1.0));

std::vector<Entity *> platforms;




ShaderProgram Setup(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("two chunks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 640, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 360, 640);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    font = LoadTexture("pixel_font.png");
    background = LoadTexture("background.png");
    GLuint concrete = LoadTexture("concrete.png");
    player->texture = background;
    std::cout << player->collidedBottom << std::endl;
    player->scale.x = 0.15f;
    player->scale.y = 0.1f;
    entities.push_back(player);
    
    platforms.push_back(new Entity(Vector3(0.0,-10.0,0.0), Vector3(2.0,0.1,1.0), concrete)); // floor
    platforms.push_back(new Entity(Vector3(-2.5,-8.0,0.0), Vector3(0.7,0.065,1.0), concrete));
    platforms.push_back(new Entity(Vector3(1.0,-6.9,0.0), Vector3(0.4,0.065,1.0), concrete));
    platforms.push_back(new Entity(Vector3(3.0,-4.95,0.0), Vector3(0.8,0.065,1.0), concrete)); // spike this
    platforms.push_back(new Entity(Vector3(-1.9,-4.3,0.0), Vector3(0.5,0.065,1.0), concrete));
    platforms.push_back(new Entity(Vector3(2.1,-2.4,0.0), Vector3(0.86,0.065,1.0), concrete));
    platforms.push_back(new Entity(Vector3(-2.6,-1.4,0.0), Vector3(0.7,0.065,1.0), concrete)); // spikes on left side
    platforms.push_back(new Entity(Vector3(1.8, 0.6,0.0), Vector3(1.35,0.065,1.0), concrete)); // spike on center
    platforms.push_back(new Entity(Vector3(-2.2, 2.4,0.0), Vector3(0.7,0.065,1.0), concrete));
    platforms.push_back(new Entity(Vector3(-0.3,3.7,0.0), Vector3(0.3,0.065,1.0), concrete)); // spike under
    platforms.push_back(new Entity(Vector3(3.0,4.0,0.0), Vector3(0.4,0.065,1.0), concrete));
    platforms.push_back(new Entity(Vector3(-3.3,4.9,0.0), Vector3(0.5,0.065,1.0), concrete));
    platforms.push_back(new Entity(Vector3(2.1,5.8,0.0), Vector3(0.6,0.065,1.0), concrete)); // spike under
    
    
    projectionMatrix.setOrthoProjection(-5.0, 5.0, -10.0f, 10.0f, -1.0f, 1.0f);
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glUseProgram(program.programID);
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);
    return program;
}


float lerp(float v0, float v1, float t) {
    return (1.0f - t) * v0 + t * v1;
}


void ProcessMainMenu(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
            
        }
        else if (event.type == SDL_KEYDOWN){
            state = STATE_GAME_LEVEL;
        }
    }
}

void ProcessGame(SDL_Event event) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                    
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                    
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_UP){

                    if (jstate == STILL){
                        jstate = JUMPING_UP;
                    }
                    
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
                    
                }
            }
        }
}

void ProcessGameOver(SDL_Event event){
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
            
        }
    }
}

void ProcessEvents() {
    SDL_Event event;
    switch (state) {
        case STATE_MAIN_MENU:
            ProcessMainMenu(event);
            break;
        case STATE_GAME_LEVEL:
            ProcessGame(event);
            break;
        case STATE_GAME_OVER:
            ProcessGameOver(event);
            break;
        default:
            break;
    }
}


void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
    float texture_size = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            (float(size+spacing) * i) + (-0.5f * size), 0.5f * size,
            (float(size+spacing) * i) + (-0.5f * size), -0.5f * size,
            (float(size+spacing) * i) + (0.5f * size), 0.5f * size,
            (float(size+spacing) * i) + (0.5f * size), -0.5f * size,
            (float(size+spacing) * i) + (0.5f * size), 0.5f * size,
            (float(size+spacing) * i) + (-0.5f * size), -0.5f * size
        });
        
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        });
    }
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}



void RenderMainMenu(ShaderProgram program){
    // for all main menu elements
    // setup transforms, render sprites

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

void RenderGameLevel(ShaderProgram program){
    // for all game elements
    // setup transforms, render sprites
    program.setModelMatrix(player->modelMatrix);
    player->modelMatrix.identity();
    player->modelMatrix.Scale(player->scale.x, player->scale.y, player->scale.z);
    player->modelMatrix.Translate(player->position.x, player->position.y, player->position.z);
    player->draw(program);
    for (Entity* platform : platforms) {
        platform->draw(program);
    }

    
    
}

void RenderGameOver(ShaderProgram program){

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}
void Render(ShaderProgram program) {
    //std::cout << "Entered Render" << "\n";
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    switch(state) {
        case STATE_MAIN_MENU:
            RenderMainMenu(program);
            break;
        case STATE_GAME_LEVEL:
            RenderGameLevel(program);
            break;
        case STATE_GAME_OVER:
            RenderGameOver(program);
            break;
    }
}

void UpdateMainMenu(){
// move stuff and check for collisions
}


void UpdateGameLevel(float elapsed){
// move stuff and check for collisions
//    for (int i = 0; i < entities.size(); i ++){
//        entities[i]->update(elapsed);
//    }
    if (jstate == JUMPING_UP){
        player->velocity.y = 15.0f;
        jstate = JUMPING_DOWN;
        
    }
    if (player->velocity.y < -15){
        jstate = STILL;
    }
    for (unsigned int i = 0; i < entities.size(); i++) {
        for (unsigned int j = 0; j < platforms.size(); j++) {
            if (entities[j]->y >= mobs[i]->y - entities[j]->height / 2 - mobs[i]->height && entities[j]->x + entities[j]->width / 2 < mobs[i]->x + mobs[i]->width / 2 && entities[j]->x - entities[j]->width / 2 > mobs[i]->x - mobs[i]->width / 2) {

                for (unsigned int k = 0; k < platforms.size(); k++) platforms[k]->velocity += 0.007f;
            }
            break;
        }
    }
    player->velocity.y -= player->acceleration.y * elapsed;
    player->position.x += player->velocity.x * elapsed;
    player->position.y += player->velocity.y * elapsed;


    
}



void Update() {
    // move stuff and check for collisions
    ticks = (float) SDL_GetTicks()/1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    fixedElapsed = elapsed;

    switch(state) {
        case STATE_MAIN_MENU:
            //UpdateMainMenu();
            break;
        case STATE_GAME_LEVEL:
            if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
                fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
            }
            while (fixedElapsed >= FIXED_TIMESTEP) {
                fixedElapsed -= FIXED_TIMESTEP;
                UpdateGameLevel(FIXED_TIMESTEP);
            }
            UpdateGameLevel(fixedElapsed);
            break;
    }
}



void Cleanup() {
    SDL_Quit();
    
}


int main() {
   
    ShaderProgram prog = Setup();
    while(!done) {
        ProcessEvents();
        Update();
        Render(prog);
        SDL_GL_SwapWindow(displayWindow);
    }
    Cleanup();
    return 0;
}

