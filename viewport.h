#ifndef VIEWPORT_H
#define VIEWPORT_H

struct Viewport {
    int x, y, width, height;

    Viewport()
        : x(0), y(0), width(800), height(600)
    {}

    Viewport(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height)
    {}

    void toClipSpace(float x, float y,
                     float &xc, float &yc)
    {
        xc = x/(width*0.5f) - 1.0f;
        yc = 1.0f - y/(height*0.5f);
    }
};

#endif
