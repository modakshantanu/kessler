#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "raylib.h"
#include "mathutils.h"
#include "utils.h"
#include "Scene.h"
#include "Settings.h"
#include "game/Planet.h"
#include "game/Ship.h"
#include "game/physics.h"
#include "game/Asteroid.h"
#include <bits/stdc++.h>

extern Settings settings;
extern float uiScale;

extern MainMenu* mainMenu; 

class PauseScene;
extern PauseScene* pauseScene; 

extern Scene* curScene, *nextScene; 


class KesslerGameScene: public Scene {
public:

    Camera2D camera = {0};
    Ship ship;
    Planet planet;
    vector<Asteroid> asteroids;
    

   
    KesslerGameScene() {
        ship.pos = {0,150};
        ship.vel = {200,0};

        planet.pos = {0,0};
        planet.vel = {0,0};

        
    }


    void reset() {
    
    }

    void update() {
        
        float frameTime = GetFrameTime();
        if (abs(frameTime - 0.01666) > 0.001) printf("%f\n", frameTime);
        // printf("%f\n", frameTime);

        camera.target = planet.pos;
        camera.offset = {settings.screenWidth / 2.0f, settings.screenHeight / 2.0f};
        camera.zoom = 1;
        camera.rotation = 0;
        
        

        if (ship.moved) {
            ship.moved = false;
            ship.newOrbit();
        }


        ship.update(frameTime);

        for (auto &it: asteroids){
            it.update(frameTime);
        }


    
        if (IsKeyPressed(KEY_ESCAPE)) {
            nextScene = (Scene*) pauseScene;
        }
    }

    void render() {
        // BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        planet.render();


        ship.render();
        for (auto &it: asteroids) {
            it.render();
        }




        EndMode2D();
        DrawFPS(10, 10);
        // EndDrawing();
    }
};

#endif