#ifndef OPTIONS_H
#define OPTIONS_H

#include "raylib.h"
#include "UIElement.h"
#include "mathutils.h"
#include "Settings.h"
#include <bits/stdc++.h>

class OptionsView;

extern float uiScale;

class OptionButton : public UIElement {
public:

    int x;
    int y;
    
    int w;
    int h;

    int fontSize;

    std::string text;
    std::string id;
    Color activeColor = BLUE;
    Color inactiveColor = WHITE;    
    
    int barWidth = 3;
    
    bool isSelected = false;
    bool isPressed = false;
    
    int idx;
    OptionsView* parent = NULL;


    OptionButton() {}
    OptionButton(std::string text, int x, int y, int fontSize, OptionsView* parent) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->fontSize = fontSize;
        this->parent = parent;
        this->id = text;

        this->w = MeasureText(text.c_str(), fontSize);
        this->y -= fontSize;
        this->h = fontSize + 2*barWidth;
    }

    bool intersects(int mouseX, int mouseY) {

        Rectangle bounds = {(float)x,(float)y,(float)w,(float)h};
        Vector2 mouseVec = {(float)mouseX, (float)mouseY};
        
        return math::intersects(bounds, mouseVec); 
    }

    void render() {

        int mouseX = GetMouseX();
        int mouseY = GetMouseY();


        bool hover = intersects(mouseX, mouseY);
        

        DrawText(text.c_str() ,x,y , fontSize, isPressed ? activeColor : inactiveColor);

        if (isSelected) {
            DrawRectangle(x , y + fontSize + barWidth, w, barWidth * 2, activeColor);
        } else if (hover) {
            DrawRectangle(x , y + fontSize + barWidth, w, barWidth, activeColor);
        }
    }

};

class OptionsView : public UIElement {
public:
    vector<OptionButton> buttons;
    std::string label;
    int labelFontSize = 32;
    int labelWidth = 0;
    int buttonFontSize = 24;
    int x;
    int y;
    int nextX;
    int hMargin = 20;
    
    void (*onChange)(std::string) = [](std::string id){};

    OptionsView() {}

    OptionsView(std::string txt, int x, int y) {
        label = txt;
        this->x = x;
        this->y = y;
        labelWidth = MeasureText(txt.c_str() , labelFontSize);
        nextX = x + labelWidth + hMargin;
    }

    void addOption(std::string text) {
        
        OptionButton b = OptionButton(text, nextX, y - 3, buttonFontSize, this);
        nextX += b.w + hMargin;
        buttons.push_back(b);
    }

    void setSelected(std::string id) {
        for (auto &it: buttons) {
            if (it.id == id) it.isSelected = true;
        }
    }

    void update() {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        // int mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
        // int mouseUp = IsMouseButtonUp(MOUSE_LEFT_BUTTON);
        // int mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        for (auto &e: buttons) {
            if (e.intersects(mouseX, mouseY) && mousePressed && !e.isSelected) {

                for (auto &f: buttons) {
                    f.isSelected = false;
                }

                e.isSelected = true;
                onChange(e.id);
            } 
        }


    }

    void render() {
        DrawText(label.c_str(), x, y-labelFontSize, labelFontSize, WHITE);
        for (auto &e: buttons) {
            e.render();
        }
    }

};

#endif