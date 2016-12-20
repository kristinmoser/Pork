#include <stdio.h>
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
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
enum EntityType {ENTITY_player1, ENTITY_OUCH, ENTITY_NICE, ENTITY_PLATFORM};
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

std::vector<Entity *> entities;
Entity * player1 = new Entity(Vector3(0,0,0), Vector3(0,0.0,0), Vector3(0,-9.0,0), 1.0, 1.0);
Entity * player2 = new Entity(Vector3(0,-2,0), Vector3(0,0.0,0), Vector3(0,-9.0,0), 1.0, 1.0);
Entity * icePower = new Entity(Vector3(0,-2,0), Vector3(0,0.0,0), Vector3(0,-9.0,0), 1.0, 1.0);

std::vector<Entity *> platforms;

ShaderProgram Setup(){
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("two chunks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 640, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 480, 640);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    projectionMatrix.setOrthoProjection(-5.0, 5.0, -10.0f, 10.0f, -1.0f, 1.0f);
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glUseProgram(program.programID);
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);
    
    
    font = LoadTexture("pixel_font.png");
    background = LoadTexture("background.png");
    GLuint concrete = LoadTexture("concrete.png");
    player1->texture = background;
    player2->texture = background;
    
    entities.push_back(player1);
    entities.push_back(player2);
    
    
    platforms.push_back(new Entity(Vector3(0.0,-9.7,0.0),concrete, 6.0, 0.5)); // floor
    platforms.push_back(new Entity(Vector3(-2.5,-7.4,0.0),concrete, 2.0, 0.5));
    platforms.push_back(new Entity(Vector3(1.0,-6.0,0.0), concrete, 2.4, 0.5));
    platforms.push_back(new Entity(Vector3(3.0,-4.3,0.0),concrete, 2.8, 0.5)); // spike this
    platforms.push_back(new Entity(Vector3(-1.9,-2.3,0.0),concrete, 2.5, 0.5));
    platforms.push_back(new Entity(Vector3(2.1,-2.0,0.0), concrete, 2.86,0.5));
    platforms.push_back(new Entity(Vector3(-2.6,-0.2,0.0),concrete, 2.7, 0.5)); // spikes on left side
    platforms.push_back(new Entity(Vector3(1.8, 0.3,0.0), concrete, 2.35,0.5)); // spike on center
    platforms.push_back(new Entity(Vector3(-2.2, 3.4,0.0),concrete, 2.7, 0.5));
    platforms.push_back(new Entity(Vector3(-0.3,5.7,0.0), concrete, 2.3, 0.5)); // spike under
    platforms.push_back(new Entity(Vector3(3.0,7.0,0.0),  concrete, 2.4, 0.5));
    platforms.push_back(new Entity(Vector3(-3.3,9.9,0.0), concrete, 2.5, 0.5));
    platforms.push_back(new Entity(Vector3(2.1,10.0,0.0),  concrete, 2.6, 0.5)); // spike under
    
    return program;
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
            //player1 controls
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                player1->velocity.x = -4.0f;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                player1->velocity.x = 4.0f;
                
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                player1->velocity.y = 9.0f;
                Mix_Chunk* jumpSound;
                jumpSound = Mix_LoadWAV("jump.wav");
                Mix_PlayChannel( -1, jumpSound, 0);
                
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
                player1->velocity.y = -5.0f;
                
            }
            //player2
            if (event.key.keysym.scancode == SDL_SCANCODE_A){
                player2->velocity.x = -4.0f;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_D){
                player2->velocity.x = 4.0f;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_W){
                player2->velocity.y = 9.0f;
                Mix_Chunk* jumpSound2;
                jumpSound2 = Mix_LoadWAV("jump2.wav");
                Mix_PlayChannel( -1, jumpSound2, 0);

            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S){
                player2->velocity.y = -5.0f;
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

void drawLine(ShaderProgram program){
    
    modelMatrix.identity();
    program.setModelMatrix(modelMatrix);
    
    float vertices[] =
    {
        player1->position.x, player1->position.y,
        player2->position.x, player2->position.y
    };
    float texCoords[] = {
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        0.0, 0.0
    };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

void RenderGameLevel(ShaderProgram program){
    // for all game elements
    // setup transforms, render sprites
    player1->draw(program);
    player2->draw(program);
    for (int i = 0 ; i < platforms.size(); ++i) {
        platforms[i]->draw(program);
    }
    drawLine(program);
    
    
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


void UpdateGameLevel(ShaderProgram& program, float elapsed){
    for ( int j = 0; j < platforms.size(); ++j){
        for (int k = 0; k < entities.size(); ++k){
            entities[k]->collide(platforms[j]);
        }
    }
    
    for (Entity * player : entities){
        player->update(elapsed);
    }
    
    for (Entity * things : platforms){
        things->update(elapsed);
    }
    
    viewMatrix.identity();
    viewMatrix.Translate(-player1->position.x, -player1->position.y, 0);
    program.setViewMatrix(viewMatrix);
    
    
}



void Update(ShaderProgram program) {
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
            UpdateGameLevel(program, FIXED_TIMESTEP);
        }
        UpdateGameLevel(program, fixedElapsed);
        break;
    }
}



void Cleanup() {

    SDL_Quit();
}


int main() {
    
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    Mix_Chunk* bgMusic;
    bgMusic = Mix_LoadWAV("gamefinalBG.wav");
    //song from https://soundcloud.com/missingsoul/8i09vwvryuvp
    Mix_PlayChannel( -1, bgMusic, 0);
    
    ShaderProgram prog = Setup();
    while(!done) {
        ProcessEvents();
        Update(prog);
        Render(prog);
        SDL_GL_SwapWindow(displayWindow);
    }
    Mix_FreeChunk(bgMusic);
    //need to free jump chunks too
    Cleanup();
    return 0;
}

