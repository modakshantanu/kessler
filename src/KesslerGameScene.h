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
        reset();


    }


    void reset() {
        
        ship.pos = {150,150};
        ship.vel = {150,0};
        ship.rot = 0;
        ship.newOrbit();

        planet.pos = {0,0};
        planet.vel = {0,0};
    }

    void update() {
        
        float frameTime = GetFrameTime();
        if (abs(frameTime - 0.01666) > 0.001) printf("%f\n", frameTime);
        // printf("%f\n", frameTime);

        camera.target = planet.pos;
        camera.offset = {settings.screenWidth / 2.0f, settings.screenHeight / 2.0f};
        camera.zoom = 1;
        camera.rotation = 0;
        
        bool leftKey = IsKeyDown(KEY_LEFT);
        bool rightKey = IsKeyDown(KEY_RIGHT);
        bool upKey = IsKeyDown(KEY_UP);
        bool spaceKey = IsKeyDown(KEY_SPACE);

        if (leftKey) {
            ship.rot -= frameTime * 5;
        } 
        if (rightKey) {
            ship.rot += frameTime * 5;
        }

        if (upKey) {

            ship.vel = ship.vel + rotate(Vector2{1,0}, ship.rot + PI / 2) * 0.5 ; 
            ship.pos += ship.vel  * frameTime;
            ship.moved = true;
        }
 
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