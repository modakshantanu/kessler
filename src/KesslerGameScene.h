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
#include "game/BulletUI.h"
#include "game/AstCount.h"
#include "AudioManager.h"
#include <bits/stdc++.h>
using namespace std;

extern Settings settings;
extern float uiScale;

extern MainMenu* mainMenu; 

class PauseScene;
extern PauseScene* pauseScene; 

extern Scene* curScene, *nextScene; 

extern AudioManager audio;

struct GameState {
    int points = 0;
    int asteroidLimit = 4 * 3;
    float zoneRadius = 1200;
    float zoneMaxRadius = 1200;
    float zoneMinRadius = 600;
    float shrinkRate = 25;
    float zoneInc = 100;
    int curStage = 1;
    int maxStage = 10;
    float stageTimeLeft = 30;
    float stageTotalTime = 30;
    int bulletLimit = 5;
    vector<int> asteroidCounts = {0,0,0,0,0};
    // Max asteroids = curStage + 2
    // Max Radius = 1300 - curStage * 75 
};

GameState initialState;


class KesslerGameScene: public Scene {
public:

    Camera2D camera = {0};
    Ship ship;
    Planet planet;
    TextView endText;
    TextView pointsTV;
    TextView astCountTV;
    TextView stageTV;
    TextView bulletsTV;

    vector<Asteroid> asteroids;
    vector<Particle> particles;
    vector<Bullet> bullets;
    GameState gs;

    BulletUI bulletUI;
    AstCount astCount;


    bool isAlive = true;
    bool shipCam = true;

    float curZoom;
    float targetZoom;
    float minZoom = 0.25;
    float maxZoom = 4;
    float zoomStep = 1.07177346254; 
    float zoomInc = 1.41421356237; // 2 ^ 1/2

    KesslerGameScene() {
        reset();
    }


    void reset() {
        
        ship.pos = {0,250};
        ship.vel = {200,0};
        ship.rot = 0;
        ship.newOrbit();

        planet.pos = {0,0};
        planet.vel = {0,0};

        asteroids.clear();
        bullets.clear();
        particles.clear();

        curZoom = targetZoom = 0.70710678118; // 1/sqrt(2)
        isAlive = true;
        shipCam = true;


        gs = initialState;

        float fontSize = settings.screenWidth / 1080.0 * 24;
        endText = TextView("Game Over. Press R to restart", settings.screenWidth / 2, settings.screenHeight * 0.75, 32, BOTTOM, CENTER, RED);
        pointsTV = TextView("Points: 0", settings.screenWidth - 5,settings.screenHeight * 0.1 + 5, fontSize, TOP, RIGHT, WHITE);
        stageTV = TextView("Stage: 1", settings.screenWidth - 5, settings.screenHeight * 0.1 + 5 + fontSize + 5 , fontSize, TOP, RIGHT, WHITE);


        float h = 32 * settings.screenHeight / 1080.0;
        h = max(32.0f, h);
        float x = settings.screenWidth / 2.0 - (gs.bulletLimit*h + gs.bulletLimit * (h-1) * 0.5) / 2.0;
        float y = settings.screenHeight * 0.9;
        bulletUI = BulletUI(x, y ,h, gs.bulletLimit);
        astCount = AstCount();

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

                if (!bbIntersects(bbAsteroids[i], bbShip)) {
                    continue;
                }
                if (polyIntersects(polyAsteroids[i], polyShip)) {
                    ship.addExplosionParticles(particles);
                    isAlive = false;   
                    audio.play(BOOM);                 
                    printf("Asteroid %d collided with ship\n", i);
                }
            }
            // Ship - planet intersection
            if (bbCircleIntersects(planet.pos, planet.radius, bbShip) 
                    && polyCircleIntersects(planet.pos, planet.radius, polyShip)) {
            
                ship.addExplosionParticles(particles);
                isAlive = false; 
                audio.play(BOOM);  
            }

            // Ship - bullet
            for (unsigned i = 0; i < bullets.size(); i++) {
                if (pointBbIntersects(bullets[i].pos, bbShip)
                        && pointPolyIntersects(bullets[i].pos, polyShip)) {
                
                    ship.addExplosionParticles(particles);
                    printf("Bullet %d collided with ship\n", i);
                    bullets[i].collided = true;
                    isAlive = false;   
                    audio.play(BOOM);
                }
            }

            // Out of range 
            if (mag(ship.pos - planet.pos) > gs.zoneRadius) {
                ship.addExplosionParticles(particles);
                isAlive = false;
                audio.play(BOOM);
                printf("Ship died due to out of range");
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
                audio.play(POP);


            }

