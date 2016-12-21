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
enum GameState { STATE_MAIN_MENU, STATE_LEVEL1, STATE_LEVEL2, STATE_LEVEL3, STATE_GAME_OVER, STATE_WIN };
int state = STATE_MAIN_MENU;
bool moveLeft = false;
bool moveRight = false;
bool jump = false;
float lastFrameTicks = 0.0f;
float fixedElapsed;
float ticks;
float elapsed;
float screenShakeTime = 0.0f;
float screenShakeSpeed = 10.0;
float screenShakeIntensity = 4.0f;
GLuint background;
GLuint font;
Entity * winner;


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
Entity * player1 = new Entity(Vector3(2,-5,0), Vector3(0,0.0,0), Vector3(0,-9.0,0), 1.0, 1.0);
Entity * player2 = new Entity(Vector3(-3,-5,0), Vector3(0,0.0,0), Vector3(0,-9.0,0), 1.0, 1.0);


std::vector<Entity *> statics1;
std::vector<Entity *> statics2;
std::vector<Entity *> statics3;

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
    GLuint greenSquare = LoadTexture("greenSquare.png");
    GLuint purpleSquare = LoadTexture("purpleSquare.png");
    GLuint concrete = LoadTexture("concrete.png");
    GLuint spikeText = LoadTexture("spike.png");
    GLuint fire = LoadTexture("fire_platform.png");
    GLuint ice = LoadTexture("ice.png");
    GLuint winnertext = LoadTexture("cppFile.png");
    player1->texture = greenSquare;
    player2->texture = purpleSquare;
    

    //powerUp->powerUptype = ICE_CUBE;
    entities.push_back(player1);
    entities.push_back(player2);
    
    winner = new Entity(Vector3(2, 35, 0), winnertext, 1.0f, 1.0f, ENTITY_NICE);
  
    
    float MIN_X = -3.5;
    float MAX_X = 3.5;
    float rangeX = MAX_X - MIN_X;

    
    for (int i= -5; i<30; i++) {
        float randomX = (rangeX * ((rand() / (float) RAND_MAX)))+ MIN_X;
        statics1.push_back(new Entity(Vector3(randomX, i * 3, 0.0), concrete, 3.0, 0.75, ENTITY_PLATFORM));
    }
    
    for (int i= -5; i<30; i++) {
        float randomX = (rangeX * ((rand() / (float) RAND_MAX)))+ MIN_X;
        statics2.push_back(new Entity(Vector3(randomX, i * 3, 0.0), fire, 3.0, 0.75, ENTITY_PLATFORM));
    }
    for (int i= -5; i<30; i++) {
        float randomX = (rangeX * ((rand() / (float) RAND_MAX)))+ MIN_X;
        statics3.push_back(new Entity(Vector3(randomX, i * 3, 0.0), ice, 3.0, 0.75, ENTITY_PLATFORM));
    }

    statics1.push_back(winner);
    statics2.push_back(winner);
    statics3.push_back(winner);
    
    statics1.push_back(new Entity(Vector3(3.0, -3.7, 0), spikeText, 1.0f, 0.6f, ENTITY_OUCH)); // spikes
    statics1.push_back(new Entity(Vector3(-2.8, 0.4, 0), spikeText, 1.0f, 0.6f, ENTITY_OUCH)); // spikes
    //statics.push_back(new Entity(Vector3(-0.3,6.1, 0), spikeText, 1.0f, 0.6f, ENTITY_OUCH, true)); // spikes
    
    
    return program;
}

void ProcessMainMenu(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
            
        }
        else if (event.type == SDL_KEYDOWN){
            state = STATE_LEVEL1;
        }
    }
}

void ProcessGame(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_Q)){
            done = true;
        }
        else if (event.type == SDL_KEYDOWN){
            //player1 controls
            //if (!player1->isAlive || !player2->isAlive){
              //  state = STATE_GAME_OVER;
            //}
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
            if (event.key.keysym.scancode == SDL_SCANCODE_E){
                player1->position = Vector3(0.0, 0.5, 1.0);
                player2->position = Vector3(0.0, 0.5, 1.0);
                state = STATE_LEVEL2;
                
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_R){
                player1->position = Vector3(0.0, 0.5, 1.0);
                player2->position = Vector3(0.0, 0.5, 1.0);
                state = STATE_LEVEL3;
                
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
        case STATE_LEVEL1:
            ProcessGame(event);
            break;
        case STATE_LEVEL2:
            ProcessGame(event);
            break;
        case STATE_LEVEL3:
            ProcessGame(event);
            break;
        case STATE_GAME_OVER:
            ProcessGameOver(event);
            break;
        case STATE_WIN:
            ProcessGame(event);
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

void draw(GLuint texture, float vertices[], ShaderProgram program, float texCoords[]){
    glBindTexture(GL_TEXTURE_2D, texture);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}

void RenderMainMenu(ShaderProgram program){
    // for all main menu elements
    // setup transforms, render sprites
    glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    modelMatrix.identity();
    modelMatrix.Translate(-2.5, 0.0, 0.0);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, font, "just 2 chunks", 0.55f, 0.0f);

    modelMatrix.identity();
    modelMatrix.Translate(-3.0, -0.5, 0.0);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, font, "Press ANY key to start", 0.3f, 0.0f);
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

void RenderGameLevel1(ShaderProgram program){
    // for all game elements
    // setup transforms, render sprites
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Entity * item: entities){
            item->draw(program);
    }
    for (int i = 0 ; i < statics1.size(); ++i) {
            statics1[i]->draw(program);
        
    }
    drawLine(program);
    
    
}
void RenderGameLevel2(ShaderProgram program){
    // for all game elements
    // setup transforms, render sprites
    glClearColor(0.2f, 0.0f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (Entity * item: entities){
        item->draw(program);
    }
    for (int i = 0 ; i < statics2.size(); ++i) {
        statics2[i]->draw(program);
        
    }
    drawLine(program);
    
    
}

void RenderGameLevel3(ShaderProgram program){
    // for all game elements
    // setup transforms, render sprites
   //glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (Entity * item: entities){
        item->draw(program);
    }
    for (int i = 0 ; i < statics3.size(); ++i) {
        statics3[i]->draw(program);
        
    }
    drawLine(program);
    
    
}

