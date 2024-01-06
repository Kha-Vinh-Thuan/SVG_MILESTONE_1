#include "Rectangle.h"



Rect_::Rect_(float x, float y, float width, float height, float fillOpacity, float strokeOpacity, RGB fillRGB, RGB strokeRGB, float strokeWidth, Transform transform, string fill, string stroke)
    : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke), x(x), y(y), width(width), height(height) {}

void Rect_::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    SolidBrush RectBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
    Pen RectBorderPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

    GraphicsState state = TransformSVG(graphics, transform);

    graphics.DrawRectangle(&RectBorderPen, x, y, width, height);

    if (fill != "")
    {
        LinearGradient* linearGradient = nullptr;

        vector<LinearGradient*> vectorlinear = defs[0]->getlinear();

        for (LinearGradient* lin : vectorlinear)
        {

            if (lin->getID() == fill)
            {
                linearGradient = lin;
                break;
            }
        }

        if (linearGradient != nullptr)
        {
            vector<Stop*> StopList = linearGradient->getStopList();
            Color* colors = new Color[StopList.size()];

            for (int i = 0; i < StopList.size(); ++i)
            {
                colors[i] = Color(255 * StopList[i]->getstopOpacity(), StopList[i]->getstopColor_red(), StopList[i]->getstopColor_green(), StopList[i]->getstopColor_blue());
            }
            float* positions = new float[StopList.size()];

            for (size_t i = 0; i < StopList.size(); ++i)
            {
                positions[i] = StopList[i]->getoffset();
            }

            pointLinearGradient pointlineargradient = linearGradient->getPoint();
           
            LinearGradientBrush gradientBrush(PointF(pointlineargradient.x1, pointlineargradient.y1), PointF(pointlineargradient.x2, pointlineargradient.y2), colors[0], colors[StopList.size() - 1]);
            
            gradientBrush.SetLinearColors(colors[0], colors[StopList.size() - 1]); //  default gradientUnits = userSpaceOnUse
            
            gradientBrush.SetInterpolationColors(colors, positions, StopList.size());

            gradientBrush.SetGammaCorrection(TRUE);
            graphics.FillRectangle(&gradientBrush, x, y, width, height);

            delete[] colors;
            delete[] positions;
        }
    }

    else
        graphics.FillRectangle(&RectBrush, x, y, width, height);
    graphics.Restore(state);
}