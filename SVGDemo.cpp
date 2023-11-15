//SVG
#include "rapidxml.hpp"
#include "stdafx.h"
#include "pugixml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex> // tim RGB

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class SVGElement
{
public:
    virtual void render() = 0;
    //virtual ~SVGElement() {} 
};

class Circle : public SVGElement
{
private:
    float cx, cy, r;
    float fillOpacity, strokeOpacity, strokeWidth;
    string fill;
    string stroke;
    int Rf, Gf, Bf; // RGB fill
    int Rs, Gs, Bs; // RGB stroke

    void extractRGBFromFill()
    {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(fill, matches, rgbRegex))
        {
            if (matches.size() == 4)
            {
                Rf = stoi(matches[1]);
                Gf = stoi(matches[2]);
                Bf = stoi(matches[3]);
            }
        }
        else
        {
            fill = "default_color";
            Rf = Gf = Bf = 0;
        }
    }

    void extractRGBFromStroke()
    {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(stroke, matches, rgbRegex))
        {
            if (matches.size() == 4)
            {
                Rs = stoi(matches[1]);
                Gs = stoi(matches[2]);
                Bs = stoi(matches[3]);
            }
        }
        else
        {
            stroke = "default_color";
            Rs = Gs = Bs = 0;
        }
    }

public:
    Circle(float cx, float cy, float r, float fillOpacity, float strokeOpacity, const string& fill, const string& stroke, float strokeWidth) : cx(cx), cy(cy), r(r), fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), fill(fill), stroke(stroke), strokeWidth(strokeWidth) {
        extractRGBFromFill();
        extractRGBFromStroke();
    }

    void render() override {}
    void Data_Transmission_Circle(float& cx_, float& cy_, float& r_, float& fillOpacity_, float& strokeOpacity_, float& strokeWidth_, int& Rf_, int& Gf_, int& Bf_, int& Rs_, int& Gs_, int& Bs_)
    {
        cx_ = this->cx;
        cy_ = this->cy;
        r_ = this->r;

        fillOpacity_ = this->fillOpacity;
        strokeOpacity_ = this->strokeOpacity;
        strokeWidth_ = this->strokeWidth;

        Rf_ = this->Rf;
        Gf_ = this->Gf;
        Bf_ = this->Bf;

        Rs_ = this->Rs;
        Gs_ = this->Gs;
        Bs_ = this->Bs;
    }
};

class Rect_ : public SVGElement
{
private:
    float x, y, width, height;
    float fillOpacity, strokeWidth;
    string fill;
    string stroke;
    int Rf, Gf, Bf; // RGB fill
    int Rs, Gs, Bs; // RGB stroke

    void extractRGBFromFill() {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(fill, matches, rgbRegex)) {
            if (matches.size() == 4) {
                Rf = stoi(matches[1]);
                Gf = stoi(matches[2]);
                Bf = stoi(matches[3]);
            }
        }
        else {
            fill = "default_color";
            Rf = Gf = Bf = 0;
        }
    }

    void extractRGBFromStroke() {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(stroke, matches, rgbRegex)) {
            if (matches.size() == 4) {
                Rs = stoi(matches[1]);
                Gs = stoi(matches[2]);
                Bs = stoi(matches[3]);
            }
        }
        else {
            stroke = "default_color";
            Rs = Gs = Bs = 0;
        }
    }
public:
    Rect_(float x, float y, float width, float height, float fillOpacity, const string& fill, const string& stroke, float strokeWidth)
        : x(x), y(y), width(width), height(height), fillOpacity(fillOpacity), fill(fill), stroke(stroke), strokeWidth(strokeWidth) {
        extractRGBFromFill();
        extractRGBFromStroke();
    }
    void render() override {}
    void Data_Transmission_Rect(float& x_, float& y_, float& width_, float& height_, float& fillOpacity_, int& Rf_, int& Gf_, int& Bf_, int& Rs_, int& Gs_, int& Bs_, float& strokeWidth_)
    {
        x_ = this->x;
        y_ = this->y;
        width_ = this->width;
        height_ = this->height;
        fillOpacity_ = this->fillOpacity;
        Rf_ = this->Rf;
        Gf_ = this->Gf;
        Bf_ = this->Bf;
        Rs_ = this->Rs;
        Gs_ = this->Gs;
        Bs_ = this->Bs;
        strokeWidth_ = this->strokeWidth;
    }
};

