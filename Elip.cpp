#include "Elip.h"

Ellipse_::Ellipse_(float cx, float cy, float rx, float ry, float fillOpacity, float strokeOpacity, float strokeWidth, RGB fillRGB, RGB strokeRGB, Transform transform, string fill, string stroke)
    : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke), cx(cx), cy(cy), rx(rx), ry(ry) {}

void Ellipse_::Draw(Graphics& graphics, vector<Defs*>& defs)
{

    Pen ellipsePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
    SolidBrush ellipseBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));

    GraphicsState state = TransformSVG(graphics, transform);
    graphics.FillEllipse(&ellipseBrush, cx - rx, cy - ry, 2 * rx, 2 * ry);
    graphics.DrawEllipse(&ellipsePen, cx - rx, cy - ry, 2 * rx, 2 * ry);
    graphics.Restore(state);
}