#include "Circle.h"

Circle::Circle(float cx, float cy, float r, float fillOpacity, float strokeOpacity, RGB fillRGB, RGB strokeRGB, float strokeWidth, Transform transform, string fill, string stroke)
    : Ellipse_(cx, cy, r, r, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fill, stroke), r(r) {}

void Circle::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    SolidBrush circleBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
    Pen circlePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
    GraphicsState state = TransformSVG(graphics, transform);
    graphics.DrawEllipse(&circlePen, cx - (r + strokeWidth / 2), cy - (r + strokeWidth / 2), 2 * (r + strokeWidth / 2), 2 * (r + strokeWidth / 2));
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
            gradientBrush.SetInterpolationColors(colors, positions, StopList.size());
            gradientBrush.SetGammaCorrection(TRUE);
            graphics.FillEllipse(&gradientBrush, cx - r, cy - r, 2 * r, 2 * r);
            delete[] colors;
            delete[] positions;
        }
    }
    else
    {
        graphics.FillEllipse(&circleBrush, cx - r, cy - r, 2 * r, 2 * r);
    }
    graphics.Restore(state);
}