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

                lastBezier.X = path.value[j + 2];
                lastBezier.Y = path.value[j + 3];
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
        case 's':
        case 'S':
        {
            if (j + 3 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);

                float control1X, control1Y;
                if (lastBezier.X == 0 && lastBezier.Y == 0)
                {
                    control1X = lastPoint.X;
                    control1Y = lastPoint.Y;
                }
                else
                {
                    control1X = 2.0f * lastPoint.X - lastBezier.X;
                    control1Y = 2.0f * lastPoint.Y - lastBezier.Y;
                }
                
                if (pathType == 's')
                {
                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y,
                        control1X, control1Y,
                        lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1],
                        lastPoint.X + path.value[j + 2], lastPoint.Y + path.value[j + 3]);

                    lastBezier.X = lastPoint.X + path.value[j];
                    lastBezier.Y = lastPoint.Y + path.value[j + 1];
                }
                else
                {
                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y, control1X, control1Y,
                        path.value[j], path.value[j + 1],
                        path.value[j + 2], path.value[j + 3]);

                    lastBezier.X = path.value[j];
                    lastBezier.Y = path.value[j + 1];
                }
                j += 4;
            }
            break;
        }

        case 'Q':
        {
            if (j + 3 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);

                pathToDraw.AddBezier(lastPoint.X, lastPoint.Y,
                    path.value[j], path.value[j + 1],
                    path.value[j + 2], path.value[j + 3],
                    path.value[j + 2], path.value[j + 3]);

                lastBezier.X = path.value[j];
                lastBezier.Y = path.value[j + 1];
                j += 4;
            }
            break;
        }
        case 'q':
        {
            if (j + 3 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);

                float point1 = lastPoint.X + 2.0f / 3.0f * (path.value[j]);
                float point2 = lastPoint.Y + 2.0f / 3.0f * (path.value[j + 1]);
                float point3 = (lastPoint.X + path.value[j + 2]) + 2.0f / 3.0f * (path.value[j] - path.value[j + 2]);
                float point4 = (lastPoint.Y + path.value[j + 3]) + 2.0f / 3.0f * (path.value[j + 1] - path.value[j + 3]);

                pathToDraw.AddBezier(lastPoint.X, lastPoint.Y,
                    point1, point2, point3, point4,
                    lastPoint.X + path.value[j + 2], lastPoint.Y + path.value[j + 3]);

                lastBezier.X = lastPoint.X + path.value[j + 2];
                lastBezier.Y = lastPoint.Y + path.value[j + 3];
                j += 4;
            }
            break;
        }

        case 't':
        case 'T':
        {
            if (j + 1 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);
                float control1X, control1Y;
                if (lastBezier.X == 0 && lastBezier.Y == 0)
                {
                    control1X = lastPoint.X;
                    control1Y = lastPoint.Y;
                }
                else
                {
                    control1X = 2 * lastPoint.X - lastBezier.X;
                    control1Y = 2 * lastPoint.Y - lastBezier.Y;
                }

                if (pathType == 't')
                {
                    float point1 = lastPoint.X + 2.0f / 3.0f * (control1X - lastPoint.X);
                    float point2 = lastPoint.Y + 2.0f / 3.0f * (control1Y - lastPoint.Y);

                    float point3 = (lastPoint.X + path.value[j]) + (2.0f / 3.0f * (control1X - lastPoint.X - path.value[j]));
                    float point4 = (lastPoint.Y + path.value[j + 1]) + (2.0f / 3.0f * (control1Y - lastPoint.Y - path.value[j + 1]));

                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y,
                        point1, point2,
                        point3, point4,
                        lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1]);

                    lastBezier.X = (3.0f / 4.0f * point1 + 1.0f / 4.0f * point3);
                    lastBezier.Y = (3.0f / 4.0f * point2 + 1.0f / 4.0f * point4);
                }
                else
                {
                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y,
                        control1X, control1Y,
                        path.value[j], path.value[j + 1],
                        path.value[j], path.value[j + 1]);

                    lastBezier.X = control1X;
                    lastBezier.Y = control1Y;

                }

                j += 2;
            }
            break;
        }

        case 'A':
        {
            if (j + 6 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);

                float rx = path.value[j];
                float ry = path.value[j + 1];
                float rotation = path.value[j + 2];
                bool largeArcFlag = path.value[j + 3] != 0;
                bool sweepFlag = path.value[j + 4] != 0;
                float x =  path.value[j + 5];
                float y =  path.value[j + 6];              

                float Angle = rotation * static_cast<float>(M_PI) / 180.0f;
                float cosAngle = cos(Angle);
                float sinAngle = sin(Angle);

                float cons = cosAngle * cosAngle - (sinAngle * -sinAngle);
                PointF point1;
                point1.X = cons * (lastPoint.X - x) / 2.0f;
                point1.Y = cons * (lastPoint.Y - y) / 2.0f;

                float check_r = (point1.X * point1.X) / (rx * rx) + (point1.Y * point1.Y) / (ry * ry);
                if (check_r > 1.0f)
                {
                    rx = sqrt(check_r) * rx;
                    ry = sqrt(check_r) * ry;
                }

                int sign;
                if (largeArcFlag == sweepFlag)
                    sign = -1;
                else
                    sign = 1;

                float num = abs(pow(rx, 2) * pow(ry, 2) - pow(rx, 2) * pow(point1.Y, 2) - pow(ry, 2) * pow(point1.X, 2));

                float den = pow(rx, 2) * pow(point1.Y, 2) + pow(ry, 2) * pow(point1.X, 2);
                PointF point2;
                point2.X = sign * sqrt(num / den) * (rx * point1.Y / ry);
                point2.Y = sign * sqrt(num / den) * (-ry * point1.X / rx);

                cons = pow(cosAngle, 2) - (sinAngle * -sinAngle);
                PointF center;
                center.X = cons * point2.X + (lastPoint.X + x) / 2.0f;
                center.Y = cons * point2.Y + (lastPoint.Y + y) / 2.0f;

                float startAngle = atan2((point1.Y - point2.Y) / ry, (point1.X - point2.X) / rx);
                float endAngle = atan2((-point1.Y - point2.Y) / ry, (-point1.X - point2.X) / rx);

                float deltAngle = endAngle - startAngle;

                if (sweepFlag && deltAngle < 0)
                {
                    deltAngle += (2.0f * M_PI);
                }
                else if (!sweepFlag && deltAngle > 0)
                {
                    deltAngle -= (2.0f * M_PI);
                }

                float startAngleDegrees = fmod((startAngle * 180.0f) / M_PI, 360.0f);
                float deltAngleDegrees = fmod((deltAngle * 180.0f) / M_PI, 360.0f);

                pathToDraw.AddArc(center.X - rx, center.Y - ry, 2.0f * rx, 2.0f * ry, 
                    startAngleDegrees, deltAngleDegrees);


                j += 7;
            }
            break;
        }
        case 'a':
        {
            if (j + 6 < path.value.size())
            {
                pathToDraw.GetLastPoint(&lastPoint);

                float rx = path.value[j];
                float ry = path.value[j + 1];
                float rotation = path.value[j + 2];
                bool largeArcFlag = path.value[j + 3];
                bool sweepFlag = path.value[j + 4];
                float x = lastPoint.X + path.value[j + 5];
                float y = lastPoint.Y + path.value[j + 6];

                float Angle = rotation * static_cast<float>(M_PI) / 180.0f;
                float cosAngle = cos(Angle);
                float sinAngle = sin(Angle);

                float cons = cosAngle * cosAngle - (sinAngle * -sinAngle);
                PointF point1;
                point1.X = cons * (lastPoint.X - x) / 2.0f;
                point1.Y = cons * (lastPoint.Y - y) / 2.0f;

                float check_r = (point1.X * point1.X) / (rx * rx) + (point1.Y * point1.Y) / (ry * ry);
                if (check_r > 1.0f)
                {
                    rx = sqrt(check_r) * rx;
                    ry = sqrt(check_r) * ry;
                }
                
                int sign = (largeArcFlag == sweepFlag ? -1 : 1);

                float num = abs(pow(rx, 2) * pow(ry, 2) - pow(rx, 2) * pow(point1.Y, 2) - pow(ry, 2) * pow(point1.X, 2));

                float den = pow(rx, 2) * pow(point1.Y, 2) + pow(ry, 2) * pow(point1.X, 2);
                PointF point2;
                point2.X = sign * sqrt(num / den) * (rx * point1.Y / ry);
                point2.Y = sign * sqrt(num / den) * (-ry * point1.X / rx);

                cons = pow(cosAngle, 2) - (sinAngle * -sinAngle);
                PointF center;
                center.X = cons * point2.X + (lastPoint.X + x) / 2.0f;
                center.Y = cons * point2.Y + (lastPoint.Y + y) / 2.0f;

                float startAngle = atan2((point1.Y - point2.Y) / ry, (point1.X - point2.X) / rx);
                float endAngle = atan2((-point1.Y - point2.Y) / ry, (-point1.X - point2.X) / rx);

                float deltAngle = endAngle - startAngle;

                if (sweepFlag && deltAngle < 0)
                {
                    deltAngle += 2.0f * M_PI;
                }
                else if(! sweepFlag && deltAngle > 0)
                {
                    deltAngle -= 2.0f * M_PI;
                }

                float startAngleDegrees = fmod((startAngle * 180.0f) / M_PI, 360.0f);
                float deltAngleDegrees = fmod((deltAngle * 180.0f) / M_PI, 360.0f);
                
                pathToDraw.AddArc(center.X - rx, center.Y - ry, 2.0f * rx, 2.0f * ry, 
                    startAngleDegrees,  deltAngleDegrees);
                
                j += 7;
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