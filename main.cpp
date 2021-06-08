

#include "raylib.h"
#include "src/utils.h"
#include "src/Scene.h"
#include "src/MainMenu.h"
#include "src/PauseScene.h"
#include "src/InstructionsScene.h"
#include "src/SettingsScene.h"
#include "src/KesslerGameScene.h"

#include "src/Settings.h"

namespace Win {
    #include "windows.h"
}

bool debug = false;

extern float uiScale;
extern KesslerGameScene* gameScene;
extern PauseScene* pauseScene;
extern MainMenu* mainMenu;
extern InstructionsScene* instructionsScene;
extern SettingsScene* settingsScene;

extern Scene* curScene, *nextScene; 
extern Settings settings;

int main() {

    srand(time(NULL));
    debug = true;

    settings = loadSettings("kessler_settings.txt");
    // screenWidth = 1920;
    // screenHeight = 1080;

    InitWindow(settings.screenWidth, settings.screenHeight, "Kessler");

    SetTargetFPS(60);            
    SetExitKey(KEY_NULL);
    SetConfigFlags(FLAG_VSYNC_HINT);

    mainMenu = new MainMenu();
    gameScene = new KesslerGameScene();
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