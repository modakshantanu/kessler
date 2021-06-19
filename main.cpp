

#include "raylib.h"
#include "src/KesslerGameScene.h"
#include "src/utils.h"
#include "src/Scene.h"
#include "src/MainMenu.h"
#include "src/PauseScene.h"
#include "src/InstructionsScene.h"
#include "src/SettingsScene.h"
#include "src/AudioManager.h"
#include "src/ResScene.h"
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
extern ResScene* resScene;



extern Scene* curScene, *nextScene; 
extern Settings settings;

extern AudioManager audio;

int main() {

    srand(time(NULL));
    debug = false;

    settings = loadSettings("kessler_settings.txt");
    // screenWidth = 1920;
    // screenHeight = 1080;

    InitWindow(settings.screenWidth, settings.screenHeight, "Kessler");
    audio.init();


    if (IsWindowFullscreen() != settings.fullScreen) ToggleFullscreen();

    SetTargetFPS(60);            
    SetExitKey(KEY_NULL);

    mainMenu = new MainMenu();
    gameScene = new KesslerGameScene();
    pauseScene = new PauseScene();
    settingsScene = new SettingsScene();
    instructionsScene = new InstructionsScene();
    resScene = new ResScene();


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