            // Asteroid - bullet collision
            for (unsigned j = 0; j < bullets.size(); j++) {
                // printf("Checking asteroid %d, bullet %d\n", i, j);
                if (pointBbIntersects(bullets[j].pos , bbAsteroids[i]) &&
                        pointPolyIntersects(bullets[j].pos, polyAsteroids[i])) {
                    
                    
                    asteroids[i].collided = true;

                    
                    auto children1 = asteroids[i].split();
                    nextAsteroids.insert(nextAsteroids.end(), children1.begin(), children1.end());    

                    asteroids[i].addExplosionParticles(particles);  
                    bullets[j].collided = true;

                    gs.points++;
                    gs.zoneRadius = min(gs.zoneMaxRadius , gs.zoneRadius + gs.zoneInc);

                    audio.play(POP);

                    // printf("Bullet %d collided with asteroid %d\n", j, i);
                }
            }

            // Out of range 
            if (mag(asteroids[i].pos - planet.pos) > gs.zoneRadius) {
                asteroids[i].collided = true;
                asteroids[i].addExplosionParticles(particles);
                audio.play(POP);

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

                    audio.play(POP);
                }
            }

            noCollisions:

            // This asteroid survived all possible collisions
            if (!asteroids[i].collided && asteroids[i].size >= 1) {
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
            }
            if (mag(bullets[i].pos - planet.pos) > gs.zoneRadius) {
                bullets[i].collided = true;
            }

            if (!bullets[i].collided) {
                // printf("Bullet %d survived\n", i);
                nextBullets.push_back(bullets[i]);
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

        bool crosshairKey = IsKeyPressed(KEY_X);


        if (isAlive) { // Ship-related actions that must only happen if player is alive
            if (leftKey) {
                ship.rot -= frameTime * 5;
            } 
            if (rightKey) {
                ship.rot += frameTime * 5;
            }

            if (upKey) {

                ship.vel = ship.vel + rotate(Vector2{1,0}, ship.rot + PI / 2) * ship.thrust; 
                ship.pos += ship.vel  * frameTime;
                ship.moved = true;
                particles.push_back(ship.getParticle());
                particles.push_back(ship.getParticle());
                audio.boostOn();
            } else {
                audio.boostOff();
            }
            if (crosshairKey) {
                ship.crossHair = !ship.crossHair; 
            }
        } else {
            audio.boostOff();
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
        if (spaceKey && bullets.size() < (unsigned)gs.bulletLimit) {
            
            audio.play(BULLET);
            bullets.push_back(ship.getBullet());

        } else if (spaceKey && bullets.size() == (unsigned)gs.bulletLimit) {
            audio.play(EMPTY);
        }

        
        if (IsKeyPressed(KEY_ESCAPE)) {
            nextScene = (Scene*) pauseScene;
        }

        if (!isAlive && IsKeyPressed(KEY_R)) {
            reset();
        }
    }

    
    void gameStateUpdate(float frameTime) {
        
        if (!isAlive) return;

        gs.stageTimeLeft -= frameTime;
        
        gs.asteroidCounts.assign(5,0);
        for (auto &it: asteroids) {
            gs.asteroidCounts[it.size]++;
        }
        astCount.update(frameTime);
        astCount.counts = gs.asteroidCounts;
        int astCnt = 0;
        for (int i = 1; i <= 4; i++) {
            astCnt += pow(2, i - 1) * gs.asteroidCounts[i];
        }

        if (astCnt <= gs.asteroidLimit - 4) {
            int size = 3;
            if (rand() % 4 == 0) size = 4;
            asteroids.push_back(getRandomAsteroid(ship.pos, gs.zoneRadius, size));
        }
        


        // Code to move to next stage
        if (gs.stageTimeLeft < 0) {
            gs.curStage = min(gs.maxStage , gs.curStage+1);
            gs.stageTimeLeft = gs.stageTotalTime;
            gs.zoneMaxRadius -= 75;
            gs.asteroidLimit = gs.curStage * 4 + 8;
        }

        gs.zoneRadius -= gs.shrinkRate * frameTime;
        gs.zoneRadius = max(gs.zoneRadius , gs.zoneMinRadius);


        pointsTV.text = "Points: " + to_string(gs.points);
        astCountTV.text = "Asteroid Count: " + to_string(gs.asteroidCounts[1]) + "/" + to_string(gs.asteroidCounts[2]) + "/" + to_string(gs.asteroidCounts[3]);
        stageTV.text = "Stage: " + to_string(gs.curStage);
        bulletsTV.text = "Bullets: " + to_string(gs.bulletLimit - bullets.size());
        bulletUI.rem = gs.bulletLimit - bullets.size();

    }

    void update() {
        
        float frameTime = GetFrameTime();
        // if (abs(frameTime - 0.01666) > 0.001) printf("%f\n", frameTime);
        // printf("%f\n", frameTime);

        inputHandler(frameTime);
        gameStateUpdate(frameTime);
        

             
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
        Color dangerZone = {32,0,0,255};
        ClearBackground(dangerZone);
        BeginMode2D(camera);


        // Render background
        DrawCircle(0,0, gs.zoneRadius, BLACK);

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

        pointsTV.render();
        // astCountTV.render();
        astCount.render();
        
        stageTV.render();
        
        // bulletsTV.render();
        bulletUI.render();


        if (!isAlive) {
            endText.render();
        }
        // DrawFPS(10, 10);
        // EndDrawing();
    }
};



#endif