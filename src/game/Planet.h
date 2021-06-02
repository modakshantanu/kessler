#ifndef PLANET_H
#define PLANET_H

#include "raylib.h"
#include "OrbitalEntity.h"

class Planet {
public:
    
    Vector2 pos, vel;
    double GM = 4500000;

    Planet() {}
    Planet(Vector2 p, Vector2 v) : pos(p) , vel(v) {}
    
    void render() {
        DrawCircle(pos.x, pos.y, 100, BROWN);
    }

};

#endif