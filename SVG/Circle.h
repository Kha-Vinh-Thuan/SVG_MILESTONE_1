#ifndef CIRCLE_H
#define CIRCLE_H
#include "Elip.h"
class Circle : public Ellipse_
{
private:
    float r;

public:
    Circle(float, float, float, float, float, RGB, RGB, float, Transform, string, string);

    void Draw(Graphics&, vector<Defs*>&) override;
};
#endif // !CIRCLE_H