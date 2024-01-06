#include "Polyline.h"

string Polyline_::chuanhoa2(string input)
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
void Polyline_::parsePoints2(string s, vector<pair<float, float>>& allPoints)
{

    int i = 0;
    s = chuanhoa2(s);
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


Polyline_::Polyline_(const string& points, float fillOpacity, float strokeOpacity, float strokeWidth, RGB fillRGB, RGB strokeRGB, Transform transform, string fill, string stroke)
    : points(points), Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke)
{
    parsePoints2(points, allPoints);
}

void Polyline_::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    vector<PointF> pointsArray;
    for (const auto& point : allPoints)
    {
        pointsArray.push_back(PointF(static_cast<float>(point.first), static_cast<float>(point.second)));
    }

    PointF* pointsArrayPtr = pointsArray.data();
    int numPoints = static_cast<int>(pointsArray.size());

    Pen polylinePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
    SolidBrush polylineBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
    GraphicsState state = TransformSVG(graphics, transform);

    if (numPoints > 0)
    {
        if (fillRGB.r == 256 && fillRGB.b == 256 && fillRGB.g == 256)
            graphics.DrawLines(&polylinePen, pointsArrayPtr, numPoints);
        else
        {
            graphics.FillPolygon(&polylineBrush, pointsArrayPtr, numPoints);
            graphics.DrawLines(&polylinePen, pointsArrayPtr, numPoints);
        }
    }
    graphics.Restore(state);
}