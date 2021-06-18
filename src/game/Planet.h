#ifndef PLANET_H
#define PLANET_H

#include "raylib.h"
#include "OrbitalEntity.h"

class Planet {
public:
    
    Vector2 pos, vel;
    double GM = 10000000;
    float radius = 100;

    Planet() {}
    Planet(Vector2 p, Vector2 v) : pos(p) , vel(v) {}
    
    void render() {
        Color brown = {111, 78, 55, 255};
        Color darkbrown = {55, 39, 27, 255};
        DrawCircle(pos.x, pos.y, radius , darkbrown);

        DrawCircle(pos.x, pos.y, radius - 5 , brown);
    }

};

#endif