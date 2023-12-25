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
    string id, gradientUnits;
public:
    Gradient();
    Gradient(string, string);
    virtual void DrawDef(Graphics&) = 0;
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
};

class LinearGradient : public Gradient
{
private:
    pointLinearGradient point;
    string spreadMethod;
    vector<Stop*> stop_list;
public:
    LinearGradient(string, pointLinearGradient, string, string, vector<Stop*>);
    void DrawDef(Graphics&) override;
    string getID() override;
    void Draww(LinearGradientBrush*&);
};

class RadialGradient : public Gradient
{
private:
    float cx, cy, r, fx, fy;
    string xlink_href;
public:
    RadialGradient(string, float, float, float, string, string, float, float);
    void DrawDef(Graphics&) override;
    string getID() override;
};
#endif // !DEFS_H