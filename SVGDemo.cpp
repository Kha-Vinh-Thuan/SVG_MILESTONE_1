﻿//SVG
#include "stdafx.h"
#include "rapidxml.hpp"
#include "pugixml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <Windows.h>
#include <Windowsx.h> 
#include <sstream>

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

struct RGB
{
    int r, g, b;
};

float zoomScale = 1.0f;

void ZoomGraphics(Graphics& graphics, float scale)
{
    graphics.SetTransform(new Matrix(scale, 0, 0, scale, 0, 0));
}
void convert_String_to_RGB(RGB& rgb, string s, smatch match, regex re)
{
    rgb = { 0, 0, 0 };
    if ((s[0] == 'r' && s[1] == 'g' && s[2] == 'b') && regex_search(s, match, re))
    {
        if (match.size() == 4)
        {
            rgb.r = toi(match[1]);
            if (rgb.r > 255)
                rgb.r = 255;
            rgb.g = stoi(match[2]);
            if (rgb.g > 255)
                rgb.g = 255;
            rgb.b = stoi(match[3]);
            if (rgb.b > 255)
                rgb.b = 255;
        }
    }
}

class Shape
{
protected:
    float fillOpacity, strokeOpacity, strokeWidth;
    RGB fillRGB, strokeRGB;
    Transform transform;
public:
    Shape(vector<string> transformOrder)
        : fillOpacity(1.0f), strokeOpacity(1.0f), strokeWidth(1.0f),
        fillRGB({ 255, 255, 255 }), strokeRGB({ 0, 0, 0 }),
        transform({ 0, 0, 0, 1.0f, 1.0f, transformOrder }) {}

    Shape(float strokeOpacity, RGB strokeRGB, float strokeWidth, Transform transform)
        : strokeOpacity(strokeOpacity), strokeRGB(strokeRGB), strokeWidth(strokeWidth), transform(transform) {}

    Shape(RGB fillRGB, RGB strokeRGB, float fillOpacity, float strokeOpacity, float strokeWidth, Transform transform)
        : fillRGB(fillRGB), strokeRGB(strokeRGB), fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), strokeWidth(strokeWidth), transform(transform) {}

    virtual void Draw(Graphics& graphics) = 0;

};

class Ellipse_ : public Shape
{
protected:
    float cx, cy;
    float rx, ry;
public:
    Ellipse_(float cx, float cy, float rx, float ry, float fillOpacity, float strokeOpacity, float strokeWidth, RGB fill, RGB stroke, Transform transform)
        : Shape(fill, stroke, fillOpacity, strokeOpacity, strokeWidth, transform), cx(cx), cy(cy), rx(rx), ry(ry) {}

    virtual void Draw(Graphics& graphics) override
    {
        GraphicsState state = TransformSVG(graphics, transform);
        Pen ellipsePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
        SolidBrush ellipseBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));

        graphics.FillEllipse(&ellipseBrush, cx - rx, cy - ry, 2 * rx, 2 * ry);
        graphics.DrawEllipse(&ellipsePen, cx - rx, cy - ry, 2 * rx, 2 * ry);
        graphics.Restore(state);
    }
};

class Circle : public Ellipse_
{
private:
    float r;

public:
    Circle(float cx, float cy, float r, float fillOpacity, float strokeOpacity, RGB fill, RGB stroke, float strokeWidth, Transform transform)
        : Ellipse_(cx, cy, r, r, fillOpacity, strokeOpacity, strokeWidth, fill, stroke, transform), r(r) {}

    virtual void Draw(Graphics& graphics) override
    {
        GraphicsState state = TransformSVG(graphics, transform);
        SolidBrush circleBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
        graphics.FillEllipse(&circleBrush, cx - r, cy - r, 2 * r, 2 * r);

        Pen circlePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
        graphics.DrawEllipse(&circlePen, cx - (r + strokeWidth / 2), cy - (r + strokeWidth / 2), 2 * (r + strokeWidth / 2), 2 * (r + strokeWidth / 2));
        graphics.Restore(state);
    }
};

