

#include "raylib.h"
#include "src/MainMenu.h"
#include "src/utils.h"

namespace Win {
    #include "windows.h"
}

bool debug = true;
int main() {
    
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib");

    SetTargetFPS(60);             
    SetExitKey(KEY_ESCAPE);

    MainMenu m = MainMenu();
    // ToggleFullscreen();

    if (debug) {
        Win::AllocConsole();
        freopen("CONOUT$", "w", stdout);
    }


    

    while (!WindowShouldClose()) {
        


        BeginDrawing();

        ClearBackground(BLACK);

        m.update();
        m.render();
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }



    CloseWindow();


    return 0;
}