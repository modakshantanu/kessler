#ifndef BULLETUI_H
#define BULLETUI_H

#include "raylib.h"
#include <bits/stdc++.h>
#include "utils.h"
#include "../UIElement.h"
#include "physics.h"

using namespace std;
extern Settings settings;

class BulletUI : public UIElement {
public:
    int total = 5;
    int rem = 5;
    float x,y,width,height;

    BulletUI() {}

    BulletUI(float xx, float yy, float hh, int tot = 5) {
        x = xx;
        y = yy; 
        height = hh;
        total = tot;
        width = height * tot * 1.5 - height * 0.5;
    }

    void renderBullet(float cx, float cy, float r) {
        Color bgColor = {168, 0, 0, 255};
        DrawCircle(cx, cy, r, bgColor);
    }

    void renderEmpty(float cx, float cy, float r) {
        Color bgColor = {168, 0, 0, 255};
        float strokeWidth = r * 0.1;
        DrawCircle(cx, cy, r, bgColor);
        DrawCircle(cx, cy, r - strokeWidth, BLACK);

        if (IsKeyDown(KEY_SPACE) && rem == 0) {
            Vector2 center = {cx, cy};
            float r2 = r * 0.4;

            DrawLineEx(center + Vector2{r2, r2} , center - Vector2{r2,r2}, strokeWidth * 2, bgColor);
            DrawLineEx(center + Vector2{r2, -r2} , center - Vector2{r2, -r2}, strokeWidth * 2, bgColor);
        }
    }

    void renderX(float cx, float cy, float r) {

    }

    void render() {
        float r = height / 2;
        float cx = x + r;
        float cy = y + r;
        for (int i = 0; i < total; i++) {
            if (i < rem) {
                renderBullet(cx, cy, r);
            } else {
                renderEmpty(cx, cy, r);
            }
            cx += 3*r;
        }
    }

};

#endif