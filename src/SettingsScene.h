#ifndef SETTINGSSCENE_H
#define SETTINGSSCENE_H

#include "raylib.h"
#include "Scene.h"
#include "TextView.h"
#include "Button.h"
#include <bits/stdc++.h>

extern int screenHeight, screenWidth;
extern float uiScale;
extern Scene* curScene, *nextScene; 

extern MainMenu* mainMenu;


class SettingsScene : public Scene {
public:
    TextView title;
    TextButton backButton;

    SettingsScene() {

        int centerX = screenWidth / 2;

        title = TextView("Settings" , centerX , 85, 64 * uiScale, BOTTOM, CENTER);


        int buttonWidth = 200 * uiScale;
        int buttonHeight = 50 * uiScale;
        backButton = TextButton(25, 25, buttonWidth, buttonHeight, "Back"); 
    
        backButton.onClick = []() {
            nextScene = (Scene*) mainMenu;
        };


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
        title.render();
    }
};

#endif