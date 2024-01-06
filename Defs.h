#ifndef DEFS_H
#define DEFS_H

#include "Header.h"

struct RGB
{
    int r, g, b;
};

struct Transform
{
    float translateX, translateY, rotateAngle, scaleX, scaleY;
    float skewX , skewY;
    vector<string> transformOrder;
};
struct groupChild
{
    float fontSize;
    float strokeOpacity, fillOpacity, strokeWidth;
    RGB strokeRGB, fillRGB;
    Transform transform;
};


struct pointLinearGradient
{
    float x1, x2, y1, y2;
};

class LinearGradient;
class RadialGradient;

class Stop {
private:
    float offset, stopOpacity;
    RGB stopColor;
public:
    Stop(float offset, float stopOpacity, RGB stopColor);
    float getoffset();
    float getstopOpacity();
    int getstopColor_red();
    int getstopColor_green();
    int getstopColor_blue();
};
class Gradient
{
protected:
    string id;
public:
    Gradient();
    Gradient(string);
    virtual string getID() = 0;
};

class Defs
{
private:
    vector<LinearGradient*> LinearGradient_list;
    vector<RadialGradient*> RadialGradient_list;
public:
    Defs(vector<LinearGradient*>, vector<RadialGradient*>);
    Defs();
    vector<LinearGradient*> getlinear();
    vector<RadialGradient*> getradial();
};

class LinearGradient : public Gradient
{
private:
    pointLinearGradient point;
    vector<Stop*> stop_list;
    Transform transform;
public:
    LinearGradient(string, pointLinearGradient, vector<Stop*>, Transform);
    string getID() override;
    vector<Stop*> getStopList();
    pointLinearGradient getPoint();
    Transform gettransform();
};

class RadialGradient : public Gradient
{
private:
    float cx, cy, r, fx, fy;
    string xlink_href;
    Transform transform;
    vector<Stop*> stop_list;
public:
    RadialGradient(string, float, float, float, string, Transform, float, float, vector<Stop*>);
    string getID() override;
    vector<Stop*> getStopList();
    float getcx();
    float getcy();
    float getr();
    float getfx();
    float getfy();
    string getxlink();
    Transform gettransform();

};
#endif // !DEFS_H