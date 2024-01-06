#include "Text.h"


bool IsStringValidForFont(const wchar_t* str, HFONT hFont)
{
    HDC hdc = GetDC(NULL);
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);

    SIZE textSize;
    GetTextExtentPoint32(hdc, str, wcslen(str), &textSize);

    SelectObject(hdc, oldFont);
    ReleaseDC(NULL, hdc);

    return (textSize.cx <= tm.tmMaxCharWidth && textSize.cy <= tm.tmHeight);
}

string normalizeTextContent(string content)
{
    content.erase(remove(content.begin(), content.end(), '\n'), content.end());
    content = regex_replace(content, regex("\\s+"), " ");
    return content;
}

Text::Text(float x, float y, const string& content, float fontSize, float fillOpacity, float strokeOpacity, float strokeWidth, RGB fillRGB, RGB strokeRGB, Transform transform, const string& fontFamily, float dx, float dy, string textAnchor, string fontStyle, bool checkStroke, string fill, string stroke, Transform trans)
    : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform, fill, stroke), x(x), y(y), fontSize(fontSize), content(content), fontFamily(fontFamily), dx(dx), dy(dy), fontStyle(fontStyle), textAnchor(textAnchor), checkk(checkStroke), trans(trans) {}

void Text::Draw(Graphics& graphics, vector<Defs*>& defs)
{
    content = normalizeTextContent(content);
    GraphicsState state = TransformSVG(graphics, transform);
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    wstring wideFontFamily = converter.from_bytes(fontFamily);

    vector<const wchar_t*> fontList = { L"Arial", L"Tahoma", L"Calibri", L"Lucida Sans", L"Verdana", L"Georgia", L"Helvetica", L"Futura", L"" };

    bool isValid = false;
    HFONT hFont = NULL;

    for (const wchar_t* fontName : fontList)
    {
        hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName);

        isValid = IsStringValidForFont(wideFontFamily.c_str(), hFont);

        if (isValid)
        {
            break;
        }

        DeleteObject(hFont);
        hFont = NULL;
    }

    if (!isValid)
    {
        fontFamily = "Times New Roman";
        wideFontFamily = L"Times New Roman";
        if (hFont)
        {
            DeleteObject(hFont);
            hFont = NULL;
        }

        hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");
    }

    FontFamily dynamicFontFamily(wideFontFamily.c_str());

    PointF point;
    StringFormat stringFormat;
    if (textAnchor == "start")
    {
        point = PointF(x + dx, y + dy - fontSize);
        stringFormat.SetAlignment(StringAlignmentNear);
        stringFormat.SetLineAlignment(StringAlignmentNear);
    }
    else if (textAnchor == "middle")
    {
        point = PointF(x + dx - (fontSize / 2.0f) - trans.translateX / 2, y + dy - (fontSize / 2.0f) - trans.translateY / 2);
        stringFormat.SetAlignment(StringAlignmentCenter);
        stringFormat.SetLineAlignment(StringAlignmentCenter);
    }
    else if (textAnchor == "end")
    {
        point = PointF(x + dx, y + dy);
        stringFormat.SetAlignment(StringAlignmentFar);
        stringFormat.SetLineAlignment(StringAlignmentFar);
    }
    else
    {
        point = PointF(x + dx - fontSize, y + dy - fontSize);
        stringFormat.SetAlignment(StringAlignmentNear);
        stringFormat.SetLineAlignment(StringAlignmentNear);
    }
    wstring text(content.begin(), content.end());

    GraphicsPath path;
    if (fontStyle == "italic")
        path.AddString(text.c_str(), -1, &dynamicFontFamily, FontStyleItalic, fontSize, point, &stringFormat);
    else if (fontStyle == "bold")
        path.AddString(text.c_str(), -1, &dynamicFontFamily, FontStyleBold, fontSize, point, &stringFormat);
    else
        path.AddString(text.c_str(), -1, &dynamicFontFamily, FontStyleRegular, fontSize, point, &stringFormat);

    path.CloseFigure();

    if (strokeWidth != 0)
    {
        SolidBrush fillBrush(Color(fillRGB.r, fillRGB.g, fillRGB.b));
        Pen pen(Color(strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
        if (checkk == 0)
        {
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
                    graphics.FillPath(&gradientBrush, &path);

                    delete[] colors;
                    delete[] positions;
                }
            }

            else
            {
                graphics.FillPath(&fillBrush, &path);
                if(strokeRGB.r != 255 && strokeRGB.g != 255 && strokeRGB.b != 255)
                    graphics.DrawPath(&pen, &path);
            }
        }
        else
        {
            graphics.DrawPath(&pen, &path);
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
                    graphics.FillPath(&gradientBrush, &path);

                    delete[] colors;
                    delete[] positions;
                }
            }
            else 
            { 
                graphics.FillPath(&fillBrush, &path);                
                graphics.DrawPath(&pen, &path);
           }
        }
    }
    else
    {
        SolidBrush textBrush(Color(fillRGB.r, fillRGB.g, fillRGB.b));
        if (fontStyle == "italic")
        {
            Font font(&dynamicFontFamily, fontSize, FontStyleItalic, UnitPixel);
            graphics.DrawString(text.c_str(), -1, &font, point, &textBrush);
        }
        else if (fontStyle == "bold")
        {
            Font font(&dynamicFontFamily, fontSize, FontStyleBold, UnitPixel);
            graphics.DrawString(text.c_str(), -1, &font, point, &textBrush);
        }
        else
        {
            Font font(&dynamicFontFamily, fontSize, FontStyleRegular, UnitPixel);
            graphics.DrawString(text.c_str(), -1, &font, point, &textBrush);
        }
    }
    if (hFont)
    {
        DeleteObject(hFont);
        hFont = NULL;
    }
    graphics.Restore(state);
}