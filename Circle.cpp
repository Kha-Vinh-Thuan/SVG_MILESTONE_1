#include "Circle.h"

Circle::Circle(float cx, float cy, float r, float fillOpacity, float strokeOpacity, RGB fillRGB, RGB strokeRGB, float strokeWidth, Transform transform, string fill, string stroke)
    : Ellipse_(cx, cy, r, r, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fill, stroke), r(r) {}

void Circle::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    SolidBrush circleBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
    Pen circlePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

    GraphicsState state = TransformSVG(graphics, transform);
    graphics.FillEllipse(&circleBrush, cx - r, cy - r, 2 * r, 2 * r);
    graphics.DrawEllipse(&circlePen, cx - (r + strokeWidth / 2), cy - (r + strokeWidth / 2), 2 * (r + strokeWidth / 2), 2 * (r + strokeWidth / 2));
    graphics.Restore(state);
}