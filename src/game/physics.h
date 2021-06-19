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

// struct BoundingBox {
//     Vector2 tl;
//     Vector2 br;
// };


void print(Orbit o) {
    printf("%f,%f  a=%f b=%f angle=%f starT = %f, dir=%d\n", o.f2.x, o.f2.y, o.a, o.b, o.angle, o.startT, o.dir);
}

void print(BoundingBox &b) {
    printf("Bounding Box (%f %f) , (%f %f)\n", b.min.x, b.min.y, b.max.x, b.max.y);
}

void print(Color c) {
    printf("%d %d %d %d\n", c.r, c.g, c.b, c.a);
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
    
    return {res , velUnit};
}

Vector2 parametricOrbit(Orbit &o, float t) {
    t = o.startT + o.dir * t;
    Vector2 p;
    if (o.a >= 0) {
        p = {o.f2.x / 2 + o.a * cos(t), o.b * sin(t)};
    } else {
        p = {o.f2.x / 2 + o.a * cosh(t) , o.b * sinh(t)};
    }
    return rotate(p, o.angle);
}

Vector2 gravAcc(Vector2 p, Vector2 s, float GM) {

    float r2 = magsq(p - s);
    float res = GM / r2;
    return unit((p - s)) * res;
}

float slope(Vector2 start, Vector2 end) {
    return (end.y - start.y) / (end.x - start.x);
}

void pVec(string s, Vector2 v) {
    printf("%s: %f %f\n", s.c_str(), v.x, v.y);
}

bool isNan(Vector2 v) {
    return isnan(v.x) || isnan(v.y);
}

float randBw(float low, float high) {
    return low + (((float)rand()) / RAND_MAX) * (high - low);
}



Color gradient(Color a, Color b, float t) {
    float s = 1-t;
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnarrowing"

    Color res = {
        a.r * s + b.r * t,
        a.g * s + b.g * t,
        a.b * s + b.b * t,
        a.a * s + b.a * t
    };

    #pragma GCC diagnostic pop

    return res;
}
bool bbIntersects(BoundingBox &a, BoundingBox &b) {
    if (a.min.x > b.max.x || b.min.x > a.max.x) return false;
    if (a.max.y < b.min.y || b.max.y < a.min.y) return false;
    return true;
}

int orientation(Vector2 &p, Vector2 &q, Vector2 &r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);
 
    if (val == 0) return 0; // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool doIntersect(Vector2 &p1, Vector2 &q1, Vector2 &p2, Vector2 &q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    return false;
}

bool polyIntersects(vector<Vector2> &a, vector<Vector2> &b) {
    
    for (unsigned i = 0; i < a.size(); i++) {
        Vector2 a1 = a[i];
        Vector2 a2 = a[(i+1)%a.size()];

        for (unsigned j = 0; j < b.size(); j++) {
            Vector2 b1 = b[j];
            Vector2 b2 = b[(j+1) % b.size()];

            if (doIntersect(a1, a2, b1, b2)) return true;
        }
    }
    return false;
}

BoundingBox getBb(vector<Vector2> &poly) {
    float minX = 1e9;
    float minY = 1e9;
    float maxX = -1e9;
    float maxY = -1e9;

    for (auto &it: poly) {
        float x = it.x;
        float y = it.y;
        minX = min(minX , x);
        minY = min(minY , y);
        maxX = max(maxX,  x);
        maxY = max(maxY , y);
    }

    return {{minX, minY} , {maxX, maxY}};
}

bool bbCircleIntersects(Vector2 &center, float radius, BoundingBox &b) {

    Vector2 p1 = {b.min.x, b.min.y};
    Vector2 p2 = {b.max.x, b.max.y};
    Vector2 p3 = {p1.x, p2.y};
    Vector2 p4 = {p2.x, p1.y};

    if (mag(p1 - center) < radius) return true;
    if (mag(p2 - center) < radius) return true;
    if (mag(p3 - center) < radius) return true;
    if (mag(p4 - center) < radius) return true;
    return false;

}

bool polyCircleIntersects(Vector2 center, float radius, vector<Vector2> &poly) {
    for (auto &p: poly) {
        if (mag(p - center) < radius) return true;
    }
    return false;
}

bool circleCircleIntersects(Vector2 c1, float r1, Vector2 c2, float r2) {
    return mag(c1 - c2) < r1 + r2;
}

bool pointPolyIntersects(Vector2 p, vector<Vector2> &poly) {
    Vector2 inf = {0,0};
    int count = 0;
    for (unsigned i = 0; i < poly.size(); i++) {
        Vector2 a1 = poly[i];
        Vector2 a2 = poly[(i+1)%poly.size()];
        if (doIntersect(a1,a2,p, inf)) {
            count++;
        }
    }
    return (count % 2 == 1);

}

bool pointBbIntersects(Vector2 p, BoundingBox bb) {
    if (p.x >= bb.min.x && p.x <= bb.max.x 
            && p.y >= bb.min.y && p.y <= bb.max.y) {
        return true;
    }

    return false;
}


bool lineCircleIntersects(Vector2 start, Vector2 end, Vector2 cx, float r) {
    float d = dot(end - start,cx - start) / mag(end - start);

    Vector2 proj = start +  unit(end - start) * d;
    if (mag(proj - cx) > r) return false;

    if (proj.x > start.x && proj.x > end.x) return false; 
    if (proj.x < start.x && proj.x < end.x) return false;
    return true;
}

bool circlePolyIntersects(vector<Vector2> &poly, Vector2 cx, float r) {
    if (pointPolyIntersects(cx, poly)) return true;

    for (unsigned i = 0; i < poly.size(); i++) {
        Vector2 start = poly[i];
        Vector2 end = poly[(i+1)%poly.size()];

        if (lineCircleIntersects(start,end, cx, r)) return true;
    }

    return false;
}

#endif  