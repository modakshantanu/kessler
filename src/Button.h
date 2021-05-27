

#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "UIElement.h"
#include "mathutils.h"
#include "utils.h"
#include <bits/stdc++.h>

using namespace std;


class TextButton : public UIElement{
public:
    int x;
    int y;
    int w;
    int h;
    string text;
    Color activeBorderColor = BLUE;
    Color inactiveBorderColor = WHITE;    
    Color activeBGColor = BLUE;
    Color inactiveBGColor = BLACK;
    int borderWidth = 5;
    bool isPressed = false;    
    TextButton() {}

    TextButton(int x, int y, int w, int h, string text) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->text = text;
        borderWidth = GetScreenHeight() / 200;
        // borderWidth = screenHeight; // GetScreenHeight() / 200;
    }

    void render() {

        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        bool isMouseHovering = intersects(mouseX, mouseY);

        Color borderColor = isMouseHovering ? activeBorderColor : inactiveBorderColor; 
        Color BGColor = isPressed ? activeBGColor : inactiveBGColor; 

        DrawRectangle(x, y, w, h, borderColor);
        DrawRectangle(x + borderWidth, y + borderWidth, w - 2*borderWidth, h - 2*borderWidth, BGColor);
        
    }

    bool intersects(int mouseX, int mouseY) {

        Rectangle bounds = {(float)x,(float)y,(float)w,(float)h};
        Vector2 mouseVec = {(float)mouseX, (float)mouseY};
        
        return math::intersects(bounds, mouseVec); 
    }


    static void buttonStateHandler(std::vector<TextButton>& buttons, int mouseX, int mouseY, bool up, bool down, bool pressed, bool released) {
        for (auto &e: buttons) {
            bool intersects = e.intersects(mouseX, mouseY); 
            if (pressed && intersects) {
                e.isPressed = true;
            } else if (released && e.isPressed) {
                e.onClick();
            }
            if (!intersects || !down) {
                e.isPressed = false;
            }
        }
    }
};



#endif