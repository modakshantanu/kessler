

#include "raylib.h"
#include "src/utils.h"
#include "src/Scene.h"
#include "src/MainMenu.h"
#include "src/GameScene.h"

namespace Win {
    #include "windows.h"
}

bool debug = true;


extern int screenWidth;
extern int screenHeight;
extern float uiScale;
extern MainMenu* mainMenu;
extern GameScene* gameScene;
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


    curScene = mainMenu;
    nextScene = mainMenu;

    if (debug) {
        Win::AllocConsole();
        freopen("CONOUT$", "w", stdout);
    }

    while (!WindowShouldClose()) {
        curScene->update();
        curScene->render();
        curScene = nextScene;

    }



    CloseWindow();


    return 0;
}