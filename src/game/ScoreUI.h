#ifndef SCOREUI_H
#define SCOREUI_H


#include "raylib.h"
#include <bits/stdc++.h>
#include "../utils.h"
#include "../UIElement.h"
#include "physics.h"

using namespace std;
extern Settings settings;

class ScoreUI: public UIElement {
public:
    int score = 0;
    int stage = 1;
    ScoreUI(){}

    void render() {
        float screenW = settings.screenWidth;
        float screenH = settings.screenHeight;

        float h = screenH * 0.1;
        float w = h * 4;
        float strokeW = h * 0.02;

        Vector2 l1 = {screenW, h};
        Vector2 l2 = l1 + Vector2{-w , 0};
        Vector2 l3 = l2 + Vector2{-h/8, -h/8};
        Vector2 l4 = l3 + Vector2{0, -h*7/8};


        DrawLineEx(l1 , l2, strokeW, WHITE);
        DrawLineEx(l2 , l3, strokeW, WHITE);
        DrawLineEx(l3 , l4, strokeW, WHITE);
        DrawRectangleV(l4, {w + h/8, 7*h/8}, BLACK);
        DrawRectangleV(l2 + Vector2{0, -h/8}, {w, h/8}, BLACK);
        DrawTriangle(l3, l2, l2 + Vector2{0, -h/8}, BLACK);

        float cy = h / 2;
        float cx;

    
        cx = screenW - w + h/2;

    }
};

#endif