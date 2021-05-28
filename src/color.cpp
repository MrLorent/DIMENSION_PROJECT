#include "../include/color.h"

Color3f createColor(float r, float g, float b) {
    Color3f color;

    color.r = r;
    color.g = g;
    color.b = b;
    
    return color;
}

Color3f multColor(Color3f c, float a) {
    c.r *= a;
    c.g *= a;
    c.b *= a;
    return c;
}

Color3f multColors(Color3f c1, Color3f c2) {
    c1.r *= c2.r;
    c1.g *= c2.g;
    c1.b *= c2.b;
    return c1;
}