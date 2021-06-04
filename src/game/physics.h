#ifndef PHYSICS_H
#define PHYSICS_H

#include "raylib.h"
#include <bits/stdc++.h>
using namespace std;


struct Ellipse {
    Vector2 f1, f2;
    float a;
};

struct Orbit {
    // f1 is fixed at origin
    Vector2 f2; // focus 2
    float angle; // Of un-normalized orbit
    float GM; // for simplicity
    float a,b; // semi-major / minor axis 
    int dir; // cw or ccw
    float startT; 
};

void print(Orbit o) {
    printf("%f,%f  a=%f b=%f angle=%f starT = %f, dir=%d\n", o.f2.x, o.f2.y, o.a, o.b, o.angle, o.startT, o.dir);
}

Vector2 operator+(Vector2 a, Vector2 b) {
    return {a.x + b.x, a.y + b.y};
}
Vector2 operator+=(Vector2 a, Vector2 b) {
    return {a.x + b.x, a.y + b.y};
}
Vector2 operator-(Vector2 a, Vector2 b) {
    return {a.x - b.x, a.y - b.y};
}
Vector2 operator-=(Vector2 a, Vector2 b) {
    return {a.x - b.x, a.y - b.y};
}
Vector2 operator-(Vector2 a) {
    return {-a.x, -a.y};
}
Vector2 operator/(Vector2 a, float n) {
    return {a.x / n, a.y / n};
}
Vector2 operator*(Vector2 a, float n) {
    return {a.x * n, a.y * n};
}
float dot(Vector2 a, Vector2 b) {return a.x * b.x + a.y * b.y;}

double cross(Vector2 a,Vector2 b) {return a.x*b.y - a.y*b.x;}
float magsq(Vector2 a) {return a.x * a.x + a.y * a.y;}
float mag(Vector2 a) {
    return sqrt(a.x * a.x + a.y * a.y);
}
Vector2 unit(Vector2 v) {
    return v / mag(v);
}
Vector2 rotate(Vector2 v, float rads) {
    return {v.x * cos(rads) - v.y*sin(rads) , v.x * sin(rads) + v.y * cos(rads)};
}

Ellipse getEllipse(Vector2 f1, Vector2 p, Vector2 tangent, float a) {
    // Get angle between one focus and tangent
    float angle = acos(dot(p - f1, tangent) / (mag(p - f1) * mag(tangent)));
    // printf("%f\n", angle);
    
    // Use ellipse reflection property to find the direction of the other focus 
    Vector2 f2unit = unit(rotate(tangent, -angle));

    // printf("%f %f\n", f2unit.x, f2unit.y);

    // Traverse along the line 
    float dist = 2*a - mag(p - f1);
    Vector2 f2 = p + f2unit*dist;

    // printf("%f %f , %f %f , %f\n", f1.x, f1.y, f2.x, f2.y, a);
    return Ellipse({f1, f2, a});

}

Vector2 parametricEllipse(Ellipse e, Vector2 start, Vector2 vel, float t, float step = 0.1) {
    float ellipseAngle = atan2(e.f2.y, e.f2.x);
    // printf("Ellipseangle = %f\n", ellipseAngle);

    Ellipse e2 = {e.f1, {mag(e.f1 - e.f2), 0}, e.a};
    
    // printf("%f %f %f %f %f\n", e2.f1.x, e2.f1.y, e2.f2.x, e2.f2.y, e2.a);

    Vector2 start2 = rotate(start, -ellipseAngle);
    vel = unit(rotate(vel, -ellipseAngle));

    float crossy = cross(start2, start2 + vel);
    bool ccw = crossy <= 0;


    float fA = mag(e.f1 - e.f2) / 2.0;
    float rmin = e.a - fA;
    float rmax = 2*e.a - rmin;
    float b = sqrt(rmin * rmax);
    float a = e2.a;

    float startT = asin(start2.y / b);
    // Check if this is the correct one
    Vector2 test1 = {fA + a * cos(startT) , b * sin(startT)};
    Vector2 test2 = {fA + a * cos(PI - startT) , b * sin(PI - startT)};

    startT = mag(start2 - test1) < mag(start2 - test2) ?startT : PI - startT; 
    
    startT -= fmod(startT , step);
    if (!ccw) startT += step;

    Vector2 res;
    if (ccw) {
        res = {fA + a * cos(startT - t), b * sin(startT - t)};
    } else {
        res = {fA + a * cos(startT + t), b * sin(startT + t)};
    }

    res = rotate(res , ellipseAngle);

    return res;
}

pair<Vector2, Vector2> updatePos(Ellipse e, Vector2 start, Vector2 vel, float disp, float GM) {
    float ellipseAngle = atan2(e.f2.y, e.f2.x);

    Ellipse e2 = {e.f1, {mag(e.f1 - e.f2), 0}, e.a};
    

    Vector2 start2 = rotate(start, -ellipseAngle);
    vel = unit(rotate(vel, -ellipseAngle));

    float crossy = cross(start2, start2 + vel);
    bool ccw = crossy <= 0;


    float fA = mag(e.f1 - e.f2) / 2.0;
    float rmin = e.a - fA;
    float rmax = 2*e.a - rmin;
    float b = sqrt(rmin * rmax);
    float a = e2.a;

    float startT = asin(start2.y / b);

    Vector2 test1 = {fA + a * cos(startT) , b * sin(startT)};
    Vector2 test2 = {fA + a * cos(PI - startT) , b * sin(PI - startT)};

    startT = mag(start2 - test1) < mag(start2 - test2) ?startT : PI - startT; 
    float dt = 0.001;

    Vector2 res;
    if (ccw) {
        res = {fA + a * cos(startT - dt), b * sin(startT - dt)};
    } else {
        res = {fA + a * cos(startT + dt), b * sin(startT + dt)};
    }

    float dx = mag(res - start2);
    float t = disp * dt / dx;

    Vector2 velUnit;

    if (ccw) {
        res = {fA + a * cos(startT - t), b * sin(startT - t)};
        Vector2 resB = {fA + a * cos(startT - t - dt), b * sin(startT - t - dt)};
        velUnit = unit(resB - res);
    } else {
        res = {fA + a * cos(startT + t), b * sin(startT + t)};
        Vector2 resB = {fA + a * cos(startT + t + dt), b * sin(startT + t + dt)};
        velUnit = unit(resB - res);
    }

    float velMag = sqrt(GM* (2/mag(res) - 1/a));
    velUnit = velUnit * velMag;

    
    velUnit = rotate(velUnit , ellipseAngle);
    res = rotate(res , ellipseAngle);
    
    // Finding the velocity 



    return {res , velUnit};
}

Vector2 parametricOrbit(Orbit &o, float t) {
    t = o.startT + o.dir * t;
    Vector2 p = {o.f2.x / 2 + o.a * cos(t), o.b * sin(t)};
    return rotate(p, o.angle);
}

Vector2 gravAcc(Vector2 p, Vector2 s, float GM) {

    float r2 = magsq(p - s);
    float res = GM / r2;
    return unit((p - s)) * res;
}





#endif  