class Line : public SVGElement {
private:
    float x1, y1, x2, y2;
    float strokeOpacity, strokeWidth;
    string stroke;
    int Rs, Gs, Bs; // RGB values for stroke
public:
    Line(float x1, float y1, float x2, float y2, float strokeOpacity, const string& fill, const string& stroke, float strokeWidth) : x1(x1), y1(y1), x2(x2), y2(y2), strokeOpacity(strokeOpacity), stroke(stroke), strokeWidth(strokeWidth)
    {
        extractRGBFromStroke();
    }

    void render() override {}
    void extractRGBFromStroke() {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(stroke, matches, rgbRegex)) {
            if (matches.size() == 4) {
                Rs = stoi(matches[1]);
                Gs = stoi(matches[2]);
                Bs = stoi(matches[3]);
            }
        }
        else {
            stroke = "default_color";
            Rs = Gs = Bs = 0;
        }
    }
    void Data_Transmission_Line(float& x1_, float& y1_, float& x2_, float& y2_, float& strokeOpacity_, float& strokeWidth_, int& Rs_, int& Gs_, int& Bs_)
    {
        x1_ = this->x1;
        y1_ = this->y1;
        x2_ = this->x2;
        y2_ = this->y2;
        strokeOpacity_ = this->strokeOpacity;
        strokeWidth_ = this->strokeWidth;
        Rs_ = this->Rs;
        Gs_ = this->Gs;
        Bs_ = this->Bs;
    }
};

class Text : public SVGElement
{
private:
    float x, y, fontSize;
    string content, fill;
    int Rf, Gf, Bf; // RGB values for fill

    void extractRGBFromFill() {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(fill, matches, rgbRegex))
        {
            if (matches.size() == 4)
            {
                Rf = stoi(matches[1]);
                Gf = stoi(matches[2]);
                Bf = stoi(matches[3]);
            }
        }
        else
        {
            fill = "default_color";
            Rf = Gf = Bf = 0;
        }
    }
public:
    Text(float x, float y, const string& content, float fontSize, const string& fill)
        : x(x), y(y), content(content), fontSize(fontSize), fill(fill)
    {
        extractRGBFromFill();
    }
    void render() override {}

    void Data_Transmission_Text(float& x_, float& y_, float& fontSize_, string& content_, int& Rf_, int& Gf_, int& Bf_)
    {
        x_ = this->x;
        y_ = this->y;
        fontSize_ = this->fontSize;
        content_ = this->content;
        Rf_ = this->Rf;
        Gf_ = this->Gf;
        Bf_ = this->Bf;
    }
};

class Polyline_ : public SVGElement
{
private:

    string points;
    float fillOpacity, strokeOpacity, strokeWidth;
    string fill;
    string stroke;
    int Rf, Gf, Bf; // RGB fill
    int Rs, Gs, Bs; // RGB stroke
    vector<pair<float, float>> allPoints;

    void extractRGBFromFill()
    {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(fill, matches, rgbRegex)) {
            if (matches.size() == 4) {
                Rf = stoi(matches[1]);
                Gf = stoi(matches[2]);
                Bf = stoi(matches[3]);
            }
        }
        else {
            fill = "default_color";
            Rf = Gf = Bf = 0;
        }
    }

    void extractRGBFromStroke()
    {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(stroke, matches, rgbRegex))
        {
            if (matches.size() == 4)
            {
                Rs = stoi(matches[1]);
                Gs = stoi(matches[2]);
                Bs = stoi(matches[3]);
            }
        }
        else
        {
            stroke = "default_color";
            Rs = Gs = Bs = 0;
        }
    }

    void parsePoints()
    {
        regex pointRegex(R"(\s*([\d.]+),\s*([\d.]+)\s*)");
        smatch matches;

        string pointsCopy = points;

        while (regex_search(pointsCopy, matches, pointRegex))
        {
            float x = stof(matches[1]);
            float y = stof(matches[2]);

            allPoints.push_back({ x, y });

            pointsCopy = matches.suffix();
        }
    }
