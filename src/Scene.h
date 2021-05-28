#ifndef SCENE_H
#define SCENE_H

#include <queue>

class Scene {
public:
    virtual void update() = 0;
    virtual void render() = 0;
};

#endif