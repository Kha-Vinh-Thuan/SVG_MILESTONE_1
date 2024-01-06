#ifndef RECT_H
#define RECT_H
#include "Shapes.h"


class Rect_ : public Shape
{
private:
    float x, y, width, height;
public:
    Rect_(float, float, float, float, float, float, RGB, RGB, float, Transform, string, string);
    void Draw(Graphics&, vector<Defs*>&) override;
};
#endif // !RECT_H