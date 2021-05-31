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
        textViews.push_back(TextView("Just a simple game of pong. Use A/D to control left and Arrow keys to control right", centerX - 300, 150, 24, TOP, LEFT, WHITE, 600));

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