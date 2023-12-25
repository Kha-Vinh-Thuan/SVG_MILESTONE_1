#ifndef GROUP_H
#define GROUP_H
#include "Shapes.h"

class Group : public Shape
{
private:
    vector<Shape*> shapes;
    int fontSize;

public:
    Group(vector<Shape*>&, float, float, RGB, RGB, float, Transform, int, string, string);
    void Draw(Graphics&, vector<Defs*>&) override;
};

#endif // !GROUP_H