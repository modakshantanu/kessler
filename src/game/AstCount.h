#ifndef ASTCOUNT_H
#define ASTCOUNT_H


#include "raylib.h"
#include <bits/stdc++.h>
#include "../utils.h"
#include "../UIElement.h"
#include "physics.h"

using namespace std;
extern Settings settings;

class AstCount: public UIElement {
public:
    vector<int> counts = {0,0,0,0,0};
    float rot = 0;
    AstCount() {}

    void renderNumber(int sides, int num, float cx, float cy, float r, float strokeW) {
        

        vector<Vector2> polyPoints;
        for (int i = 0; i < sides; i++) {
            polyPoints.push_back(Vector2{cx, cy} + rotate({0,-r}, i * 2 * PI / sides + rot));
        }
        for (int i = 0; i < sides; i++) {
            DrawLineEx(polyPoints[i], polyPoints[(i+1)%sides], strokeW, WHITE);
        }
        float fontSize = 24 * (r / 37.8);
        string text = to_string(num);
        float width = MeasureText(text.c_str(), fontSize);

        float textX = cx - width / 2;
        float textY = cy - fontSize / 2;
        DrawText(text.c_str(), textX, textY, fontSize, WHITE);
    }

    void render() {
        float screenW = settings.screenWidth;
        float screenH = settings.screenHeight;

        float h = screenH * 0.1;
        float w = h * 4;
        float strokeW = h * 0.02;

        Vector2 l1 = {screenW, h};
        Vector2 l2 = l1 + Vector2{-w , 0};
        Vector2 l3 = l2 + Vector2{-h/2, -h/2};
        Vector2 l4 = l3 + Vector2{0, -h/2};

        Color dark = {30, 30, 30, 255};

        DrawLineEx(l1 , l2, strokeW, WHITE);
        DrawLineEx(l2 , l3, strokeW, WHITE);
        DrawLineEx(l3 , l4, strokeW, WHITE);
        DrawRectangleV(l4, {w + h/2, h/2}, BLACK);
        DrawRectangleV(l2 + Vector2{0, -h/2}, {w, h/2}, BLACK);
        DrawTriangle(l3, l2, l2 + Vector2{0, -h/2}, BLACK);

        float cy = h / 2;
        float cx;

    
        cx = screenW - w + h/2;
    

        for (int i = 1; i < 5; i++) {


            renderNumber(i + 3, counts[i], cx, cy, h * 0.35, strokeW);
            cx += h;
        }
    }

    void update(float frameTime) {
        rot += frameTime * 0.5;
    }
};

#endif