void RenderGameOver(ShaderProgram program){
    glClearColor(0.3f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    modelMatrix.identity();
    modelMatrix.Translate(0, -2.0, 0.0);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, font, "GAME OVER", .75f, 0.0f);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

void RenderWin(ShaderProgram program){
    modelMatrix.identity();
    modelMatrix.Translate(-3.5, -.5, 0.0);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, font, "WIN", 0.3f, 0.0f);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

void Render(ShaderProgram program) {
    //std::cout << "Entered Render" << "\n";
    
    switch(state) {
        case STATE_MAIN_MENU:
            RenderMainMenu(program);
            break;
        case STATE_LEVEL1:
            RenderGameLevel1(program);
            break;
        case STATE_LEVEL2:
            RenderGameLevel2(program);
            break;
        case STATE_LEVEL3:
            RenderGameLevel3(program);
            break;
        case STATE_GAME_OVER:
            RenderGameOver(program);
            break;
        case STATE_WIN:
            RenderWin(program);
    }
    
}

void UpdateMainMenu(){
    // move stuff and check for collisions
}


void UpdateGameLevel1(ShaderProgram& program, float elapsed){
    for ( int j = 0; j < statics1.size(); ++j){
        for (int k = 0; k < entities.size(); ++k){
            entities[k]->collide(statics1[j]);
            if (entities[k]->winner){
                entities[k]->winner = false;
                winner->position.x = 0;
                state = STATE_LEVEL2;
            }
        }
    }
    
    for (Entity * player : entities){
        player->update(elapsed);
    }
    
    for (Entity * things : statics1){
            things->update(elapsed);
    }
    std::cout << fabsf(viewMatrix.m[3][1]) - 8 << std::endl;
    std::cout << "PLAYER: " << player1->position.y << std::endl;
    
    if (player1->position.y < (fabsf(viewMatrix.m[3][1]) -12 ) || player2->position.y < (fabsf(viewMatrix.m[3][1]) - 12)){
        state = STATE_GAME_OVER;
    }

        viewMatrix.Translate(0, 1.5*(-elapsed), 0);
        program.setViewMatrix(viewMatrix);
    

}
void UpdateGameLevel2(ShaderProgram& program, float elapsed){
    for ( int j = 0; j < statics2.size(); ++j){
        for (int k = 0; k < entities.size(); ++k){
            entities[k]->collide(statics2[j]);
            if (entities[k]->winner){
                entities[k]->winner = false;
                winner->position.x = 0;
                state = STATE_LEVEL3;
            }
        }
    }
    
    for (Entity * player : entities){
        player->update(elapsed);
    }
    
    for (Entity * things : statics2){
        things->update(elapsed);
    }
    
    if (player1->position.y < (fabsf(viewMatrix.m[3][1]) -12 ) || player2->position.y < (fabsf(viewMatrix.m[3][1]) - 12)){
        state = STATE_GAME_OVER;
    }
    viewMatrix.Translate(0, 3.5*(-elapsed), 0);
    program.setViewMatrix(viewMatrix);

    
}
void UpdateGameLevel3(ShaderProgram& program, float elapsed){
    for ( int j = 0; j < statics3.size(); ++j){
        for (int k = 0; k < entities.size(); ++k){
            entities[k]->collide(statics3[j]);
            if (entities[k]->winner){
                state = STATE_WIN;
            }
        }
    }
    
    for (Entity * player : entities){
        player->update(elapsed);
    }
    
    for (Entity * things : statics3){
        things->update(elapsed);
    }
    
    if (player1->position.y < (fabsf(viewMatrix.m[3][1]) -8 ) || player2->position.y < (fabsf(viewMatrix.m[3][1]) - 8)){
        state = STATE_GAME_OVER;
    }
    viewMatrix.Translate(0, 2.5*(-elapsed), 0);
    program.setViewMatrix(viewMatrix);
    
    
}


void Update(ShaderProgram program) {
    // move stuff and check for collisions
    ticks = (float) SDL_GetTicks()/1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    fixedElapsed = elapsed;
//    screenShakeTime += elapsed;
//    if (screenShakeTime > 2.0){
//        screenShakeTime = 0;
//    }
    switch(state) {
        case STATE_MAIN_MENU:
        //UpdateMainMenu();
        break;
        case STATE_LEVEL1:
        if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        while (fixedElapsed >= FIXED_TIMESTEP) {
            fixedElapsed -= FIXED_TIMESTEP;
            UpdateGameLevel1(program, FIXED_TIMESTEP);
        }
        UpdateGameLevel1(program, fixedElapsed);
        break;
        case STATE_LEVEL2:
            if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
                fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
            }
            while (fixedElapsed >= FIXED_TIMESTEP) {
                fixedElapsed -= FIXED_TIMESTEP;
                UpdateGameLevel2(program, FIXED_TIMESTEP);
            }
            UpdateGameLevel2(program, fixedElapsed);
            break;
        case STATE_LEVEL3:
            if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS){
                fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
            }
            while (fixedElapsed >= FIXED_TIMESTEP) {
                fixedElapsed -= FIXED_TIMESTEP;
                UpdateGameLevel3(program, FIXED_TIMESTEP);
            }
            UpdateGameLevel3(program, fixedElapsed);
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

