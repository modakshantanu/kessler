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




    void newOrbit(float GM = 10000000) {
        
        // Zero vector
        // OR Parallel to major axis
        // OR Perfectly circular
        if (magsq(vel) == 0 
            || abs(dot(pos, vel)) == (mag(pos) * mag(vel)) 
            || mag(vel) == sqrt(GM / mag(pos))) {
            vel.x += 0.2;
            vel.y += 0.2;
        }

        Vector2 vel2 = vel;
        int dir = -1;
        
        if (cross(pos, pos + vel) > 0) {
            vel2 = -vel;
            dir = 1;
        } 

        float a = 1.0 / (2.0/mag(pos) - ((double)magsq(vel2)) / GM);
        float angle = acos(dot(pos, vel2) / (mag(pos) * mag(vel2)));
        Vector2 f2unit; 

        if (a >= 0) { // Ellipse
            f2unit = unit(rotate(vel2, -angle));
        } else { // Hyperbola
            f2unit = unit(rotate(vel2, PI - angle));
        }
        float dist;

        if (a >= 0) { // Ellipse
            dist = 2*a - mag(pos);
        } else { // Hyperbola
            dist = mag(pos) - 2*a;
        }

        Vector2 f2 = pos + f2unit*dist;
        float ellipseAngle = atan2(f2.y, f2.x);
        Vector2 startN = rotate(pos, -ellipseAngle);
        
        f2 = {mag(f2), 0}; // Normalise along x axis
        
        
        float fA = f2.x / 2.0;
        float b;
        float rmin, rmax;
        if (a >= 0) {
            rmin = a - fA;
            rmax = 2*a - rmin;
            b = sqrt(rmin * rmax);
        } else {
            rmin = fA + a;
            Vector2 v1 = {rmin , 0};
            Vector2 v2 = {rmin - 2*a, 0};
            float s1 = slope(v1 , startN);
            float s2 = slope(v2, startN);
            b = sqrt(s1 * s2 * a * a);


        }
        float startT;

        if (a >= 0) { // Ellipse
            startT = asin(startN.y / b);
            Vector2 test1 = {fA + a * cos(startT) , b * sin(startT)};
            Vector2 test2 = {fA + a * cos(PI - startT) , b * sin(PI - startT)};

            startT = mag(startN - test1) < mag(startN - test2) ?startT : PI - startT; 
        } else { // Hyperbola
            startT = asinh(startN.y / b);
        }

        orbit = Orbit{f2, ellipseAngle, GM, a, b, dir, startT};
    

        t = 0;
    }


    void update(float elapsedTime) {
        
        float displacement = mag(vel) * elapsedTime;
        float dt = 0.001;
        float dx = mag(parametricOrbit(orbit, t + dt) - parametricOrbit(orbit, t));
        float nextT = t + (displacement * dt / dx);
        Vector2 nextPos = parametricOrbit(orbit, nextT);
        Vector2 nextVel = unit(parametricOrbit(orbit, nextT + dt) - parametricOrbit(orbit, nextT));
        nextVel = nextVel * sqrt(orbit.GM * (2/mag(nextPos) - 1/orbit.a));

        if (isNan(nextPos) || isNan(nextVel)) {
            return;
        }

        pos = nextPos;
        vel = nextVel;
        t = nextT;
    }

};

#endif