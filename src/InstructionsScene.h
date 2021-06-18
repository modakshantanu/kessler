#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "raylib.h"
#include "Scene.h"
#include "TextView.h"
#include "Button.h"
#include "Settings.h"
#include <bits/stdc++.h>

extern Settings settings;
extern float uiScale;
extern Scene* curScene, *nextScene; 

extern MainMenu* mainMenu;

class InstructionsScene : public Scene {
public:
    TextButton backButton;
    std::vector<TextView> textViews;

    InstructionsScene() {

        int centerX = settings.screenWidth / 2;
        int buttonWidth = 200 * uiScale;
        int buttonHeight = 50 * uiScale;
        backButton = TextButton(25, 25, buttonWidth, buttonHeight, "Back"); 
    
        backButton.onClick = []() {
            nextScene = (Scene*) mainMenu;
        };

        textViews.push_back(TextView("Instructions" , centerX , 85, 64 * uiScale, BOTTOM, CENTER));
        textViews.push_back(TextView("Controls", centerX - 300, 150, 32, TOP, LEFT, BLUE, 600));
        textViews.push_back(TextView("A/D or Left/Right Arrow Keys -> Steer the Ship ", centerX - 300, 195, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("W or Up Arrow -> Boost ", centerX - 300, 224, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("Space -> Shoot ", centerX - 300, 253, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("Q/E -> Zoom in/out", centerX - 300, 282, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("C -> Toggle Camera Focus, x -> Toggle Crosshair", centerX - 300, 311, 24, TOP, LEFT, WHITE, 600));


        textViews.push_back(TextView("Gameplay", centerX - 300, 350, 32, TOP, LEFT, BLUE, 600));
        textViews.push_back(TextView("Shoot as many asteroids as you can and dont die!", centerX - 300, 395, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("You die if you hit the planet, any bullet or asteroid, or go into the red zone.", centerX - 300, 429, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("The number of asteroids will slowly increase and the zone will shrink as the game progresses.", centerX - 300, 500, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("Use ammo wisely! You can only have 5 bullets active at any time.", centerX - 300, 570, 24, TOP, LEFT, WHITE, 600));
        textViews.push_back(TextView("All objects follow proper orbital paths, so movement can be a bit counterintuitive.", centerX - 300, 640, 24, TOP, LEFT, WHITE, 600));
        

    }

    void update() {

        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        int mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        int mouseUp = IsMouseButtonUp(MOUSE_LEFT_BUTTON);
        int mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        TextButton::buttonStateHandler(backButton, mouseX, mouseY, mouseUp, mouseDown, mousePressed, mouseReleased);
        
        bool esc = IsKeyPressed(KEY_ESCAPE);
        if (esc) {
            nextScene = (Scene*) mainMenu;
        }

    }

    void render() {

        ClearBackground(BLACK);
        backButton.render();
        for (auto &e: textViews) {
            e.render();
        }
    }

};

#endif