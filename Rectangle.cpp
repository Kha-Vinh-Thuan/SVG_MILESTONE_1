#include "Rectangle.h"



Rect_::Rect_(float x, float y, float width, float height, float fillOpacity, float strokeOpacity, RGB fillRGB, RGB strokeRGB, float strokeWidth, Transform transform, string fill, string stroke)
    : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke), x(x), y(y), width(width), height(height) {}

void Rect_::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    SolidBrush RectBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
    Pen RectBorderPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

    GraphicsState state = TransformSVG(graphics, transform);
    
    graphics.DrawRectangle(&RectBorderPen, x, y, width, height);

    if (!fill.empty())
    {
        LinearGradient* linearGradient = nullptr;

        for (Defs* def : defs)
        {
            vector<LinearGradient*> vectorlinear = def->getlinear();
            for (LinearGradient* lin : vectorlinear)
            {
                if (lin->getID() == fill) {
                    linearGradient = lin;
                    break;
                }
            }
        }

        if (linearGradient != nullptr)
        {
            LinearGradientBrush* RectLinearBrush = nullptr;
            linearGradient->Draww(RectLinearBrush);

            graphics.FillRectangle(RectLinearBrush, x, y, width, height);
        }

        if (linearGradient != nullptr)
            delete linearGradient;
    }
    else
    {
        graphics.FillRectangle(&RectBrush, x, y, width, height);
    }

    graphics.Restore(state);
}