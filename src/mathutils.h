#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "raylib.h"
#include "bits/stdc++.h"
using namespace std;

namespace math {
    bool intersects(Rectangle &rect, Vector2 &point) {
        return point.x >= rect.x && point.x <= rect.x + rect.width
                && point.y >= rect.y && point.y <= rect.y + rect.height;  
    }



}

Vector2 operator+(Vector2&a, Vector2&b) {
    return {a.x + b.x, a.y + b.y};
}

#endif