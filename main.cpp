

#include "raylib.h"
#include "src/utils.h"
#include "src/Scene.h"
#include "src/MainMenu.h"
#include "src/GameScene.h"
#include "src/PauseScene.h"

namespace Win {
    #include "windows.h"
}

bool debug = true;


extern int screenWidth;
extern int screenHeight;
extern float uiScale;
extern GameScene* gameScene;
extern PauseScene* pauseScene;
extern MainMenu* mainMenu;
extern Scene* curScene, *nextScene; 

int main() {
    
    screenWidth = 1280;
    screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Kessler");

    SetTargetFPS(60);            
    SetExitKey(KEY_NULL);
    SetConfigFlags(FLAG_VSYNC_HINT);

    mainMenu = new MainMenu();
    gameScene = new GameScene();
    pauseScene = new PauseScene();


    curScene = mainMenu;
    nextScene = mainMenu;

    if (debug) {
        Win::AllocConsole();
        freopen("CONOUT$", "w", stdout);
    }

    while (!WindowShouldClose()) {
        curScene->update();
        BeginDrawing();
        curScene->render();
        EndDrawing();
        curScene = nextScene;

    }



    CloseWindow();


    return 0;
}