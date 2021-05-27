#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "Scene.h"
#include "Button.h"

#include <queue>
#include <vector>


void dd() {
    printf("Pressed\n");
    
}

void (*fun_ptr)(void);

class MainMenu : public Scene {
private:
    std::vector<TextButton> buttons; 

public:

    MainMenu() {
        buttons.push_back(TextButton(100, 100, 100,100, "Hello World"));

        buttons[0].onClick = []() {
            cout<<"Hello";
        };

    }   
    

    Scene* update() {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        int mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        int mouseUp = IsMouseButtonUp(MOUSE_LEFT_BUTTON);
        int mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        


        TextButton::buttonStateHandler(buttons, mouseX, mouseY, mouseUp, mouseDown, mousePressed, mouseReleased);

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