#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "Scene.h"
#include "Button.h"
#include "utils.h"

#include <queue>
#include <vector>



void (*fun_ptr)(void);
extern int screenHeight, screenWidth;


class MainMenu : public Scene {
private:
    std::vector<TextButton> buttons; 

public:

    MainMenu() {

        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;
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
        buttons[2].text = "Help";
        buttons[3].text = "Exit";


        buttons[0].onClick = []() {
            
        };
    }   
    

    Scene* update() {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        int mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        int mouseUp = IsMouseButtonUp(MOUSE_LEFT_BUTTON);
        int mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        

        for (auto &e: buttons) {
            TextButton::buttonStateHandler(e, mouseX, mouseY, mouseUp, mouseDown, mousePressed, mouseReleased);
        }

        



        return this;
    }

    void render() {
        for (auto &e: buttons) {
            e.render();
        }
    }
};

int a = 10;
int b = 5;

#endif