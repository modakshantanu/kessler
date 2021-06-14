#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "physics.h"
#include "../utils.h"
using namespace std;

extern Color bulletPurple;

class Bullet : public OrbitalEntity {
public:

    float ttl = 5;
    float radius = 2;
    bool collided = false;

    Bullet() {}

    Bullet(Vector2 p , Vector2 v) {
        pos = p;
        vel = v;
        newOrbit();
    }
    
    void update(float frameTime) {
        OrbitalEntity::update(frameTime);
        ttl = max(0.0f, ttl - frameTime);
        // printf("TTl  = %f\n",ttl);


    }

    void render() {
        DrawCircle(pos.x, pos.y, radius, bulletPurple);
    }
};

#endif