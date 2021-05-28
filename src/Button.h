

#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include "UIElement.h"
#include "mathutils.h"
#include "utils.h"
#include <bits/stdc++.h>

using namespace std;

extern float uiScale;

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
        this->x = x * uiScale;
        this->y = y * uiScale;
        this->w = w * uiScale;
        this->h = h * uiScale;
        this->text = text;
        borderWidth = 4 * uiScale;
        // borderWidth = screenHeight; // GetScreenHeight() / 200;
    }

    void render() {

        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        bool isMouseHovering = intersects(mouseX, mouseY);

        Color borderColor = isMouseHovering ? activeBorderColor : inactiveBorderColor; 
        Color BGColor = isPressed ? activeBGColor : inactiveBGColor; 
        Color fontColor = isPressed ? inactiveBGColor : activeBGColor;

        DrawRectangle(x, y, w, h, borderColor);
        DrawRectangle(x + borderWidth, y + borderWidth, w - 2*borderWidth, h - 2*borderWidth, BGColor);

        int fontSize = 32 * uiScale;
        int textX = (2*x + w) / 2 -  MeasureText(text.c_str(), fontSize) / 2;
        int textY = y + (h - fontSize) / 2;

        DrawText(text.c_str(), textX, textY, fontSize, fontColor);
        
    }

    bool intersects(int mouseX, int mouseY) {

        Rectangle bounds = {(float)x,(float)y,(float)w,(float)h};
        Vector2 mouseVec = {(float)mouseX, (float)mouseY};
        
        return math::intersects(bounds, mouseVec); 
    }


    static void buttonStateHandler(TextButton &button, int mouseX, int mouseY, bool up, bool down, bool pressed, bool released) {
        
        bool intersects = button.intersects(mouseX, mouseY); 
        if (pressed && intersects) {
            button.isPressed = true;
        } else if (released && button.isPressed) {
            button.onClick();
        }
        if (!intersects || !down) {
            button.isPressed = false;
        }
        
    }
};



#endif