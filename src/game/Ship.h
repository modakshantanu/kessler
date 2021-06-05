#ifndef SHIP_H
#define SHIP_H

#include "raylib.h"
#include "OrbitalEntity.h"
#include "physics.h"

class Ship : public OrbitalEntity {
public:
    
    vector<Vector2> orbitPoints;
    bool moved = true;

    Ship() {}
    Ship(Vector2 p, Vector2 v) {
        pos = p;
        vel = v;
    }

    void newOrbit(float GM = 4500000) {
        // printf("initial vel = %f %f\n", vel.x, vel.y);
        Vector2 vel2 = vel;
        int dir = -1;
        
        if (cross(pos, pos + vel) > 0) {
            vel2 = -vel;
            dir = 1;
        } 

        float a = 1.0 / (2.0/mag(pos) - ((double)magsq(vel2)) / GM);
        // pVec("Vel2" , vel2);
        // printf("a = %f\n",a);

        float angle = acos(dot(pos, vel2) / (mag(pos) * mag(vel2)));
        // printf("Vec angle = %f\n", angle);

        Vector2 f2unit; 

        if (a >= 0) { // Ellipse
            f2unit = unit(rotate(vel2, -angle));
        } else { // Hyperbola
            f2unit = unit(rotate(vel2, PI - angle));
        }

        // pVec("f2unit" , f2unit);
        float dist;

        if (a >= 0) { // Ellipse
            dist = 2*a - mag(pos);
        } else { // Hyperbola
            dist = mag(pos) - 2*a;
            // printf("dist %f\n", dist);
        }

        Vector2 f2 = pos + f2unit*dist;


        float ellipseAngle = atan2(f2.y, f2.x);
        // printf("f2 = %f %f\n", f2.x, f2.y);
        // printf("angle = %f\n", ellipseAngle);



        Vector2 startN = rotate(pos, -ellipseAngle);
        Vector2 vel2N = rotate(vel2, -ellipseAngle);
        

        f2 = {mag(f2), 0}; // Normalise along x axis
        
        // printf("startN = %f %f    velN = %f %f f2 = %f %f\n ", startN.x, startN.y, vel2N.x, vel2N.y, f2.x, f2.y);
        
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
            // pVec("v1" , v1);
            // pVec("v2" , v2);
            float s1 = slope(v1 , startN);
            float s2 = slope(v2, startN);
            // printf("s1 = %f, s2 = %f\n", s1, s2); 
            b = sqrt(s1 * s2 * a * a);

            // printf("b = %f\n", b);

        }




        // printf("%f %f %f %f\n", rmin, rmax, a, b);
        // dir = cross(startN, startN + vel2N) <= 0? -1 : 1;
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
        // printf("Orbit dir = %d\n", dir);
        
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


    void update(float elapsedTime) {
        
        float displacement = mag(vel) * elapsedTime;
        float dt = 0.001;
        float dx = mag(parametricOrbit(orbit, t + dt) - parametricOrbit(orbit, t));
        float nextT = t + (displacement * dt / dx);
        // printf("%f %f\n",t, nextT);
        Vector2 nextPos = parametricOrbit(orbit, nextT);
        Vector2 nextVel = unit(parametricOrbit(orbit, nextT + dt) - parametricOrbit(orbit, nextT));
        nextVel = nextVel * sqrt(orbit.GM * (2/mag(nextPos) - 1/orbit.a));

        pos = nextPos;
        vel = nextVel;
        t = nextT;
        // printf("vel = %f %f\n", vel.x, vel.y);
        // printf("t = %f\n", t);
    }

    void render() {
        for (auto &it: orbitPoints) {
            DrawPixel(it.x, it.y, GRAY);
        }

        // Debugging f2
        // auto o = rotate(orbit.f2, orbit.angle);
        // DrawCircle(o.x, o.y, 2, RED);

        Vector2 v1 = {0,8};
        Vector2 v2 = {5,-8};
        Vector2 v3 = {-5,-8};

        v1 = rotate(v1, rot) + pos;
        v2 = rotate(v2, rot) + pos;
        v3 = rotate(v3, rot) + pos;

        DrawTriangle(v1,v2,v3, BLUE);

   
    }

};

#endif