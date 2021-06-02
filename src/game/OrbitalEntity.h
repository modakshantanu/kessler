#ifndef ORBITALENTITY_H
#define ORBITALENTITY_H

#include "raylib.h"

class OrbitalEntity {
public:
    Vector2 pos, vel;

    virtual void render() = 0;

};

#endif