class Rect_ : public Shape
{
private:
    float x, y, width, height;

public:

    Rect_(float x, float y, float width, float height, float fillOpacity, float strokeOpacity, RGB fillRGB, RGB strokeRGB, float strokeWidth, Transform transform)
        : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform), x(x), y(y), width(width), height(height) {}
    virtual void Draw(Graphics& graphics) override
    {
        GraphicsState state = TransformSVG(graphics, transform);
        SolidBrush RectBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
        graphics.FillRectangle(&RectBrush, x, y, width, height);

        Pen RectBorderPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
        graphics.DrawRectangle(&RectBorderPen, x, y, width, height);
        graphics.Restore(state);
    }
};

class Line : public Shape
{
private:
    float x1, y1, x2, y2;
public:
    Line(float x1, float y1, float x2, float y2, float strokeOpacity, RGB strokeRGB, float strokeWidth, Transform transform)
        : Shape(strokeOpacity, strokeRGB, strokeWidth, transform), x1(x1), y1(y1), x2(x2), y2(y2) {}

    virtual void Draw(Graphics& graphics) override
    {
        GraphicsState state = TransformSVG(graphics, transform);
        Pen linePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
        graphics.DrawLine(&linePen, x1, y1, x2, y2);
        graphics.Restore(state);
    }
};

class Text : public Shape
{
private:
    float dx, dy;
    float x, y, fontSize;
    string content, fontFamily, textAnchor, fontStyle;

public:
    Text(float x, float y, const string& content, float fontSize, float fillOpacity, float strokeOpacity, float strokeWidth, RGB fillRGB, RGB strokeRGB, Transform transform, const string& fontFamily, float dx, float dy, string textAnchor, string fontStyle)
        : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform), x(x), y(y), fontSize(fontSize), content(content), fontFamily(fontFamily), dx(dx), dy(dy), fontStyle(fontStyle), textAnchor(textAnchor) {}

    virtual void Draw(Graphics& graphics) override
    {
        content = normalizeTextContent(content);
        GraphicsState state = TransformSVG(graphics, transform);
        wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
        wstring wideFontFamily = converter.from_bytes(fontFamily);

        vector<const wchar_t*> fontList = { L"Arial", L"Tahoma", L"Calibri", L"Lucida Sans", L"Verdana", L"Georgia", L"Helvetica",L"Futura", L"" };

        bool isValid = false;
        HFONT hFont = NULL;

        for (const wchar_t* fontName : fontList)
        {
            hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName);

            isValid = IsStringValidForFont(wideFontFamily.c_str(), hFont);

            if (isValid) {
                break;
            }

            DeleteObject(hFont);
            hFont = NULL;
        }

        if (!isValid) {
            fontFamily = "Times New Roman";
            wideFontFamily = L"Times New Roman";
            if (hFont) {
                DeleteObject(hFont);
                hFont = NULL;
            }

            hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");
        }

        FontFamily dynamicFontFamily(wideFontFamily.c_str());

        PointF point;
        StringFormat stringFormat;
        if (textAnchor == "start") {
            point = PointF(x + dx, y + dy - fontSize);
            stringFormat.SetAlignment(StringAlignmentNear);
            stringFormat.SetLineAlignment(StringAlignmentNear);
        }
        else if (textAnchor == "middle") {
            point = PointF(x + dx - fontSize / 2.0f, y + dy - fontSize / 2.0f);
            stringFormat.SetAlignment(StringAlignmentCenter);
            stringFormat.SetLineAlignment(StringAlignmentCenter);
        }
        else if (textAnchor == "end") {
            point = PointF(x + dx - fontSize, y + dy - fontSize);
            stringFormat.SetAlignment(StringAlignmentFar);
            stringFormat.SetLineAlignment(StringAlignmentFar);
        }
        else {
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
            graphics.FillPath(&fillBrush, &path);

            Pen pen(Color(strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
            graphics.DrawPath(&pen, &path);
        }
        else {
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
};

class Polyline_ : public Shape
{
private:
    string points;
    vector<pair<float, float>> allPoints;
public:
    Polyline_(const string& points, float fillOpacity, float strokeOpacity, float strokeWidth, RGB fillRGB, RGB strokeRGB, Transform transform)
        : points(points), Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform)
    {
        parsePoints(points, allPoints);
    }

    virtual void Draw(Graphics& graphics) override
    {
        vector<PointF> pointsArray;
        for (const auto& point : allPoints) {
            pointsArray.push_back(PointF(static_cast<float>(point.first), static_cast<float>(point.second)));
        }

        PointF* pointsArrayPtr = pointsArray.data();
        int numPoints = static_cast<int>(pointsArray.size());

        Pen polylinePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);
        SolidBrush polylineBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
        GraphicsState state = TransformSVG(graphics, transform);

        if (numPoints > 0)
        {
            if (fillRGB.r != 256 && fillRGB.b != 256 && fillRGB.g != 256 && fillOpacity != 0)
                graphics.FillPolygon(&polylineBrush, pointsArrayPtr, numPoints);
            if (strokeRGB.r != 256 && strokeRGB.b != 256 && strokeRGB.g != 256 && strokeOpacity != 0)
                graphics.DrawLines(&polylinePen, pointsArrayPtr, numPoints);
        }
        graphics.Restore(state);
    }

};