public:
    Polyline_(const string& points, float fillOpacity, float strokeOpacity, float strokeWidth, const string& fill, const string& stroke)
        : points(points), fillOpacity(fillOpacity), fill(fill), strokeOpacity(strokeOpacity), strokeWidth(strokeWidth), stroke(stroke)
    {
        extractRGBFromFill();
        extractRGBFromStroke();
        parsePoints();
    }

    void render() override {}

    void Data_Transmission_Polyline(float& fillOpacity_, float& strokeOpacity_, float& strokeWidth_, int& Rf_, int& Gf_, int& Bf_, int& Rs_, int& Gs_, int& Bs_, vector<pair<float, float>>& points_)
    {
        fillOpacity_ = this->fillOpacity;
        strokeOpacity_ = this->strokeOpacity;
        strokeWidth_ = this->strokeWidth;
        Rf_ = this->Rf;
        Gf_ = this->Gf;
        Bf_ = this->Bf;
        Rs_ = this->Rs;
        Gs_ = this->Gs;
        Bs_ = this->Bs;
        points_ = this->allPoints;
    }

};

class Polygon_ : public SVGElement
{
private:
    string points;
    float fillOpacity, strokeOpacity, strokeWidth;
    string fill;
    string stroke;
    int Rf, Gf, Bf; // RGB values for fill
    int Rs, Gs, Bs; // RGB values for stroke
    vector<pair<float, float>> allPoints;

    void extractRGBFromFill()
    {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(fill, matches, rgbRegex))
        {
            if (matches.size() == 4)
            {
                Rf = stoi(matches[1]);
                Gf = stoi(matches[2]);
                Bf = stoi(matches[3]);
            }
        }
        else
        {
            fill = "default_color";
            Rf = Gf = Bf = 0;
        }
    }

    void extractRGBFromStroke()
    {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(stroke, matches, rgbRegex))
        {
            if (matches.size() == 4)
            {
                Rs = stoi(matches[1]);
                Gs = stoi(matches[2]);
                Bs = stoi(matches[3]);
            }
        }
        else
        {
            stroke = "default_color";
            Rs = Gs = Bs = 0;
        }
    }

    void parsePoints()
    {
        regex pointRegex(R"(\s*([\d.]+),\s*([\d.]+)\s*)");
        smatch matches;

        string pointsCopy = points; // Make a copy to avoid modifying the original string

        while (regex_search(pointsCopy, matches, pointRegex))
        {
            float x = stof(matches[1]);
            float y = stof(matches[2]);

            allPoints.push_back({ x, y });

            pointsCopy = matches.suffix();
        }
    }
public:
    Polygon_(const string& points, float fillOpacity, float strokeOpacity, const string& fill, const string& stroke, float strokeWidth)
        : points(points), fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), fill(fill), stroke(stroke), strokeWidth(strokeWidth)
    {
        extractRGBFromFill();
        extractRGBFromStroke();
        parsePoints();
    }

    void render() override {}

    void Data_Transmission_Polygon(float& fillOpacity_, float& strokeOpacity_, float& strokeWidth_, int& Rf_, int& Gf_, int& Bf_, int& Rs_, int& Gs_, int& Bs_, vector<pair<float, float>>& points_)
    {
        fillOpacity_ = this->fillOpacity;
        strokeOpacity_ = this->strokeOpacity;
        strokeWidth_ = this->strokeWidth;
        Rf_ = this->Rf;
        Gf_ = this->Gf;
        Bf_ = this->Bf;
        Rs_ = this->Rs;
        Gs_ = this->Gs;
        Bs_ = this->Bs;
        points_ = this->allPoints;
    }
};

class Ellipse_ : public SVGElement
{
private:
    float cx, cy, rx, ry;
    float fillOpacity, strokeOpacity, strokeWidth;
    string fill;
    string stroke;
    int Rf, Gf, Bf; // RGB fill
    int Rs, Gs, Bs; // RGB stroke

    void extractRGBFromFill() {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(fill, matches, rgbRegex)) {
            if (matches.size() == 4) {
                Rf = stoi(matches[1]);
                Gf = stoi(matches[2]);
                Bf = stoi(matches[3]);
            }
        }
        else {
            fill = "default_color";
            Rf = Gf = Bf = 0;
        }
    }


    void extractRGBFromStroke()
    {
        regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
        smatch matches;

        if (regex_search(stroke, matches, rgbRegex))
        {
            if (matches.size() == 4)
            {
                Rs = stoi(matches[1]);
                Gs = stoi(matches[2]);
                Bs = stoi(matches[3]);
            }
        }
        else
        {
            stroke = "default_color";
            Rs = Gs = Bs = 0;
        }
    }
