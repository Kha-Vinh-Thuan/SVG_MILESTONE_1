#ifndef PATH_H
#define PATH_H
#define _USE_MATH_DEFINES
#include "Shapes.h"

struct Path
{
    vector<float> value;
    vector<char> type;
};

class ClassPath : public Shape
{
private:
    Path path;
    float width_out, height_out;

public:
    ClassPath(float, float, float, RGB, RGB, Transform, Path, string, string, float, float);
    void Draw(Graphics&, vector<Defs*>&) override;
};

#endif // !PATH_H