class Polygon_ : public Shape
{
private:
    string points;
    vector<pair<float, float>> allPoints;
public:
    Polygon_(const string& points, float fillOpacity, float strokeOpacity, RGB fill, RGB stroke, float strokeWidth, Transform transform)
        : points(points), Shape(fill, stroke, fillOpacity, strokeOpacity, strokeWidth, transform)
    {
        parsePoints(points, allPoints);
    }

    virtual void Draw(Graphics& graphics) override
    {
        GraphicsState state = TransformSVG(graphics, transform);
        SolidBrush polygonBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
        Pen polygonPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

        vector<PointF> pointsArray;

        for (const auto& point : allPoints) {
            pointsArray.push_back(PointF(point.first, point.second));
        }

        PointF* pointsArrayPtr = pointsArray.data();
        int numPoints = static_cast<int>(pointsArray.size());

        if (numPoints > 0)
        {
            if (fillRGB.r != 256 && fillRGB.b != 256 && fillRGB.g != 256 && fillOpacity != 0)
                graphics.FillPolygon(&polygonBrush, pointsArrayPtr, numPoints, FillModeWinding);
            if (strokeRGB.r != 256 && strokeRGB.b != 256 && strokeRGB.g != 256 && strokeOpacity != 0)
                graphics.DrawPolygon(&polygonPen, pointsArrayPtr, numPoints);
        }
        graphics.Restore(state);
    }
};

