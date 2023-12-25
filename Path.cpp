#include "Path.h"


ClassPath::ClassPath(float fillOpacity, float strokeOpacity, float strokeWidth, RGB fillRGB, RGB strokeRGB, Transform transform, Path path, string fill, string stroke) :
    Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke), path(path) {}

void ClassPath::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    GraphicsPath pathToDraw;

    size_t j = 0;
    PointF lastPoint;
    PointF lastBezier = { 0,0 };
    PointF lastARC = { 0,0 };
    for (size_t i = 0; i < path.type.size(); ++i)
    {
        char pathType = path.type[i];
        switch (pathType)
        {
        case 'M':
        {
            if (j + 1 < path.value.size())
            {
                pathToDraw.StartFigure();
                pathToDraw.AddLine(path.value[j], path.value[j + 1], path.value[j], path.value[j + 1]);

              
                 
                j += 2;
            }
            break;
        }
        case 'm':
        {
            if (j + 1 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddLine(lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1], lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1]);
                j += 2;
            }
            break;
        }

        case 'L':
        {
            if (j + 1 < path.value.size())
            {
                pathToDraw.AddLine(path.value[j], path.value[j + 1], path.value[j], path.value[j + 1]);
                
                j += 2;
            }
            break;
        }
        case 'l':
        {
            if (j + 1 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddLine(lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1], lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1]);
                j += 2;
            }
            break;
        }

        case 'H':
        {
            if (j < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddLine(path.value[j], lastPoint.Y, path.value[j], lastPoint.Y);
                j += 1;
            }
            break;
        }
        case 'h':
        {
            if (j < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddLine(lastPoint.X, lastPoint.Y, lastPoint.X + path.value[j], lastPoint.Y);
                j += 1;
            }
            break;
        }

        case 'V':
        {
            if (j < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddLine(lastPoint.X, path.value[j], lastPoint.X, path.value[j]);
                j += 1;
            }
            break;
        }
        case 'v':
        {
            if (j < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddLine(lastPoint.X, lastPoint.Y, lastPoint.X, lastPoint.Y + path.value[j]);
                j += 1;
            }
            break;
        }

        case 'C':
        {
            if (j + 5 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddBezier(lastPoint.X, lastPoint.Y, path.value[j], path.value[j + 1],
                    path.value[j + 2], path.value[j + 3], path.value[j + 4], path.value[j + 5]);
                lastBezier.X = path.value[j + 4];
                lastBezier.Y = path.value[j + 5];
                j += 6;
            }
            break;
        }
        case 'c':
        {
            if (j + 5 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                pathToDraw.AddBezier(lastPoint.X, lastPoint.Y,
                    lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1],
                    lastPoint.X + path.value[j + 2], lastPoint.Y + path.value[j + 3],
                    lastPoint.X + path.value[j + 4], lastPoint.Y + path.value[j + 5]);

                lastBezier.X = lastPoint.X + path.value[j + 2];
                lastBezier.Y = lastPoint.Y + path.value[j + 3];

                j += 6;
            }
            break;
        }
        case 'Z':
        case 'z':
        {
            pathToDraw.CloseFigure();

            break;
        }
        }
    }
    SolidBrush pathBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
    Pen pathPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

    pathToDraw.SetFillMode(FillModeWinding);
    GraphicsState state = TransformSVG(graphics, transform);
    graphics.FillPath(&pathBrush, &pathToDraw);
    graphics.DrawPath(&pathPen, &pathToDraw);
    graphics.Restore(state);
}