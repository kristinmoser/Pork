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
#define LEVEL_HEIGHT 20
#define LEVEL_WIDTH 75
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 7
#define TILE_SIZE 0.5f

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

////-------- SETUP ----------

SDL_Window* displayWindow;
unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH] =
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {84,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,0,0,65,0,0,0,0,0,0,84,0},
    {93,80,0,0,0,0,47,83,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,84,0,0,0,0,0,0,64,0,61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,24,24,24,23,0,0,0,0,81,79,82},
    {93,94,39,41,0,0,9,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,0,0,0,95,93,82,0,0,0,0,0,5,3,19,0,0,0,0,0,0,0,0,0,66,0,0,0,0,0,0,0,0,0,0,83,0,0,9,23,0,0,58,0,0,0,0,0,0,0,81,79,82},
    {71,65,53,55,0,0,0,0,0,0,0,0,0,0,0,0,69,0,0,0,5,3,19,0,0,0,78,93,96,0,0,58,0,0,0,0,0,0,0,0,0,56,0,0,0,0,68,0,0,0,0,0,0,0,0,0,0,5,19,0,0,0,87,5,3,3,19,0,0,39,40,41,81,79,82},
    {3,3,3,3,3,3,3,3,6,56,0,0,0,0,0,0,9,23,0,0,28,43,29,31,56,0,0,86,0,0,0,9,23,0,0,0,0,0,0,0,7,17,47,7,3,3,3,6,0,0,0,0,0,0,0,0,56,0,0,0,0,0,86,0,0,59,0,0,56,54,55,54,0,86,0},
    {16,16,16,16,16,16,16,16,20,3,6,0,0,9,23,0,0,0,0,7,3,3,3,3,3,3,3,3,3,6,0,0,0,0,56,0,0,7,3,3,21,16,16,16,16,16,16,20,49,50,51,49,50,50,52,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {16,16,16,16,16,16,16,16,16,16,20,3,49,50,51,49,50,51,50,21,16,16,16,16,16,16,16,16,16,20,3,3,3,3,3,3,3,21,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16}
};

unsigned char solids[] = { 0, 1, 2, 3, 4 , 5, 6, 7, 8, 9, 10, 11, 12, 13,14,15,16, 17, 18, 19, 20, 21, 22, 23, 35, 36, 37, 38, 48, 49, 50, 51};
int mapWidth = 75;
int mapHeight = 20;
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


float fullScreenVertices[] = {
    -3.55f, -2.0f, //BL
    3.55f, -2.0f, //BR
    3.55f, 2.0f, //TR
    -3.55f, -2.0f, //BL
    3.55f, 2.0f, //TR
    -3.55f, 2.0f //TL
};

float wholeTexCoords[] = {
    0.0, 1.0,
    1.0, 1.0,
    1.0, 0.0,
    0.0, 1.0,
    1.0, 0.0,
    0.0, 0.0
};

////-------- MATRICES--------

Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;

////-------- TIME ----------

float lastFrameTicks = 0.0f;
float ticks;
float elapsed;

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
class Vector3 {
public:
    Vector3(float x, float y, float z) : x(x), y(y), z(z){}
    float x;
    float y;
    float z;
};

class SheetSprite {
public:
    SheetSprite(){}
    SheetSprite(GLuint texture, float u, float v, float width, float height, float size): texture(texture), u(u), v(v), width(width), height(height), size(size){}
    void Draw(ShaderProgram program){
        
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
    float size;
    GLuint texture;
    float u;
    float v;
    float width;
    float height;
};

class Entity{
public:
    
    Entity() : position(Vector3(0,0,0)), velocity(Vector3(0,0,0)), acceleration(Vector3(0,0,0)){}
    Entity(Vector3 pos, Vector3 vel, Vector3 acc): position(pos), velocity(vel), acceleration(acc){}
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float gravity = -2.0f;
    void calcBTLR(){
        bottom = position.y - (sprite.height/2);
        top = position.y + (sprite.height/2);
        left = position.x - (sprite.width / 2);
        right = position.x + (sprite.width/2);
    }
    void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
        *gridX = (int)(worldX / TILE_SIZE);
        *gridY = (int)(-worldY / TILE_SIZE);
    }
    float bottom;
    float top;
    float left;
    float right;
    float rotation;
    SheetSprite sprite;
};


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
    Vector3 zero = Vector3(0,0,0);
    player = Entity(zero, zero, zero);
    player.sprite = SheetSprite(playerText, 0/1024.0f, 0/1024.0f, 72/512.0f, 97/512.0f, 0.5);
    player.position.y = 5.0f;
    player.calcBTLR();
    entities.push_back(player);
    //tile = SheetSprite(sheet, tileMatrix, 0.0f, (84.0f / 14.0f) / 7.0f, 70.0f/1024.0f, 70.0f/1024.0f, 0.5f);
    projectionMatrix.setOrthoProjection(-3.0, 3.0, -2.0f, 2.0f, -1.0f, 1.0f);
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glUseProgram(program.programID);
    program.setModelMatrix(modelMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewMatrix(viewMatrix);
    return program;
}