void parseAndRenderSVG(const string& filePath, vector<SVGElement*>& elements)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if (result)
    {
        pugi::xml_node svgNode = doc.child("svg");

        for (pugi::xml_node elementNode : svgNode.children())
        {
            string elementName = elementNode.name();

            if (elementName == "circle")
            {
                float cx = elementNode.attribute("cx").as_float();
                float cy = elementNode.attribute("cy").as_float();
                float r = elementNode.attribute("r").as_float();
                float fillOpacity = elementNode.attribute("fill-opacity").as_float();

                float strokeOpacity = elementNode.attribute("stroke-opacity").as_float(); //add

                string fill = elementNode.attribute("fill").value();
                string stroke = elementNode.attribute("stroke").value();
                float strokeWidth = elementNode.attribute("stroke-width").as_float();

                Circle* circle = new Circle(cx, cy, r, fillOpacity, strokeOpacity, fill, stroke, strokeWidth);
                elements.push_back(circle);
            }

            else if (elementName == "rect")
            {

                float x = elementNode.attribute("x").as_float();
                float y = elementNode.attribute("y").as_float();
                float width = elementNode.attribute("width").as_float();
                float height = elementNode.attribute("height").as_float();

                //fill-opacity, fill, stroke, và stroke-width
                float fillOpacity = elementNode.attribute("fill-opacity").as_float();

                string fill = elementNode.attribute("fill").value();
                string stroke = elementNode.attribute("stroke").value();
                float strokeWidth = elementNode.attribute("stroke-width").as_float();

                Rect_* rect = new Rect_(x, y, width, height, fillOpacity, fill, stroke, strokeWidth);
                elements.push_back(rect);
            }

            else if (elementName == "line")
            {
                float x1 = elementNode.attribute("x1").as_float();
                float y1 = elementNode.attribute("y1").as_float();
                float x2 = elementNode.attribute("x2").as_float();
                float y2 = elementNode.attribute("y2").as_float();
                float fillOpacity = elementNode.attribute("stroke-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                string stroke = elementNode.attribute("stroke").value();
                float strokeWidth = elementNode.attribute("stroke-width").as_float();

                Line* line = new Line(x1, y1, x2, y2, fillOpacity, fill, stroke, strokeWidth);
                elements.push_back(line);
            }

            else if (elementName == "text")
            {
                float x = elementNode.attribute("x").as_float();
                float y = elementNode.attribute("y").as_float();
                string content = elementNode.child_value();
                float fontSize = elementNode.attribute("font-size").as_float();
                string fill = elementNode.attribute("fill").value();

                Text* text = new Text(x, y, content, fontSize, fill);
                elements.push_back(text);
            }

            else if (elementName == "polyline")
            {
                string points = elementNode.attribute("points").value();
                float fillOpacity = elementNode.attribute("fill-opacity").as_float();

                float strokeOpacity = elementNode.attribute("stroke-opacity").as_float(); //add

                string fill = elementNode.attribute("fill").value();
                string stroke = elementNode.attribute("stroke").value();
                float strokeWidth = elementNode.attribute("stroke-width").as_float();

                Polyline_* polyline = new Polyline_(points, fillOpacity, strokeOpacity, strokeWidth, fill, stroke);
                elements.push_back(polyline);
            }

            else if (elementName == "polygon")
            {
                string points = elementNode.attribute("points").value();
                float fillOpacity = elementNode.attribute("fill-opacity").as_float();

                float strokeOpacity = elementNode.attribute("stroke-opacity").as_float(); //add

                string fill = elementNode.attribute("fill").value();
                string stroke = elementNode.attribute("stroke").value();
                float strokeWidth = elementNode.attribute("stroke-width").as_float();

                Polygon_* polygon = new Polygon_(points, fillOpacity, strokeOpacity, fill, stroke, strokeWidth);
                elements.push_back(polygon);
            }

            else if (elementName == "ellipse")
            {
                float cx = elementNode.attribute("cx").as_float();
                float cy = elementNode.attribute("cy").as_float();
                float rx = elementNode.attribute("rx").as_float();
                float ry = elementNode.attribute("ry").as_float();
                float fillOpacity = elementNode.attribute("fill-opacity").as_float();

                float strokeOpacity = elementNode.attribute("stroke-opacity").as_float(); //add

                string fill = elementNode.attribute("fill").value();
                string stroke = elementNode.attribute("stroke").value();
                float strokeWidth = elementNode.attribute("stroke-width").as_float();

                Ellipse_* ellipse = new Ellipse_(cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth, fill, stroke);
                elements.push_back(ellipse);
            }
        }
    }
    else
    {
        cout << "Khong the doc file SVG." << endl;
    }
}

string GetClassName(SVGElement* element)
{
    if (dynamic_cast<Circle*>(element) != NULL)
    {
        return "Circle";
    }
    else if (dynamic_cast<Rect_*>(element) != NULL)
    {
        return "Rect";
    }
    else if (dynamic_cast<Line*>(element) != NULL)
    {
        return "Line";
    }
    else if (dynamic_cast<Text*>(element) != NULL)
    {
        return "Text";
    }
    else if (dynamic_cast<Polyline_*>(element) != NULL)
    {
        return "Polyline";
    }
    else if (dynamic_cast<Polygon_*>(element) != NULL)
    {
        return "Polygon";
    }
    else if (dynamic_cast<Ellipse_*>(element) != NULL)
    {
        return "Ellipse";
    }

}

