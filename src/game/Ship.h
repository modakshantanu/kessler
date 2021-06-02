#ifndef SHIP_H
#define SHIP_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "physics.h"

class Ship : public OrbitalEntity {
public:
    Vector2 pos, vel, acc;
    Ellipse orbit;

    bool moved = true;

    Ship() {}
    Ship(Vector2 p, Vector2 v) : pos(p) , vel(v) {}
    
    void render() {
        DrawCircle(pos.x, pos.y, 5, BLUE);
    }

};

#endif