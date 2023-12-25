#ifndef LINE_H
#define LINE_H
#include "Shapes.h"

class Line : public Shape
{
private:
    float x1, y1, x2, y2;

public:
    Line(float, float, float, float, float, RGB, float, Transform, string);
    void Draw(Graphics&, vector<Defs*>&) override;
};

#endif // !LINE_H