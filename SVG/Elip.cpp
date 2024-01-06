#include "Elip.h"

Ellipse_::Ellipse_(float cx, float cy, float rx, float ry, float fillOpacity, float strokeOpacity, float strokeWidth, RGB fillRGB, RGB strokeRGB, Transform transform, string fill, string stroke)
    : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke), cx(cx), cy(cy), rx(rx), ry(ry) {}

void Ellipse_::Draw(Graphics& graphics, vector<Defs*>& defs)
{

    Pen ellipsePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
    SolidBrush ellipseBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));

    GraphicsState state = TransformSVG(graphics, transform);
    graphics.DrawEllipse(&ellipsePen, cx - rx, cy - ry, 2 * rx, 2 * ry);
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
            graphics.FillEllipse(&gradientBrush, cx - rx, cy - ry, 2 * rx, 2 * ry);
            delete[] colors;
            delete[] positions;
        }
    }
    else
        graphics.FillEllipse(&ellipseBrush, cx - rx, cy - ry, 2 * rx, 2 * ry);

    graphics.Restore(state);
}