void drawTiles(ShaderProgram program){
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int y=0; y < LEVEL_HEIGHT; y++) {
        for(int x=0; x < LEVEL_WIDTH; x++) {
            if(levelData[y][x] != 0) {
                float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
                float spriteWidth = 1.0f/(float)SPRITE_COUNT_X;
                float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
                
                vertexData.insert(vertexData.end(), {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    TILE_SIZE * x, -TILE_SIZE * y,
                    (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                    (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
                });
                texCoordData.insert(texCoordData.end(), {
                    u, v,
                    u, v+(spriteHeight),
                    u+spriteWidth, v+(spriteHeight),
                    u, v,
                    u+spriteWidth, v+(spriteHeight),
                    u+spriteWidth, v
                });
                
            }
        }
    }

    
    for(int y=0; y < vertexData.size(); y+= 12) {
        float vertex[] = {
            vertexData[y], vertexData[y+1], vertexData[y + 2], vertexData[y + 3],
            vertexData[y + 4], vertexData[y+5], vertexData[y + 6], vertexData[y + 7],
            vertexData[y + 8], vertexData[y+9], vertexData[y + 10], vertexData[y + 11]
        };
        float tex[] = {
            texCoordData[y], texCoordData[y+1], texCoordData[y + 2], texCoordData[y + 3],
            texCoordData[y + 4], texCoordData[y+5], texCoordData[y + 6], texCoordData[y + 7],
            texCoordData[y + 8], texCoordData[y+9], texCoordData[y + 10], texCoordData[y + 11]
        };
        
        draw(sheet, vertex, program, tex);
    }
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
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
            else if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                    moveLeft = true;
                    player.velocity.x = -2.0f;
                    
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                    moveRight = true;
                    player.velocity.x = 2.0f;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                    jump = true;
                    player.velocity.y = 2.0f;
       
                }
            }
            else if (event.type == SDL_KEYUP){
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                    moveLeft = false;
                  
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                    moveRight = false;
                    
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                    jump = false;
                    
                }
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
    modelMatrix.identity();
    program.setModelMatrix(modelMatrix);
    draw(background, fullScreenVertices, program, wholeTexCoords);
    modelMatrix.identity();
    modelMatrix.Translate(-3, 6.75, 0);
    program.setModelMatrix(modelMatrix);
    drawTiles(program);
    modelMatrix.identity();
    modelMatrix.Translate(player.position.x, player.position.y, 0);
    program.setModelMatrix(modelMatrix);
    player.sprite.Draw(program);
    viewMatrix.identity();
    viewMatrix.Translate(-player.position.x, -player.position.y, 0);
    program.setViewMatrix(viewMatrix);
    /*
     Does not recognize the position of the entity inside the vector
     
     for(int i=0; i < entities.size(); i++) {
        modelMatrix.identity();
        std::cout << entities[i].position.x << std::endl;
        modelMatrix.Translate(entities[i].position.x, entities[i].position.y, 0);
        program.setModelMatrix(modelMatrix);
        entities[i].sprite.Draw(program); 
     
    
    }*/
    
    
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

//void UpdateMainMenu(){
// move stuff and check for collisions
//}

void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
    *gridX = (int)(worldX / TILE_SIZE);
    *gridY = (int)(-worldY / TILE_SIZE);
}

void UpdateGameLevel(){
    player.velocity.y += player.gravity * elapsed;
    player.position.y += player.velocity.y * elapsed;
    if (moveLeft || moveRight){
        player.position.x += player.velocity.x * elapsed;
    }
    int gridX;
    int gridY;
    worldToTileCoordinates(player.position.x, player.position.y, &gridX, &gridY);
    std::cout << "grid before: " << gridX << " " << gridY << std::endl;
    if (!(gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight) && (std::find(std::begin(solids), std::end(solids), levelData[gridY][gridX]) != std::end(solids))){
        std::cout << "grid after: " << gridX << " " << gridY << std::endl;
        player.velocity.y = 0;
    }
    // move stuff and check for collisions
    //call .Update() on all entities
    
}



void Update() {
    // move stuff and check for collisions
    ticks = (float) SDL_GetTicks()/1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    switch(state) {
        case STATE_MAIN_MENU:
            //UpdateMainMenu();
            break;
        case STATE_GAME_LEVEL:
            UpdateGameLevel();
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

