#include "Line.h"

Line::Line(float x1, float y1, float x2, float y2, float strokeOpacity, RGB strokeRGB, float strokeWidth, Transform transform, string stroke)
    : Shape(strokeOpacity, strokeRGB, strokeWidth, transform, stroke), x1(x1), y1(y1), x2(x2), y2(y2) {}

void Line::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    GraphicsState state = TransformSVG(graphics, transform);
    Pen linePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
    graphics.DrawLine(&linePen, x1, y1, x2, y2);
    graphics.Restore(state);
}