void DrawText_(Graphics& graphics, float x, float y, float fontSize, string content, int Rf, int Gf, int Bf)
{
    SolidBrush textBrush(Color(Rf, Gf, Bf)); // Màu tím
    FontFamily fontFamily(L"Times New Roman");
    Font font(&fontFamily, 30, FontStyleRegular, UnitPixel);
    PointF point(x - fontSize, y - fontSize);
    wstring text(content.begin(), content.end());
    graphics.DrawString(text.c_str(), -1, &font, point, &textBrush);
}

void DrawPolyline(Graphics& graphics, float fillOpacity, float strokeOpacity, float strokeWidth, int Rf, int Gf, int Bf, int Rs, int Gs, int Bs, vector<pair<float, float>> points)
{
    Pen polylinePen(Color(strokeOpacity * 255, Rs, Gs, Bs), strokeWidth);
    SolidBrush polylineBrush(Color(fillOpacity * 255, Rf, Gf, Bf));
    int numPoints = points.size();
    PointF* pointArray = new PointF[numPoints];

    for (int i = 0; i < numPoints; ++i)
    {
        pointArray[i] = PointF(points[i].first, points[i].second);
    }

    graphics.FillPolygon(&polylineBrush, pointArray, static_cast<int>(numPoints));
    graphics.DrawLines(&polylinePen, pointArray, static_cast<int>(numPoints));

    delete[] pointArray;
}

VOID OnPaint(HDC hdc, float zoomScale)
{
    Graphics graphics(hdc);
    ZoomGraphics(graphics, zoomScale);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    vector<Shape*> elements;
    parseAndRenderSVG("sample.svg", elements);

    for (const auto& element : elements)
    {
        string className = GetClassName(element);
        if (className == "Circle") {
            Circle* circle = dynamic_cast<Circle*>(element);
            circle->Draw(graphics);
        }
        else if (className == "Rect") {
            Rect_* rect = dynamic_cast<Rect_*>(element);
            rect->Draw(graphics);
        }
        else if (className == "Line") {
            Line* line = dynamic_cast<Line*>(element);
            line->Draw(graphics);
        }
        else if (className == "Text") {
            Text* text = dynamic_cast<Text*>(element);
            text->Draw(graphics);
        }
        else if (className == "Polyline") {
            Polyline_* polyline = dynamic_cast<Polyline_*>(element);
            polyline->Draw(graphics);
        }
        else if (className == "Polygon") {
            Polygon_* polygon = dynamic_cast<Polygon_*>(element);
            polygon->Draw(graphics);
        }
        else if (className == "Ellipse") {
            Ellipse_* ellipse = dynamic_cast<Ellipse_*>(element);
            ellipse->Draw(graphics);
        }
        else if (className == "Path") {
            ClassPath* path = dynamic_cast<ClassPath*>(element);
            path->Draw(graphics);
        }
        else if (className == "Group")
        {
            Group* group = dynamic_cast<Group*>(element);
            group->Draw(graphics);
        }
    }
    graphics.ResetTransform();
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Read XML
    xml_document<> doc;
    xml_node<>* rootNode;
    // Read the xml file into a vector
    ifstream file("sample.svg");
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc 
    doc.parse<0>(&buffer[0]);

    rootNode = doc.first_node();
    xml_node<>* node = rootNode->first_node();

    while (node != NULL) {
        char* nodeName = node->name();
        xml_attribute<>* firstAttribute = node->first_attribute();
        char* attributeName = firstAttribute->name();
        char* attributeValue = firstAttribute->value();
        xml_attribute<>* secondAttribute = firstAttribute->next_attribute();
        // Set breakpoint here to view value
        // Ref: http://rapidxml.sourceforge.net/manual.html
        node = node->next_sibling();
    }



    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("SVG Demo"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;
    //static float zoomScale = 1.0f;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc, zoomScale);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_MOUSEWHEEL:
    {
        short delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (delta > 0)
            zoomScale += 0.1f;
        else
            zoomScale -= 0.1f;

        if (zoomScale < 0.1f)
            zoomScale = 0.1f;

        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc