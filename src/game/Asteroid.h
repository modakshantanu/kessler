#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "Particle.h"
#include "physics.h"
using namespace std;

class Asteroid : public OrbitalEntity {
public:

    vector<Vector2> points;
    int size = 3;
    float angVel = 0;
    float rot = 1;
    float maxRadius = 0;
    bool collided = false;
    float cooldown = 1;

    Asteroid() {
        pos = {0,0};
        vel = {0,0};

    }
    Asteroid(Vector2 p, Vector2 v, int s = 3) {
        pos = p;
        vel = v;
        size = s;
    }   

    void generateRandom(Vector2 shipPos, float zoneRadius) {
        float GM = 10000000;
        generateShape();
        // Should not spawn too close to player's ship
        pos = {randBw(250, zoneRadius * 0.5) , 0};
        float circularVel = sqrt(GM / pos.x);
        vel = {0, -randBw(circularVel * 0.9, circularVel * 1.20)};
        vel = rotate(vel, randBw(- PI / 8, PI / 8));

        if (rand() % 10 == 0) {
            vel = -vel;
        }

        float shipAngle = atan2(shipPos.y, shipPos.x);

        float angel = randBw(shipAngle + PI /2, shipAngle + 3*PI/2);
        pos = rotate(pos, angel);
        vel = rotate(vel, angel);
        
        newOrbit();
    }
    
    void update(float frameTime) {
        OrbitalEntity::update(frameTime);
        rot += angVel * frameTime;
        cooldown = max(0.0f, cooldown - frameTime);
    }

    void render() { 
        
        for (unsigned i = 0; i < points.size() - 1; i++) {
            Vector2 v1 = pos + rotate(points[i], rot); 
            Vector2 v2 = pos + rotate(points[i + 1], rot); 
            DrawLineV(v1, v2, WHITE);
        }

        Vector2 v1 = pos + rotate(points.back(), rot); 
        Vector2 v2 = pos + rotate(points[0], rot); 
        DrawLineV(v1, v2, WHITE);
    }

    vector<Vector2> getPoly() {
        vector<Vector2> res;
        for (unsigned i = 0; i < points.size(); i++) {
            res.push_back(pos + rotate(points[i], rot));
        } 
        return res;
    }

    vector<Asteroid> split() {
        if (size <= 1) {
            return {};
        }
        Vector2 pos1 = pos;
        Vector2 pos2 = pos;
        Vector2 vel1 = rotate(vel, PI / 8);
        Vector2 vel2 = rotate(vel, -PI / 8);

        int newSize = size - 1;

        Asteroid a1, a2;
        a1.size = a2.size = newSize;

        a1.generateShape();        
        a2.generateShape();

        a1.pos = pos1;
        a1.vel = vel1;
        a2.pos = pos2;
        a2.vel = vel2;

        a1.newOrbit();
        a2.newOrbit();

        return {a1, a2};
    }

    void generateShape() {
        Vector2 unit = {1,0};
        int vertices = size + 3;
        float avgRadius = sqrt(75 * (1<<size));

        for (float t = 0; t < 2*PI - 0.01; t += 2 * PI / vertices) {
            float d = avgRadius * randBw(0.7, 1.4);
            maxRadius = max(maxRadius , d);
            points.push_back(rotate(unit * d, t));
        }

        angVel = randBw(-5,5);
    }

    
    void addExplosionParticles(vector<Particle> &particles) {

        for (int i = 0; i < 20; i++) {
            Vector2 pVel = vel*1.5 + rotate({randBw(20,40) , 0} , randBw(0, 2*PI));
            Vector2 pPos = pos;
            float pRadius = randBw(1 , 2);

            Particle p = Particle(pPos, pVel, GRAY, GRAY, pRadius);
            p.ttl = randBw(0.75, 1.5);
            p.decay = 0.99;
            particles.push_back(p);
        }
    }


};

Asteroid getRandomAsteroid(Vector2 shipPos, float zoneRadius, int size = 3) {
    Asteroid a;
    a.size = size;
    a.generateRandom(shipPos, zoneRadius);
    return a;
}

#endif