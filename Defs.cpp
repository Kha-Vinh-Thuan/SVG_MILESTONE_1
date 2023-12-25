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

Gradient::Gradient(string id, string units):id(id), gradientUnits(units) {}

Defs::Defs(vector<LinearGradient*> LinearGradient_list, vector<RadialGradient*> RadialGradient_list) :
    LinearGradient_list(LinearGradient_list), RadialGradient_list(RadialGradient_list) {}
Defs::Defs() {}

LinearGradient::LinearGradient(string id, pointLinearGradient point, string spreadMethod, string gradientUnits, vector<Stop*> stop_list) :
    Gradient(id, gradientUnits), point(point), spreadMethod(spreadMethod), stop_list(stop_list) {}

vector<LinearGradient*> Defs::getlinear()
{
    return LinearGradient_list;
}

RadialGradient::RadialGradient(string id, float cx, float cy, float r, string gradientUnits, string xlink_href, float fx, float fy) :
    Gradient(id, gradientUnits), cx(cx), cy(cy), r(r), xlink_href(xlink_href), fx(fx), fy(fy) {}

void LinearGradient::DrawDef(Graphics& graphics)
{
   /* size_t numColors = min(stop_list.size(), static_cast<size_t>(3));    
    Color* colors = new Color[numColors];

    for (size_t i = 0; i < numColors; ++i)
    {
        colors[i] = Color(stop_list[i]->getstopColor_red(), stop_list[i]->getstopColor_green(), stop_list[i]->getstopColor_blue());
    }
    LinearGradientBrush gradientBrush(PointF(point.x1, point.y1), PointF(point.x2, point.y2), colors[0], colors[numColors - 1]);

    REAL* positions = new REAL[numColors];
    for (size_t i = 0; i < numColors; ++i)
    {
        positions[i] = stop_list[i]->getoffset();
    }
    gradientBrush.SetInterpolationColors(colors, positions, numColors);*/
}

void LinearGradient::Draww(LinearGradientBrush*& linear)
{
    size_t numColors = min(stop_list.size(), static_cast<size_t>(3));

    Color* colors = new Color[numColors];

    for (size_t i = 0; i < numColors; ++i)
    {
        colors[i] = Color(stop_list[i]->getstopColor_red(), stop_list[i]->getstopColor_green(), stop_list[i]->getstopColor_blue());
    }

    LinearGradientBrush gradientBrush(PointF(point.x1, point.y1), PointF(point.x2, point.y2), colors[0], colors[numColors - 1]);

    REAL* positions = new REAL[numColors];
    for (size_t i = 0; i < numColors; ++i)
    {
        positions[i] = stop_list[i]->getoffset();
    }

    gradientBrush.SetInterpolationColors(colors, positions, numColors);

    linear = &gradientBrush;


    delete[] colors;
    delete[] positions;
}

string LinearGradient::getID()
{
    return id;
}
string RadialGradient::getID()
{
    return id;
}
void RadialGradient::DrawDef(Graphics&) {}

