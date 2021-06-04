#ifndef ORBITALENTITY_H
#define ORBITALENTITY_H

#include "raylib.h"
#include "physics.h"

class OrbitalEntity {
public:
    Vector2 pos, vel;
    float rot = 0;
    Orbit orbit;
    float t;
    virtual void render() = 0;

};

#endif