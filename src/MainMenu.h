#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "Scene.h"
#include "Button.h"
#include "TextView.h"
#include "utils.h"
#include "Settings.h"

#include <queue>
#include <vector>



extern Settings settings;
extern Scene* curScene, *nextScene; 

class GameScene;
extern GameScene* gameScene;

class InstructionsScene;
extern InstructionsScene* instructionsScene;

class SettingsScene;
extern SettingsScene* settingsScene;

class MainMenu : public Scene {
private:
    std::vector<TextButton> buttons; 
    std::vector<TextView> textViews; 

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


        bool esc = IsKeyPressed(KEY_ESCAPE);
        if (esc) {
            std::exit(0);
        }

    }

    void render() {

        // BeginDrawing();
        ClearBackground(BLACK);

        for (auto &e: buttons) {
            e.render();
        }

        for (auto &e: textViews) {
            e.render();
        }

        DrawFPS(10, 10);
        // EndDrawing();
    }
};

#endif