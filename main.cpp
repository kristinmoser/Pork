#include <stdio.h>
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>


#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

////-------- SETUP ----------

SDL_Window* displayWindow;

////-------- GAME STATE & BOOLS -------------

bool done = false;
enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
int state = STATE_GAME_LEVEL;
bool moveLeft = false;
bool moveRight = false;
bool jump = false;


////-------- BACKGROUND VERTICES & TEXTCOORDS -------

GLuint background;
GLuint font;
GLuint sheet;
GLuint playerText;



////-------- MATRICES--------

Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;


////------ EVENTS ------

SDL_Event event;

////------ LOAD TEXTURE ---------

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

///------ CLASSES -------------





std::vector<Entity> entities;
Entity player;

////------ SETUP ---------




ShaderProgram Setup(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("holly jolly snowman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 360);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    font = LoadTexture("pixel_font.png");
    sheet = LoadTexture("sheet.png");
    background = LoadTexture("sky.png");
    playerText = LoadTexture("p1_spritesheet.png");

    
    projectionMatrix.setOrthoProjection(-3.0, 3.0, -2.0f, 2.0f, -1.0f, 1.0f);
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glUseProgram(program.programID);
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);
    return program;
}




////----------- EVENT PROCESSING ---------------

void ProcessMainMenu() {
    // our SDL event loop
    // check input events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
            
        }
        else if (event.type == SDL_KEYDOWN){
            state = STATE_GAME_LEVEL;
        }
    }
}

void ProcessGame() {
        // our SDL event loop
        // check input events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }

    }
}

void ProcessGameOver(){
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
            
        }
    }
}

void ProcessEvents() {
    // our SDL event loop
    // check input events
    switch (state) {
        case STATE_MAIN_MENU:
            ProcessMainMenu();
            break;
        case STATE_GAME_LEVEL:
            ProcessGame();
            break;
        case STATE_GAME_OVER:
            ProcessGameOver();
            break;
        default:
            break;
    }
}


///---------- RENDERING ---------

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
            //std::cout << "Enter Main Menu Render" << "\n";
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

////---------------- MOVEMENT & COLLISION -----------------------

void UpdateMainMenu(){
// move stuff and check for collisions
}


void UpdateGameLevel(float elapsed){
// move stuff and check for collisions
    
}



void Update() {
    // move stuff and check for collisions
    float ticks = (float) SDL_GetTicks()/1000.0f;
    float lastFrameTicks = ticks;
    float elapsed = ticks - lastFrameTicks;

    
    switch(state) {
        case STATE_MAIN_MENU:
            //UpdateMainMenu();
            break;
        case STATE_GAME_LEVEL:
            UpdateGameLevel(elapsed);
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

