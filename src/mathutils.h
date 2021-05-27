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

#endif