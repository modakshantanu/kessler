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
    

    void newOrbit(float GM = 4500000) {
        float a = 1.0 / (2.0/mag(pos) - magsq(vel) / GM);
        // printf("a = %f\n",a);
        float angle = acos(dot(pos, vel) / (mag(pos) * mag(vel)));

        // printf("angle = %f\n", angle);

        Vector2 f2unit = unit(rotate(vel, -angle));


        float dist = 2*a - mag(pos);
        Vector2 f2 = pos + f2unit*dist;

        // printf("f2 = %f %f\n", f2.x, f2.y);
        float ellipseAngle = atan2(f2.y, f2.x);



        Vector2 startN = rotate(pos, -ellipseAngle);
        Vector2 velN = rotate(vel, -ellipseAngle);
        
        int dir = cross(startN, startN + velN) <= 0? -1 : 1;

        f2 = {mag(f2), 0}; // Normalise along x axis
        float fA = f2.x / 2.0;
        float rmin = a - fA;
        float rmax = 2*a - rmin;
        float b = sqrt(rmin * rmax);

        // printf("%f %f %f %f\n", rmin, rmax, a, b);

        float startT = asin(startN.y / b);
        Vector2 test1 = {fA + a * cos(startT) , b * sin(startT)};
        Vector2 test2 = {fA + a * cos(PI - startT) , b * sin(PI - startT)};

        startT = mag(startN - test1) < mag(startN - test2) ?startT : PI - startT; 

        orbit = Orbit{f2, ellipseAngle, GM, a, b, dir, startT};
        
        float step = PI / 60.0;
        startT -= fmod(startT , step);
        if (dir == 1) startT += step;
        t = 0;
    }


    void update(float elapsedTime) {
        
        float displacement = mag(vel) * elapsedTime;
        float dt = 0.001;
        float dx = mag(parametricOrbit(orbit, t + dt) - parametricOrbit(orbit, t));
        float nextT = t + displacement * dt / dx;
        // printf("%f %f\n",t, nextT);
        Vector2 nextPos = parametricOrbit(orbit, nextT);
        Vector2 nextVel = unit(parametricOrbit(orbit, nextT + dt) - parametricOrbit(orbit, nextT));
        nextVel = nextVel * sqrt(orbit.GM * (2/mag(nextPos) - 1/orbit.a));

        pos = nextPos;
        vel = nextVel;
        t = nextT;

    }



    void render() { 
        DrawCircle(pos.x, pos.y, 5, DARKGRAY);
    }

};

#endif