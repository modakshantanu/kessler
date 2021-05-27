

#include "raylib.h"
#include "src/Scene.h"
#include "src/MainMenu.h"
#include "src/utils.h"

namespace Win {
    #include "windows.h"
}

bool debug = true;
float uiScale = 1.0;
int screenWidth;
int screenHeight;


MainMenu mainMenu;
GameScene gameScene;
Scene* curScene, *nextScene; 

int main() {
    
    screenWidth = 1280;
    screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib");

    SetTargetFPS(60);             
    SetExitKey(KEY_ESCAPE);

    mainMenu = MainMenu();
    curScene = &mainMenu;
    // ToggleFullscreen();

    if (debug) {
        Win::AllocConsole();
        freopen("CONOUT$", "w", stdout);
    }


    

    while (!WindowShouldClose()) {
        


        BeginDrawing();

        ClearBackground(BLACK);

        nextScene = curScene->update();
        curScene->render();
        curScene = nextScene;

        
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }



    CloseWindow();


    return 0;
}