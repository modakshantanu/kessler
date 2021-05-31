

#include "raylib.h"
#include "src/utils.h"
#include "src/Scene.h"
#include "src/MainMenu.h"
#include "src/GameScene.h"
#include "src/PauseScene.h"
#include "src/InstructionsScene.h"
#include "src/SettingsScene.h"

#include "src/Settings.h"

namespace Win {
    #include "windows.h"
}

bool debug = false;


extern int screenWidth;
extern int screenHeight;
extern float uiScale;
extern GameScene* gameScene;
extern PauseScene* pauseScene;
extern MainMenu* mainMenu;
extern InstructionsScene* instructionsScene;
extern SettingsScene* settingsScene;

extern Scene* curScene, *nextScene; 
extern Settings settings;

int main() {
    
    screenWidth = 1280;
    screenHeight = 720;
    // screenWidth = 1920;
    // screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Kessler");

    SetTargetFPS(60);            
    SetExitKey(KEY_NULL);
    SetConfigFlags(FLAG_VSYNC_HINT);

    mainMenu = new MainMenu();
    gameScene = new GameScene();
    pauseScene = new PauseScene();
    settingsScene = new SettingsScene();
    instructionsScene = new InstructionsScene();


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