#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "raylib.h"
#include "mathutils.h"
#include "utils.h"
#include "Scene.h"
#include "TextView.h"
#include "Settings.h"
#include "game/Planet.h"
#include "game/Ship.h"
#include "game/physics.h"
#include "game/Asteroid.h"
#include "game/Particle.h"
#include "game/Bullet.h"
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
    TextView endText;
    vector<Asteroid> asteroids;
    vector<Particle> particles;
    vector<Bullet> bullets;

    bool isAlive = true;
    bool shipCam = true;

    float curZoom = 1;
    float targetZoom = 1;
    float minZoom = 0.25;
    float maxZoom = 4;
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

        asteroids.clear();
        bullets.clear();
        particles.clear();
        for (int i = 0; i < 5; i++) {
            asteroids.push_back(Asteroid());
            asteroids[i].generateRandom(ship.pos);
        }

        isAlive = true;
        shipCam = true;

        endText = TextView("Game Over. Press R to restart", settings.screenWidth / 2, settings.screenHeight * 0.75, 32, BOTTOM, CENTER, RED);
    }

    void collisionHandler() {
        
        vector<vector<Vector2>> polyAsteroids;
        vector<Vector2> polyShip;

        vector<BoundingBox> bbAsteroids;
        BoundingBox bbShip;

        for (unsigned i = 0; i < asteroids.size(); i++) {
            polyAsteroids.push_back(asteroids[i].getPoly());
            bbAsteroids.push_back(getBb(polyAsteroids[i]));
        }

        polyShip = ship.getPoly();
        bbShip = getBb(polyShip);

        if (isAlive) {
            // Asteroid - ship intersection
            for (unsigned i = 0; i < asteroids.size(); i++) {

                if (asteroids[i].cooldown > 0) {
                    continue;
                }

                if (!bbIntersects(bbAsteroids[i], bbShip)) {
                    continue;
                }
                if (polyIntersects(polyAsteroids[i], polyShip)) {
                    ship.addExplosionParticles(particles);
                    isAlive = false;                    
                }
            }
            // Ship - planet intersection
            if (bbCircleIntersects(planet.pos, planet.radius, bbShip) 
                    && polyCircleIntersects(planet.pos, planet.radius, polyShip)) {
            
                ship.addExplosionParticles(particles);
                isAlive = false;   
            }

            // Ship - bullet
            for (unsigned i = 0; i < bullets.size(); i++) {
                if (pointBbIntersects(bullets[i].pos, bbShip)
                        && pointPolyIntersects(bullets[i].pos, polyShip)) {
                
                    ship.addExplosionParticles(particles);
                    // printf("Bullet %d collided with ship\n", i);
                    bullets[i].collided = true;
                    isAlive = false;   
                }
            }
        }


        vector<Asteroid> nextAsteroids;


        for (unsigned i = 0; i < asteroids.size(); i++) {
            
            // Asteroid - planet collision
            if (bbCircleIntersects(planet.pos, planet.radius, bbAsteroids[i]) 
                    && polyCircleIntersects(planet.pos, planet.radius, polyAsteroids[i])) {
                // Collided with planet
                asteroids[i].collided = true;
                asteroids[i].addExplosionParticles(particles);  
            }

            // Asteroid - bullet collision
            for (unsigned j = 0; j < bullets.size(); j++) {
                // printf("Checking asteroid %d, bullet %d\n", i, j);
                if ( 
                         pointPolyIntersects(bullets[j].pos, polyAsteroids[i])) {
                
                    asteroids[i].collided = true;

                    if (asteroids[i].size > 1) {
                        auto children1 = asteroids[i].split();
                        nextAsteroids.insert(nextAsteroids.end(), children1.begin(), children1.end());    
                    }

                    asteroids[i].addExplosionParticles(particles);  
                    bullets[j].collided = true;

                    // printf("Bullet %d collided with asteroid %d\n", j, i);
                }
            }


            // Size 1 asteroids will not collide with each other
            // Newly formed asteroids will not collide
            if (asteroids[i].size == 1 || asteroids[i].cooldown > 0) {
                goto noCollisions; // YEAH I USED IT WHAT ARE YOU GONNA DO?????
            }

            // Asteroid - asteroid intersection
            for (unsigned j = i+1; j < asteroids.size(); j++) {
                
                if (!bbIntersects(bbAsteroids[i], bbAsteroids[j])) {
                    continue;
                }

                if (polyIntersects(polyAsteroids[i],  polyAsteroids[j])) {
                    asteroids[i].collided = asteroids[j].collided = true;
                    auto children1 = asteroids[i].split();
                    auto children2 = asteroids[j].split();

                    nextAsteroids.insert(nextAsteroids.end(), children1.begin(), children1.end());    
                    nextAsteroids.insert(nextAsteroids.end(), children2.begin(), children2.end());

                    asteroids[i].addExplosionParticles(particles);  
                    asteroids[j].addExplosionParticles(particles);    
                }
            }

            noCollisions:

            // This asteroid survived all possible collisions
            if (!asteroids[i].collided) {
                nextAsteroids.push_back(asteroids[i]);
            }
        }

        asteroids = nextAsteroids;
        sort(asteroids.begin(), asteroids.end(), [](Asteroid &a, Asteroid &b){return a.size > b.size;}); // sort in descending order of size 


        vector<Bullet> nextBullets;
        for (unsigned i = 0; i < bullets.size(); i++) {
            // printf("Checking Bullet with pos %f %f\n", bullets[i].pos.x, bullets[i].pos.y);
            if (circleCircleIntersects(bullets[i].pos, bullets[i].radius, planet.pos, planet.radius)) {
                bullets[i].collided = true;
                printf("Bullet %d collided with planet\n",i);
            }

            if (!bullets[i].collided) {
                // printf("Bullet %d survived\n", i);
                nextBullets.push_back(bullets[i]);
            } else {
                // printf("Bullet %d collided somewhere\n", i);
            }

        }
        bullets = nextBullets;
    }

    void inputHandler(float frameTime) {
          
        bool leftKey = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
        bool rightKey = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
        bool upKey = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
        bool spaceKey = IsKeyPressed(KEY_SPACE);
        bool zoomInKey = IsKeyPressed(KEY_E);
        bool zoomOutKey = IsKeyPressed(KEY_Q);


        if (isAlive) { // Ship-related actions that must only happen if player is alive
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
        }

        if (zoomInKey) {
            targetZoom = max(minZoom, targetZoom / zoomInc);
        } 
        if (zoomOutKey) {
            targetZoom = min(maxZoom, targetZoom * zoomInc);
        }
        if (IsKeyPressed(KEY_C)) {
            shipCam = !shipCam;
        }
        if (spaceKey) {
            Vector2 bPos = ship.pos + rotate({0,10} , ship.rot);
            Vector2 bVel = ship.vel + rotate({0,500}, ship.rot);


            bullets.push_back(ship.getBullet());

        }

        
        if (IsKeyPressed(KEY_ESCAPE)) {
            nextScene = (Scene*) pauseScene;
        }

        if (!isAlive && IsKeyPressed(KEY_R)) {
            reset();
        }
    }

    void update() {
        
        float frameTime = GetFrameTime();
        // if (abs(frameTime - 0.01666) > 0.001) printf("%f\n", frameTime);
        // printf("%f\n", frameTime);

        inputHandler(frameTime);
             
        if (ship.moved) {
            ship.moved = false;
            ship.newOrbit();
        }

        if (isAlive) {
            ship.update(frameTime);
        }

        for (auto &it: asteroids){
            it.update(frameTime);
        }
        for (auto &it: bullets) {
            // printf("Bullet pos = %f %f, vel = %f %f\n", it.pos.x, it.pos.y, it.vel.x, it.vel.y);
            it.update(frameTime);
            if (it.ttl == 0) {
                it.collided = true;
            }
        }

        vector<Particle> temp = particles;
        particles.clear();

        for (auto &it: temp) {
            it.update(frameTime);
            if (it.lifetime < it.ttl) {
                particles.push_back(it);
            }
        }

        // printf("Bullets size = %d\n", bullets.size());
        collisionHandler();


        if (abs(curZoom / targetZoom - 1) > 0.01) {
            if (targetZoom > curZoom) {
                curZoom *= zoomStep;
            } else {
                curZoom /= zoomStep;
            }
        } 





        camera.zoom = curZoom;
        camera.target = shipCam ? ship.pos : planet.pos;
        camera.offset = {settings.screenWidth / 2.0f, settings.screenHeight / 2.0f};
        camera.rotation = 0;
        
    }

    void render() {
        // BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        planet.render();

        
        for (auto &it: asteroids) {
            it.render();
        }
        for (auto &it: particles) {
            it.render();
        }
        for (auto &it: bullets) {
            it.render();
        }

        if (isAlive) {
            ship.render(curZoom);
        } 


        EndMode2D();

        if (!isAlive) {
            endText.render();
        }
        DrawFPS(10, 10);
        // EndDrawing();
    }
};



#endif