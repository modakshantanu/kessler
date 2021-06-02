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
        
        float n = 1;
        // for (int i  =0; i < n; i++) {
            ship.acc = gravAcc(planet.pos, ship.pos, planet.GM);
            ship.vel = ship.vel + (ship.acc * (frameTime/ n));
            ship.pos = ship.pos + (ship.vel * (frameTime/ n));

        // }
        

        float v2 = magsq(ship.vel);
        // printf("%f\n", v2);
        float r = mag(ship.pos - planet.pos);
        // printf("%f\n", r);
        float a = 1.0 / (2.0/r - v2 / planet.GM);
        // printf("%f\n", a);

        Ellipse orbit = getEllipse(planet.pos, ship.pos, ship.vel, a);
        ship.orbit  = orbit;


    



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


        for (float t = 0; t < 2*PI; t += PI / 60.0) {
            Vector2 p = parametricEllipse(ship.orbit, ship.pos, ship.vel, t, PI / 60.0);
            DrawPixel(p.x , p.y, Color{0, 150, 50, 255});
        }


        EndMode2D();
        DrawFPS(10, 10);
        // EndDrawing();
    }
};

#endif