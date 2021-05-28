#ifndef UIELEMENT_H
#define UIELEMENT_H


class UIElement {
    public:
    virtual void render() = 0;
    bool intersects(int x, int y) {return false;}

};


#endif