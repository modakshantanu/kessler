#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "physics.h"

class Asteroid : public OrbitalEntity {
public:

    vector<Vector2> points;
    int size = 3;
    float angVel = 0;
    float rot = 1;
    float maxRadius = 0;
    bool collided = false;

    Asteroid() {
        pos = {0,0};
        vel = {0,0};

    }
    Asteroid(Vector2 p, Vector2 v, int s = 3) {
        pos = p;
        vel = v;
        size = s;
    }   

    void generateRandom(Vector2 shipPos, float planetRadius = 150, float GM = 4500000) {
        Vector2 unit = {1,0};
        int vertices = size + 3;
        float avgRadius = sqrt(50 * (1<<size));

        for (float t = 0; t < 2*PI - 0.01; t += 2 * PI / vertices) {
            float d = avgRadius * randBw(0.7, 1.4);
            maxRadius = max(maxRadius , d);
            points.push_back(rotate(unit * d, t));
        }

        angVel = randBw(-5,5);
        // Should not spawn too close to player's ship
        pos = {randBw(200, 300) , 0};
        float circularVel = sqrt(GM / pos.x);
        vel = {0, -randBw(circularVel, circularVel * 1.20)};
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

};

#endif