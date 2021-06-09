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
    float radius = 1;

    Particle() {}
    Particle(Vector2 p, Vector2 v, Color col1, Color col2, float r = 1) {
        pos = p;
        vel = v;
        ttl = 1;
        lifetime = 0;
        decay = 0.97;
        c1 = col1;
        c2 = col2;
        radius = r;
    }
    
    void update(float frameTime) {
        
        vel = vel * decay;
        pos = pos + vel * frameTime;
        lifetime += frameTime;

    }

    void render() { 
        DrawCircle(pos.x, pos.y,radius, gradient(c1, c2, lifetime / ttl));
    }

};

#endif