#ifndef SHIP_H
#define SHIP_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "physics.h"
#include "Particle.h"
#include "Bullet.h"

class Ship : public OrbitalEntity {
public:
    
    vector<Vector2> orbitPoints;
    bool moved = true;
    bool collided = false;
    float thrust = 0.75;

    Ship() {}
    Ship(Vector2 p, Vector2 v) {
        pos = p;
        vel = v;
    }

    void newOrbit(float GM = 10000000) {
        
        OrbitalEntity::newOrbit(GM);
        
        float startT = orbit.startT;
        float dir = orbit.dir;

        float step = PI / 60.0;
        startT -= fmod(startT , step);
        if (dir == 1) startT += step;

        orbitPoints.clear();
        for (float t = -PI; t < PI; t += step) {
            Vector2 point = parametricOrbit(orbit, t);
            orbitPoints.push_back(point);
        }
        t = 0;
    }


    void render(float zoomLevel = 1) {
        for (auto &it: orbitPoints) {
            DrawCircle(it.x, it.y, 1 / zoomLevel, GRAY);
        }

        Vector2 v1 = {0,13};
        Vector2 v2 = {8,-13};
        Vector2 v3 = {-8,-13};

        v1 = rotate(v1, rot) + pos;
        v2 = rotate(v2, rot) + pos;
        v3 = rotate(v3, rot) + pos;

        DrawTriangle(v1,v2,v3, BLUE);

   
    }

    vector<Vector2> getPoly() {
        vector<Vector2> res;
        Vector2 v1 = {0,13};
        Vector2 v2 = {8,-13};
        Vector2 v3 = {-8,-13};

        v1 = rotate(v1, rot) + pos;
        v2 = rotate(v2, rot) + pos;
        v3 = rotate(v3, rot) + pos;
        res = {v1, v2, v3};
        return res;
    }

    Particle getParticle() {
        
        float xPos = randBw(-2,2);

        Vector2 pPos = {xPos,-13};
        pPos = rotate(pPos, rot) + pos;
        Vector2 pVel = {0, -250};
        pVel = rotate(pVel, rot) + vel;

        Particle p = Particle(pPos, pVel,YELLOW, RED,1.5);
        p.ttl = randBw(0.3, 0.6);

        return p;
    }

    void addExplosionParticles(vector<Particle> &particles) {

        for (int i = 0; i < 100; i++) {
            Vector2 pVel = vel + rotate({randBw(125,250) , 0} , randBw(0, 2*PI));
            Vector2 pPos = pos;
            float pRadius = randBw(1 , 3);
            
 ;
            Particle p = Particle(pPos, pVel, YELLOW, RED, pRadius);
            p.ttl = randBw(1 , 2);
            particles.push_back(p);
        }
    }

    Bullet getBullet() {
        Vector2 bPos = pos + rotate({0,10} , rot);
        Vector2 bVel = vel + rotate({0,350}, rot);

        return Bullet(bPos, bVel);
    }

};

#endif