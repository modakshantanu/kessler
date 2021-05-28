#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "raylib.h"
#include "Scene.h"
#include "Button.h"
#include "utils.h"

#include <queue>
#include <vector>



extern int screenHeight, screenWidth;
extern Scene* curScene, *nextScene; 

class GameScene;
extern GameScene* gameScene;
extern MainMenu* mainMenu;

class PauseScene : public Scene {
private:
    std::vector<TextButton> buttons; 

public:

    PauseScene() {

        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;
        int buttonWidth = 250 * uiScale;
        int buttonHeight = 50 * uiScale;
        int buttonSpacing = 25 * uiScale;
        int numButtons = 3;
        int startY = centerY - ((numButtons - 1) * buttonSpacing + numButtons * buttonHeight) / 2;


        for (int i = 0; i < numButtons; i++) {
            buttons.push_back(TextButton(centerX - buttonWidth / 2 , startY + i * (buttonHeight + buttonSpacing) , buttonWidth , buttonHeight, "dummy" ));
        }

        buttons[0].text = "Resume";
        buttons[1].text = "Restart";
        buttons[2].text = "Main Menu";


        buttons[0].onClick = []() {
            nextScene = (Scene*) gameScene;
        };

        buttons[1].onClick = []() {
            nextScene = (Scene*) gameScene;
            gameScene->reset();
        };

        buttons[2].onClick = []() {
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
        

        for (auto &e: buttons) {
            TextButton::buttonStateHandler(e, mouseX, mouseY, mouseUp, mouseDown, mousePressed, mouseReleased);
        }

    }

    void render() {

        // Show game in bg of pause screen
        gameScene->render();
        
        ClearBackground(BLACK);
        for (auto &e: buttons) {
            e.render();
        }
        DrawFPS(10, 10);
    }
};

#endif