#ifndef COLOR_H
#define COLOR_H
/*################ COULEUR ################*/

struct Color3f {
    float r;
    float g;
    float b;
};

Color3f createColor(float r, float g, float b);
Color3f multColor(Color3f c, float a);
Color3f multColors(Color3f c1, Color3f c2);

#endif // COLOR_H