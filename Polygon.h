#ifndef POLYGON_H
#define POLYGON_H
#include "Shapes.h"

class Polygon_ : public Shape
{
private:
    string points;
    vector<pair<float, float>> allPoints;

public:
    string chuanhoa1(string);
    void parsePoints1(string, vector<pair<float, float>>&);
    Polygon_(const string&, float, float, RGB, RGB, float, Transform, string, string);
    void Draw(Graphics&, vector<Defs*>&) override;
};

#endif // !POLYGON_H