#ifndef UIELEMENT_H
#define UIELEMENT_H

void doNothing() {}

class UIElement {
    public:
    virtual void render() = 0;
    bool intersects(int x, int y) {return false;}
    void (*onClick)() = doNothing;
    void (*onHover)() = doNothing;
};


#endif