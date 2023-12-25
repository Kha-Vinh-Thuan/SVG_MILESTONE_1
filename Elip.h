#ifndef ELIP_H
#define ELIP_H
#include "Shapes.h"

class Ellipse_ : public Shape
{
protected:
    float cx, cy;
    float rx, ry;

public:
    Ellipse_(float, float, float, float, float, float, float, RGB, RGB, Transform, string, string);
    void Draw(Graphics&, vector<Defs*>&) override;
};
#endif // !ELIP_H