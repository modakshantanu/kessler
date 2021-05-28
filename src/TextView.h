#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "raylib.h"
#include "UIElement.h"
#include <bits/stdc++.h>


enum {LEFT, CENTER, RIGHT};
enum {TOP, MIDDLE, BOTTOM};

class TextView : public UIElement {
public:
    std::string text;
    int vAlign = TOP;
    int hAlign = LEFT;
    int maxWidth = -1;
    int x;
    int y;
    int fontSize;
    Color textColor;

    TextView() {}

    TextView(std::string text, int x, int y, int fontSize, int vAlign = TOP, int hAlign = LEFT, Color color = WHITE, int maxWidth = -1) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->fontSize = fontSize;
        this->vAlign = vAlign;
        this->hAlign = hAlign;
        this->maxWidth = maxWidth;
        this->textColor = color;
    }

    void render() {
        int tlX = 0 , tlY = 0;
        if (vAlign == TOP) {
            tlY = y;
        } else if (vAlign == MIDDLE) {
            tlY = y - fontSize / 2;
        } else if (vAlign == BOTTOM) {
            tlY = y - fontSize;
        }

        int textWidth = MeasureText(text.c_str(), fontSize);

        if (hAlign == LEFT) {
            tlX = x;
        } else if (hAlign == CENTER) {
            tlX = x - textWidth / 2;
        } else if (hAlign == RIGHT) {
            tlX = x - textWidth;
        }

        Rectangle boundingRect = {(float) tlX,(float) tlY,100000, 100000};

        if (maxWidth >= 0) {
            boundingRect.width = maxWidth;
        }

        DrawTextRec(GetFontDefault(), text.c_str(), boundingRect, fontSize, fontSize / 10, true, textColor);
    }


};


#endif