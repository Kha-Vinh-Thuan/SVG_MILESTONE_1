#ifndef POLYLINE_H
#define POLYLINE_H
#include "Shapes.h"

class Polyline_ : public Shape
{
private:

    string points;
    vector<pair<float, float>> allPoints;

public:
    string chuanhoa2(string);
    void parsePoints2(string, vector<pair<float, float>>&);
    Polyline_(const string&, float, float, float, RGB, RGB, Transform, string, string);
    void Draw(Graphics&, vector<Defs*>&) override;
};

#endif // !POLYLINE_H