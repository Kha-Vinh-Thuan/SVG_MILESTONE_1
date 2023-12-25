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

public:
    ClassPath(float, float, float, RGB, RGB, Transform, Path, string, string);
    void Draw(Graphics&, vector<Defs*>&) override;
};

#endif // !PATH_H