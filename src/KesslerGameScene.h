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
#include "game/Particle.h"
#include <bits/stdc++.h>
using namespace std;

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
    vector<Particle> particles;

    float curZoom = 1;
    float targetZoom = 1;
    float minZoom = 0.5;
    float maxZoom = 2;
    float zoomStep = 1.023373892; // 2 ^ 1/60
    float zoomInc = 1.41421356237; // 2 ^ 1/2

   
    KesslerGameScene() {
        reset();
    }


    void reset() {
        
        ship.pos = {0,150};
        ship.vel = {200,0};
        ship.rot = 0;
        ship.newOrbit();

        planet.pos = {0,0};
        planet.vel = {0,0};
    }

    void update() {
        
        float frameTime = GetFrameTime();
        if (abs(frameTime - 0.01666) > 0.001) printf("%f\n", frameTime);
        // printf("%f\n", frameTime);

       
        bool leftKey = IsKeyDown(KEY_A);
        bool rightKey = IsKeyDown(KEY_D);
        bool upKey = IsKeyDown(KEY_W);
        bool spaceKey = IsKeyDown(KEY_SPACE);
        bool zoomInKey = IsKeyPressed(KEY_E);
        bool zoomOutKey = IsKeyPressed(KEY_Q);

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
            particles.push_back(ship.getParticle());
            particles.push_back(ship.getParticle());

        }

        if (zoomInKey) {
            targetZoom = max(minZoom, targetZoom / zoomInc);
        } 
        if (zoomOutKey) {
            targetZoom = min(maxZoom, targetZoom * zoomInc);
        }



        if (ship.moved) {
            ship.moved = false;
            ship.newOrbit();
        }


        ship.update(frameTime);

        for (auto &it: asteroids){
            it.update(frameTime);
        }

        vector<Particle> temp = particles;
        particles.clear();


        for (auto &it: temp) {
            it.update(frameTime);
            if (it.lifetime < it.ttl) {
                particles.push_back(it);
            }
        }
    
        if (IsKeyPressed(KEY_ESCAPE)) {
            nextScene = (Scene*) pauseScene;
        }


        if (abs(curZoom / targetZoom - 1) > 0.01) {
            if (targetZoom > curZoom) {
                curZoom *= zoomStep;
            } else {
                curZoom /= zoomStep;
            }
        } 
        
        

        

        camera.zoom = curZoom;
        camera.target = ship.pos;
        camera.offset = {settings.screenWidth / 2.0f, settings.screenHeight / 2.0f};
        camera.rotation = 0;
        
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
        for (auto &it: particles) {
            it.render();
        }



        EndMode2D();
        DrawFPS(10, 10);
        // EndDrawing();
    }
};

#endif