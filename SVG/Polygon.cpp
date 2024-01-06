#include "Polygon.h"

string Polygon_::chuanhoa1(string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ',')
        {
            input[i] = ' ';
        }
    }
    istringstream iss(input);
    ostringstream oss;
    string word;
    bool first = true;

    while (iss >> word)
    {

        if (!first)
        {
            oss << " ";
        }
        oss << word;
        first = false;
    }
    return oss.str();
}
void Polygon_::parsePoints1(string s, vector<pair<float, float>>& allPoints)
{

    int i = 0;
    s = chuanhoa1(s);
    while (i < s.length())
    {

        if (isdigit(s[i]) || s[i] == '-' || s[i] == '.' || s[i] == ' ')
        {

            size_t next_pos;
            float x = stof(s.substr(i), &next_pos);
            i += next_pos + 1;

            float y = stof(s.substr(i), &next_pos);
            i += next_pos;

            allPoints.push_back(make_pair(x, y));
        }
    }
}

Polygon_::Polygon_(const string& points, float fillOpacity, float strokeOpacity, RGB fillRGB, RGB strokeRGB, float strokeWidth, Transform transform, string fill, string stroke)
    : points(points), Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke)
{
    parsePoints1(points, allPoints);
}

void Polygon_::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    GraphicsState state = TransformSVG(graphics, transform);
    SolidBrush polygonBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
    Pen polygonPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

    vector<PointF> pointsArray;

    for (const auto& point : allPoints)
    {
        pointsArray.push_back(PointF(point.first, point.second));
    }

    PointF* pointsArrayPtr = pointsArray.data();
    int numPoints = static_cast<int>(pointsArray.size());

    if (numPoints > 0)
    {
        graphics.FillPolygon(&polygonBrush, pointsArrayPtr, numPoints, FillModeWinding);
        graphics.DrawPolygon(&polygonPen, pointsArrayPtr, numPoints);
    }
    graphics.Restore(state);
}