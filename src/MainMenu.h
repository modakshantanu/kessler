#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "Scene.h"
#include "Button.h"
#include "TextView.h"
#include "utils.h"
#include "Settings.h"
#include "game/Planet.h"
#include "game/Asteroid.h"
#include "game/Ship.h"
#include "game/Particle.h"


#include <queue>
#include <vector>

using namespace std;



extern Settings settings;
extern Scene* curScene, *nextScene; 

class KesslerGameScene;
extern KesslerGameScene* gameScene;

class InstructionsScene;
extern InstructionsScene* instructionsScene;

class SettingsScene;
extern SettingsScene* settingsScene;

class MainMenu : public Scene {
private:
    std::vector<TextButton> buttons; 
    std::vector<TextView> textViews; 

    Planet planet;
    vector<Asteroid> asteroids;
    vector<Particle> particles;

    float bgTime = 0;
    float bgLimit = 20;

public:

    MainMenu() {

        int centerX = settings.screenWidth / 2;
        int centerY = settings.screenHeight / 2;
        int buttonWidth = 250 * uiScale;
        int buttonHeight = 50 * uiScale;
        int buttonSpacing = 25 * uiScale;
        int numButtons = 4;
        int startY = centerY - ((numButtons - 1) * buttonSpacing + numButtons * buttonHeight) / 2;


        for (int i = 0; i < numButtons; i++) {
            buttons.push_back(TextButton(centerX - buttonWidth / 2 , startY + i * (buttonHeight + buttonSpacing) , buttonWidth , buttonHeight, "dummy" ));
            buttons[i].inactiveBorderColor.a = buttons[i].inactiveBGColor.a = 160; 
        }

        buttons[0].text = "Play";
        buttons[1].text = "Settings";
        buttons[2].text = "Instructions";
        buttons[3].text = "Exit";


        buttons[0].onClick = []() {
            nextScene = (Scene*) gameScene;
        };

        buttons[1].onClick = []() {
            nextScene = (Scene*) settingsScene;
        };

        buttons[2].onClick = []() {
            nextScene = (Scene*) instructionsScene;
        };

        buttons[3].onClick = []() {
            std::exit(0);
        };

        textViews.push_back(TextView("Kessler" , centerX, settings.screenHeight * 0.1, 96, TOP, CENTER));
        textViews.push_back(TextView("By Shantanu Modak", 5, settings.screenHeight - 5, 20, BOTTOM, LEFT));
        resetBg();
    }   

    void resetBg() {
        planet = Planet({0,0}, {0,0});
        asteroids.clear();
        for (int i = 0; i < 8; i++) {
            asteroids.push_back(getRandomAsteroid({150,0}, 800));
        }


    }
    

    void update() {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        int mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        int mouseUp = IsMouseButtonUp(MOUSE_LEFT_BUTTON);
        int mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        

        for (auto &e: buttons) {
            TextButton::buttonStateHandler(e, mouseX, mouseY, mouseUp, mouseDown, mousePressed, mouseReleased);
        }

        updateBg();

        bool esc = IsKeyPressed(KEY_ESCAPE);
        if (esc) {
            std::exit(0);
        }

    }

    void updateBg() {
        
        float frameTime = GetFrameTime();
        bgTime += frameTime;
        if (bgTime > bgLimit) {
            bgTime = 0;
            resetBg();
        }
        
        
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


        vector<vector<Vector2>> polyAsteroids;

        vector<BoundingBox> bbAsteroids;

        for (unsigned i = 0; i < asteroids.size(); i++) {
            polyAsteroids.push_back(asteroids[i].getPoly());
            bbAsteroids.push_back(getBb(polyAsteroids[i]));
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
            if (!asteroids[i].collided && asteroids[i].size >= 1) {
                nextAsteroids.push_back(asteroids[i]);
            } 
        }

        asteroids = nextAsteroids;
        sort(asteroids.begin(), asteroids.end(), [](Asteroid &a, Asteroid &b){return a.size > b.size;}); // sort in descending order of size 

    }

    void renderBg() {

        Camera2D camera;
        camera.target = {0,0};
        camera.zoom = 1 / sqrt(2);
        camera.rotation = 0;
        camera.offset = {settings.screenWidth / 2.0f, settings.screenHeight / 2.0f};

        BeginMode2D(camera);
        planet.render();
        for (auto &e: asteroids) {
            e.render();
        }
        for (auto &e: particles) {
            e.render();
        }
        EndMode2D();

    }

    void render() {

        // BeginDrawing();
        ClearBackground(BLACK);
        renderBg();

        for (auto &e: buttons) {
            e.render();
        }

        for (auto &e: textViews) {
            e.render();
        }


        // DrawFPS(10, 10);
        // EndDrawing();
    }
};

#endif