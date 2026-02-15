/**
* Author: Kareena Joseph 
* Assignment: Project 1: Simple 2D Scene
* Date due: 02/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"  
#include <math.h>

// Screen and timing configuration
constexpr int SCREEN_WIDTH  = 1600 / 2,
              SCREEN_HEIGHT = 900 / 2,
              FPS           = 60;

// World positions used for movement and placement
constexpr Vector2 SCREEN_CENTER = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
                  ENTRANCE_POS  = { SCREEN_WIDTH * 0.48f, SCREEN_HEIGHT * 0.66f },
                  MIDDLE_POS    = SCREEN_CENTER;

// Movement speeds, orbit radius, and scaling factors
constexpr float MAIN_SPEED         = 80.0f,
                DOG_ORBIT_R        = 120.0f,
                DOG_ORBIT_SPEED    = 1.6f,
                TREASURE_ROT_SPEED = 90.0f,   
                MAIN_SCALE         = 0.13f,
                DOG_SCALE          = MAIN_SCALE * 0.65f,
                TREASURE_SCALE     = 0.10f;

constexpr char MAIN_FP[]     = "assets/Main_Char.png", // this is me as a game character 
               SIDE_FP[]     = "assets/Side_kick.png", // this is my dog as my side kick 
               TREASURE_FP[] = "assets/Treasure.png";

AppStatus gAppStatus = RUNNING;

float gPreviousTicks = 0.0f,
      gTime          = 0.0f,
      gDogOrbitAngle = 0.0f,
      gLoopTimer     = 0.0f,
      gTreasureAngle = 0.0f,
      gSideScaleFactor = 1.0f;

Texture2D gMain,
          gSide,
          gTreasure;

Vector2 gMainPos     = SCREEN_CENTER,
        gSidePos     = SCREEN_CENTER,
        gTreasurePos = SCREEN_CENTER;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
void initialise()
{
    // Create window and load all required textures
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1 - Simple 2D Scene");
    gMain     = LoadTexture(MAIN_FP);
    gSide     = LoadTexture(SIDE_FP);
    gTreasure = LoadTexture(TREASURE_FP);
    gPreviousTicks = (float)GetTime();

    // Set starting position of main character (bottom-left area)
    gMainPos = { SCREEN_WIDTH * 0.20f, SCREEN_HEIGHT * 0.75f };
    gTreasurePos = ENTRANCE_POS;
    SetTargetFPS(FPS);
}

void processInput()
{
    // Close application if window exit is requested
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{

    // Calculate frame delta time
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    gTime += deltaTime;
    gLoopTimer += deltaTime;

    // Move main character toward the center for 5 seconds
    Vector2 toMiddle = { MIDDLE_POS.x - gMainPos.x, MIDDLE_POS.y - gMainPos.y };
    float dist = sqrtf(toMiddle.x * toMiddle.x + toMiddle.y * toMiddle.y);

    if (dist > 1.0f && gLoopTimer < 5.0f) {
        Vector2 dir = { toMiddle.x / dist, toMiddle.y / dist };
        float step = MAIN_SPEED * deltaTime;

        if (step > dist) step = dist;
        gMainPos.x += dir.x * step;
        gMainPos.y += dir.y * step;
    }

    // Reset scene after 5 seconds to create looping animation
    if (gLoopTimer >= 5.0f) {
        gMainPos = { SCREEN_WIDTH * 0.20f, SCREEN_HEIGHT * 0.75f };
        gDogOrbitAngle = 0.0f;
        gTreasurePos = ENTRANCE_POS;
        gLoopTimer = 0.0f;
    }

    // Rotate side character around main character
    gDogOrbitAngle += DOG_ORBIT_SPEED * deltaTime;

    if (gDogOrbitAngle > 6.2831853f) gDogOrbitAngle -= 6.2831853f;
    gSidePos = {
        gMainPos.x + DOG_ORBIT_R * cosf(gDogOrbitAngle),
        gMainPos.y + DOG_ORBIT_R * sinf(gDogOrbitAngle)
    };

    // Subtle pulsing animation for orbiting character
    gSideScaleFactor = 0.95f + 0.05f * (0.5f + 0.5f * cosf(gTime * 3.0f));
    gTreasurePos = ENTRANCE_POS;
    gTreasureAngle += TREASURE_ROT_SPEED * deltaTime;
}

void render()
{
    // Draw all objects to the screen
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw main character (scaled and centered)
    Rectangle mainSrc = { 0.0f, 0.0f, static_cast<float>(gMain.width), static_cast<float>(gMain.height) };
    Rectangle mainDst = {
        gMainPos.x,
        gMainPos.y,
        gMain.width  * MAIN_SCALE,
        gMain.height * MAIN_SCALE
    };
    Vector2 mainOrg = { mainDst.width / 2.0f, mainDst.height / 2.0f };
    DrawTexturePro(gMain, mainSrc, mainDst, mainOrg, 0.0f, WHITE);

    // Draw orbiting side character
    Rectangle sideSrc = { 0.0f, 0.0f, static_cast<float>(gSide.width), static_cast<float>(gSide.height) };
    Rectangle sideDst = {
        gSidePos.x,
        gSidePos.y,
        gSide.width  * DOG_SCALE * gSideScaleFactor,
        gSide.height * DOG_SCALE * gSideScaleFactor
    };
    Vector2 sideOrg = { sideDst.width / 2.0f, sideDst.height / 2.0f };
    DrawTexturePro(gSide, sideSrc, sideDst, sideOrg, 0.0f, WHITE);

    // Draw treasure at entrance position
    Rectangle treSrc = { 0.0f, 0.0f, static_cast<float>(gTreasure.width), static_cast<float>(gTreasure.height) };
    Rectangle treDst = {
        gTreasurePos.x,
        gTreasurePos.y,
        gTreasure.width  * TREASURE_SCALE,
        gTreasure.height * TREASURE_SCALE
    };
    Vector2 treOrg = { treDst.width / 2.0f, treDst.height / 2.0f };
    DrawTexturePro(gTreasure, treSrc, treDst, treOrg, gTreasureAngle, WHITE);

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gMain);
    UnloadTexture(gSide);
    UnloadTexture(gTreasure);

    CloseWindow();
}

int main(void)
{
    initialise();
    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }
    
    shutdown();
    return 0;
}