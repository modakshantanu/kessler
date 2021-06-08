#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "physics.h"

class Asteroid : public OrbitalEntity {
public:

    Asteroid() {}
    Asteroid(Vector2 p, Vector2 v) {
        pos = p;
        vel = v;
    }
    

    void render() { 
        DrawCircle(pos.x, pos.y, 5, DARKGRAY);
    }

};

#endif