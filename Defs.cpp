#include"Defs.h"

Stop::Stop(float offset, float stopOpacity, RGB stopColor) :
    offset(offset), stopOpacity(stopOpacity), stopColor(stopColor) {}
float Stop::getoffset()
{
    return offset;
}
float Stop::getstopOpacity()
{
    return stopOpacity;
}
int Stop::getstopColor_red()
{
    return stopColor.r;
}
int Stop::getstopColor_green()
{
    return stopColor.g;
}
int Stop::getstopColor_blue()
{
    return stopColor.b;
}
Gradient::Gradient() {}

Gradient::Gradient(string id) :id(id) {}

Defs::Defs(vector<LinearGradient*> LinearGradient_list, vector<RadialGradient*> RadialGradient_list) :
    LinearGradient_list(LinearGradient_list), RadialGradient_list(RadialGradient_list) {}
Defs::Defs() {}

LinearGradient::LinearGradient(string id, pointLinearGradient point, vector<Stop*> stop_list, Transform transform) :
    Gradient(id), point(point), stop_list(stop_list), transform(transform) {}

vector<LinearGradient*> Defs::getlinear()
{
    return LinearGradient_list;
}
Transform LinearGradient::gettransform()
{
    return transform;
}
vector<RadialGradient*> Defs::getradial()
{
    return RadialGradient_list;
}

RadialGradient::RadialGradient(string id, float cx, float cy, float r, string xlink_href, Transform transform, float fx, float fy, vector<Stop*> stop_list) :
    Gradient(id), cx(cx), cy(cy), r(r), xlink_href(xlink_href), fx(fx), fy(fy), transform(transform), stop_list(stop_list) {}


string LinearGradient::getID()
{
    return id;
}
vector<Stop*> LinearGradient::getStopList()
{
    return stop_list;
}
pointLinearGradient LinearGradient::getPoint()
{
    return point;
}
string RadialGradient::getID()
{
    return id;
}

float RadialGradient::getcx()
{
    return cx;
}
float RadialGradient::getcy()
{
    return cy;
}
float RadialGradient::getfx()
{
    return fx;
}
float RadialGradient::getfy()
{
    return fy;
}
float RadialGradient::getr()
{
    return r;
}
string RadialGradient::getxlink()
{
    return xlink_href;
}
Transform RadialGradient::gettransform()
{
    return transform;
}
vector<Stop*> RadialGradient::getStopList()
{
    return stop_list;
}