public:
    Ellipse_(float cx, float cy, float rx, float ry, float fillOpacity, float strokeOpacity, float strokeWidth, string fill, string stroke) : cx(cx), cy(cy), rx(rx), ry(ry), fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), fill(fill), stroke(stroke), strokeWidth(strokeWidth)
    {
        extractRGBFromFill();
        extractRGBFromStroke();
    }

    void render() override {}

    void Data_Transmission_Ellipse(float& cx_, float& cy_, float& rx_, float& ry_, float& fillOpacity_, float& strokeOpacity_, float& strokeWidth_, int& Rf_, int& Gf_, int& Bf_, int& Rs_, int& Gs_, int& Bs_)
    {
        cx_ = this->cx;
        cy_ = this->cy;
        rx_ = this->rx;
        ry_ = this->ry;
        fillOpacity_ = this->fillOpacity;
        strokeOpacity_ = this->strokeOpacity;
        strokeWidth_ = this->strokeWidth;
        Rf_ = this->Rf;
        Gf_ = this->Gf;
        Bf_ = this->Bf;
        Rs_ = this->Rs;
        Gs_ = this->Gs;
        Bs_ = this->Bs;
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

void DrawRect(Graphics& graphics, float x, float y, float width, float height, float fillOpacity, int Rf, int Gf, int Bf, int Rs, int Gs, int Bs, int strokeWidth)
{
    SolidBrush RectBrush(Color(255 * fillOpacity, Rf, Gf, Bf));
    graphics.FillRectangle(&RectBrush, x, y, width, height);
    Pen RectBorderPen(Color(Rs, Gs, Bs), strokeWidth);
    graphics.DrawRectangle(&RectBorderPen, x, x, width, height);
}

void DrawCircle(Graphics& graphics, float cx, float cy, float r, float fillOpacity, float strokeOpacity, float strokeWidth, int Rf, int Gf, int Bf, int Rs, int Gs, int Bs)
{
    //circle
    SolidBrush circleBrush(Color(255 * fillOpacity, Rf, Gf, Bf)); // Màu vàng 
    graphics.FillEllipse(&circleBrush, cx - r, cy - r, 2 * r, 2 * r); // Tô màu hình tròn

    Pen circlePen(Color(255 * strokeOpacity, Rs, Gs, Bs), strokeWidth); //Màu xanh lam và độ dày của đường viền
    graphics.DrawEllipse(&circlePen, cx - (r + strokeWidth / 2), cy - (r + strokeWidth / 2), 2 * (r + strokeWidth / 2), 2 * (r + strokeWidth / 2)); // Vẽ đường viền hình tròn
}

void DrawLine(Graphics& graphics, float x1, float y1, float x2, float y2, float strokeOpacity, float strokeWidth, int Rs, int Gs, int Bs)
{
    Pen linePen(Color(255 * strokeOpacity, Rs, Gs, Bs), strokeWidth); // Màu xanh
    graphics.DrawLine(&linePen, x1, y1, x2, y2);
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

void DrawPolygon(Graphics& graphics, float fillOpacity, float strokeOpacity, float strokeWidth, int Rf, int Gf, int Bf, int Rs, int Gs, int Bs, const vector<pair<float, float>>& points)
{
    SolidBrush polygonBrush(Color(255 * fillOpacity, Rf, Gf, Bf));
    Pen polygonPen(Color(255 * strokeOpacity, Rs, Gs, Bs), strokeWidth);

    vector<PointF> pointsArray;
    for (const auto& point : points) {
        pointsArray.push_back(PointF(point.first, point.second));
    }
    PointF* pointsArrayPtr = pointsArray.data();

    graphics.FillPolygon(&polygonBrush, pointsArrayPtr, static_cast<int>(points.size()));
    graphics.DrawPolygon(&polygonPen, pointsArrayPtr, static_cast<int>(points.size()));
}

void DrawEllipse(Graphics& graphics, float cx, float cy, float rx, float ry, float fillOpacity, float strokeOpacity, float strokeWidth, int Rf, int Gf, int Bf, int Rs, int Gs, int Bs)
{
    Pen ellipsePen(Color(255 * strokeOpacity, Rs, Gs, Bs), strokeWidth);
    SolidBrush ellipseBrush(Color(255 * fillOpacity, Rf, Gf, Bf));

    graphics.FillEllipse(&ellipseBrush, cx - rx, cy - ry, 2 * rx, 2 * ry);
    graphics.DrawEllipse(&ellipsePen, cx - rx, cy - ry, 2 * rx, 2 * ry);
}

float zoomScale = 1.0f;

void ZoomGraphics(Graphics& graphics, float scale)
{
    graphics.SetTransform(new Matrix(scale, 0, 0, scale, 0, 0));
}

VOID OnPaint(HDC hdc, float zoomScale)
{

    // Ref: https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-getting-started-use
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

    ZoomGraphics(graphics, zoomScale);

    vector<SVGElement*> elements;
    parseAndRenderSVG("sample.svg", elements);

    for (const auto& element : elements)
    {
        string className = GetClassName(element);

        if (className == "Circle")
        {
            Circle* circle = dynamic_cast<Circle*>(element);
            float cx, cy, r, fillOpacity, strokeOpacity, strokeWidth;
            int Rf, Gf, Bf, Rs, Gs, Bs;
            circle->Data_Transmission_Circle(cx, cy, r, fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs);
            DrawCircle(graphics, cx, cy, r, fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs);
        }

        else if (className == "Rect")
        {
            Rect_* rect = dynamic_cast<Rect_*>(element);
            float x, y, width, height, fillOpacity, strokeWidth;
            int Rf, Gf, Bf, Rs, Gs, Bs;

            rect->Data_Transmission_Rect(x, y, width, height, fillOpacity, Rf, Gf, Bf, Rs, Gs, Bs, strokeWidth);
            DrawRect(graphics, x, y, width, height, fillOpacity, Rf, Gf, Bf, Rs, Gs, Bs, strokeWidth);
        }

        else if (className == "Line")
        {
            Line* line = dynamic_cast<Line*>(element);
            float x1, y1, x2, y2, strokeOpacity, strokeWidth;
            int Rs, Gs, Bs;

            line->Data_Transmission_Line(x1, y1, x2, y2, strokeOpacity, strokeWidth, Rs, Gs, Bs);
            DrawLine(graphics, x1, y1, x2, y2, strokeOpacity, strokeWidth, Rs, Gs, Bs);
        }

        else if (className == "Text")
        {
            Text* text = dynamic_cast<Text*>(element);
            float x, y, fontSize;
            string content;
            int Rf, Gf, Bf;
            text->Data_Transmission_Text(x, y, fontSize, content, Rf, Gf, Bf);
            DrawText_(graphics, x, y, fontSize, content, Rf, Gf, Bf);
        }

        else if (className == "Polyline")
        {
            Polyline_* polyline = dynamic_cast<Polyline_*>(element);
            float fillOpacity, strokeOpacity, strokeWidth;
            int Rf, Gf, Bf, Rs, Gs, Bs;
            vector<pair<float, float>> allPoints;
            polyline->Data_Transmission_Polyline(fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs, allPoints);
            DrawPolyline(graphics, fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs, allPoints);
        }

        else if (className == "Polygon")
        {
            Polygon_* polygon = dynamic_cast<Polygon_*>(element);
            float fillOpacity, strokeOpacity, strokeWidth;
            int Rf, Gf, Bf, Rs, Gs, Bs;
            vector<pair<float, float>> allPoints;
            polygon->Data_Transmission_Polygon(fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs, allPoints);
            DrawPolygon(graphics, fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs, allPoints);
        }

        else if (className == "Ellipse")
        {
            Ellipse_* ellipse = dynamic_cast<Ellipse_*>(element);
            float cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth;
            int Rf, Gf, Bf, Rs, Gs, Bs;
            ellipse->Data_Transmission_Ellipse(cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs);
            DrawEllipse(graphics, cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth, Rf, Gf, Bf, Rs, Gs, Bs);
        }
    }
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
   xml_node<> *rootNode;
   // Read the xml file into a vector
   ifstream file("sample.svg");
   vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
   buffer.push_back('\0');
   // Parse the buffer using the xml file parsing library into doc 
   doc.parse<0>(&buffer[0]);

   rootNode = doc.first_node();
   xml_node<> *node = rootNode->first_node();

   while (node != NULL) {
	   char *nodeName = node->name();
	   xml_attribute<> *firstAttribute = node->first_attribute();
	   char *attributeName = firstAttribute->name();
	   char *attributeValue = firstAttribute->value();
	   xml_attribute<> *secondAttribute = firstAttribute->next_attribute();
	   // Set breakpoint here to view value
	   // Ref: http://rapidxml.sourceforge.net/manual.html
	   node = node->next_sibling();
   }

   
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
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
   
   while(GetMessage(&msg, NULL, 0, 0))
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
   
   switch(message)
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