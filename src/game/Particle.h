#ifndef PARTICLE_H
#define PARTICLE_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "physics.h"

class Particle : public OrbitalEntity {
public:

    Color c1, c2;
    float ttl;
    float lifetime;
    float decay;

    Particle() {}
    Particle(Vector2 p, Vector2 v, Color col1, Color col2) {
        pos = p;
        vel = v;
        ttl = 1;
        lifetime = 0;
        decay = 0.95;
        col1 = c1;
        col2 = c2;
    }
    
    void update(float frameTime) {
        
        vel = vel * decay;
        pos = pos + vel * frameTime;
        lifetime += frameTime;

    }

    void render() { 
        DrawCircle(pos.x, pos.y,1, gradient(YELLOW, RED, lifetime / ttl));
    }

};

#endif