#ifndef SHAPE_H
#define SHAPE_H

#include "Defs.h"

class Shape
{
protected:
    float fillOpacity, strokeOpacity, strokeWidth;
    RGB fillRGB, strokeRGB;
    Transform transform;
    string fill, stroke;

public:
    Shape();
    Shape(vector<string>);
    Shape(float, RGB, float, Transform, string);//Line

    Shape(RGB, RGB, float, float, float, Transform, string, string);
    GraphicsState TransformSVG(Graphics&, Transform);
    virtual void Draw(Graphics&, vector<Defs*>&) = 0;
};

#endif // !SHAPES_H