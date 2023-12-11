//SVG
#include "stdafx.h"
#include "rapidxml.hpp"
#include "pugixml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <Windows.h>
#include <Windowsx.h> 
#include <sstream>
#include <memory>
#include <locale>
#include <codecvt>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <GdiPlusEnums.h>
#include <GdiPlusFontFamily.h>
#include <GdiPlusHeaders.h>

using namespace rapidxml;
using namespace std;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

struct RGB
{
    int r, g, b;
};

struct Transform
{
    float translateX, translateY, rotateAngle;
    float scaleX, scaleY;
    vector<string> transformOrder;
};

struct groupChild
{
    float fontSize;
    float strokeOpacity, fillOpacity, strokeWidth;
    RGB strokeRGB, fillRGB;
    Transform transform;
};

struct Path
{
    vector<float> value;
    vector <char> type;
};

string chuanhoa(string input) {
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ',') {
            input[i] = ' ';
        }
    }
    istringstream iss(input);
    ostringstream oss;
    string word;
    bool first = true;

    while (iss >> word) {

        if (!first) {
            oss << " ";
        }
        oss << word;
        first = false;
    }
    return oss.str();
}

void getNextNumberOfValues(string& s, int& i, int num, Path& path, char curType) {
    int count = 0;
    for (int j = 0; j < num; ++j) {
        if (isdigit(s[i]) || s[i] == '-' || s[i] == '.' || s[i] == ' ') {
            size_t next_pos;
            double num = stod(s.substr(i), &next_pos);
            path.value.push_back(num);
            i += next_pos;
        }
    }
    path.type.push_back(curType);
}

void convertPathToValue(string s, Path& path) {

    char currentCommand = '\0';
    path.type.clear();
    path.value.clear();

    bool passM = false;
    s = chuanhoa(s);
    int i = 0;
    while (i < s.length()) {
        if (isalpha(s[i])) {

            currentCommand = s[i];
            if (currentCommand == 'M' || currentCommand == 'm' && !passM)
                passM = true;
            i++;

            switch (currentCommand) {
            case 'M':
            case 'm':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'L':
            case 'l':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'H':
            case 'h':
            case 'V':
            case 'v':
                getNextNumberOfValues(s, i, 1, path, currentCommand);
                break;
            case 'C':
            case 'c':
                getNextNumberOfValues(s, i, 6, path, currentCommand);
                break;
            case 'Z':
            case 'z':
                getNextNumberOfValues(s, i, 0, path, currentCommand);
                break;
            case 'S':
            case 's':
                getNextNumberOfValues(s, i, 4, path, currentCommand);
                break;
            }
        }
        else {
            if (currentCommand == 'M' || currentCommand == 'm' && passM)
                currentCommand = 'L';
            i++;
            switch (currentCommand) {
            case 'M':
            case 'm':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'L':
            case 'l':
                getNextNumberOfValues(s, i, 2, path, currentCommand);
                break;
            case 'H':
            case 'h':
            case 'V':
            case 'v':
                getNextNumberOfValues(s, i, 1, path, currentCommand);
                break;
            case 'C':
            case 'c':
                getNextNumberOfValues(s, i, 6, path, currentCommand);
                break;
            case 'Z':
            case 'z':
                getNextNumberOfValues(s, i, 0, path, currentCommand);
                break;
            case 'S':
            case 's':
                getNextNumberOfValues(s, i, 4, path, currentCommand);
                break;
            }
        }
        if (s[i] == ' ') i++;
        if (s[i] >= '0' && s[i] <= '9' || s[i] == '-') {
            i--;
        }
    }
}

void parsePoints(string s, vector<pair<float, float>>& allPoints) {

    int i = 0;
    s = chuanhoa(s);
    while (i < s.length()) {

        if (isdigit(s[i]) || s[i] == '-' || s[i] == '.' || s[i] == ' ') {

            size_t next_pos;
            float x = stof(s.substr(i), &next_pos);
            i += next_pos + 1;

            float y = stof(s.substr(i), &next_pos);
            i += next_pos;

            allPoints.push_back(make_pair(x, y));
        }
    }
}

string toLower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void convert_letters_to_RGB(RGB& rgb, string s)
{
    map<string, RGB> colorMap =
    {
        {"black", {0, 0, 0}},
        {"darkslategray", {47, 79, 79}},
        {"white", {255, 255, 255}},
        {"none", {255, 255, 255}},
        {"red", {255, 0, 0}},
        {"green", {0, 255, 0}},
        {"grey", {128, 128, 128}},
        {"blue", {0, 0, 255}},
        {"yellow", {255, 255, 0}},
        {"cyan", {0, 255, 255}},
        {"magenta", {255, 0, 255}},
        {"grey", {128, 128, 128}},
        {"lightgrey", {192, 192, 192}},
        {"darkgrey", {64, 64, 64}},
        {"maroon", {128, 0, 0}},
        {"olive", {128, 128, 0}},
        {"green", {138, 43, 226}},
        {"navy", {0, 0, 128}},
        {"purple", {128, 0, 128}},
        {"teal", {0, 128, 128}},
        {"orange", {255, 165, 0}},
        {"brown", {165, 42, 42}},
        {"pink", {255, 182, 193}},
        {"salmon", {250, 128, 114}},
        {"gold", {255, 215, 0}},
        {"lime", {0, 255, 0}},
        {"skyblue", {135, 206, 235}},
        {"forestgreen", {34, 139, 34}},
        {"turquoise", {64, 224, 208}},
        {"steelblue", {70, 130, 180}},
        {"dodgerblue", {30, 144, 255}},
        {"royalblue", {65, 105, 225}},
        {"indigo", {75, 0, 130}},
        {"slateblue", {106, 90, 205}},
        {"lavender", {230, 230, 250}},
        {"thistle", {216, 191, 216}},
        {"plum", {221, 160, 221}},
        {"violet", {238, 130, 238}},
        {"orchid", {218, 112, 214}},
        {"mediumpurple", {147, 112, 219}},
        {"darkorchid", {153, 50, 204}},
        {"darkviolet", {148, 0, 211}},
        {"darkmagenta", {139, 0, 139}},
        {"mediumorchid", {186, 85, 211}},
        {"palevioletred", {219, 112, 147}},
        {"cornflowerblue", {100, 149, 237}},
        {"deepskyblue", {0, 191, 255}},
        {"lightskyblue", {135, 206, 250}},
        {"midnightblue", {25, 25, 112}},
        {"mediumaquamarine", {102, 205, 170}},
        {"darkseagreen", {143, 188, 143}},
        {"lightgreen", {144, 238, 144}},
        {"palegreen", {152, 251, 152}},
        {"springgreen", {0, 255, 127}},
        {"mediumspringgreen", {0, 250, 154}},
        {"darkolivegreen", {85, 107, 47}},
        {"olivedrab", {107, 142, 35}},
        {"chartreuse", {127, 255, 0}},
        {"lawngreen", {124, 252, 0}},
        {"darkgreen", {0, 100, 0}},
        {"mediumseagreen", {60, 179, 113}},
        {"seagreen", {46, 139, 87}},
        {"darkslategray", {47, 79, 79}},
        {"dimgray", {105, 105, 105}},
        {"rosybrown", {188, 143, 143}},
        {"indianred", {205, 92, 92}},
        {"blueviolet", {138, 43, 226}},
        {"skyblue", {135, 206, 235}}

    };

    string lowerCaseS = toLower(s);

    auto it = colorMap.find(lowerCaseS);
    if (it != colorMap.end())
    {
        rgb = it->second;
    }
    else
    {
        rgb = { 255,255,255 };
    }
}

void convert_String_to_RGB(RGB& rgb, string s, smatch match, regex re)
{
    rgb = { 0, 0, 0 };
    if ((s[0] == 'r' && s[1] == 'g' && s[2] == 'b') && regex_search(s, match, re))
    {
        if (match.size() == 4)
        {
            rgb.r = stoi(match[1]);
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
    else if (s != "")
    {
        if (s[0] != '#')
            convert_letters_to_RGB(rgb, s);

        else if (s[0] == '#')
        {
            unsigned int hexValue;
            if (s.length() == 4) {
                char a1 = s[1], a2 = s[2], a3 = s[3];
                s = "#" + string(1, a1) + string(1, a1) + string(1, a2) + string(1, a2) + string(1, a3) + string(1, a3);
            }
            istringstream(s.substr(1)) >> hex >> hexValue;

            rgb.r = (hexValue >> 16) & 0xFF;
            rgb.g = (hexValue >> 8) & 0xFF;
            rgb.b = hexValue & 0xFF;
            int alpha = (hexValue >> 24) & 0xFF;
        }
    }
}

vector<string> split(string& s, char deli)
{
    vector<string> tokens;
    istringstream tokenStream(s);
    string token;
    while (getline(tokenStream, token, deli))
    {
        tokens.push_back(token);
    }
    return tokens;
}

bool containsKeyword(string& s, string keyword)
{
    return s.find(keyword) != string::npos;
}

string standardize(string transform) {
    string result;

    int pos = 0;
    while (pos < transform.length()) {
        int open = transform.find("(", pos);
        if (open == string::npos) break;

        int close = transform.find(")", open);
        if (close == string::npos) break;

        string content = transform.substr(open + 1, close - open - 1);

        if (content.find(",") == string::npos) {
            int space = content.find(" ");
            if (space != string::npos) {
                content.insert(space, ",");
            }
        }

        result += transform.substr(pos, open - pos + 1);
        result += content;
        result += ")";

        pos = close + 1;
    }

    result += transform.substr(pos);

    return result;
}

void parseTransform(const string& transformStr, Transform& transform)
{
    transform.transformOrder.clear();
    string transformString = transformStr;
    transformString = standardize(transformString);
    vector<string> transformDeli = split(transformString, ' ');
    for (string& deli : transformDeli) {
        if (containsKeyword(deli, "rotate")) {
            transform.transformOrder.push_back("rotate");
        }
        else if (containsKeyword(deli, "scale")) {
            transform.transformOrder.push_back("scale");
        }
        else if (containsKeyword(deli, "translate")) {
            transform.transformOrder.push_back("translate");
        }
    }

    // Xử lý translate
    smatch translateMatches;
    regex translateRegex("translate\\(([^,]+),([^)]+)\\)");
    if (regex_search(transformString, translateMatches, translateRegex)) {
        transform.translateX = stof(translateMatches[1]);
        transform.translateY = stof(translateMatches[2]);
    }
    // Xử lý rotate
    regex rotateRegex("rotate\\(([^)]+)\\)");
    smatch rotateMatches;
    if (regex_search(transformString, rotateMatches, rotateRegex)) {
        transform.rotateAngle = stof(rotateMatches[1]);
    }
    // Xử lý scale
    bool check = false;
    regex scaleCheck("scale\\((.*?)\\)");
    smatch scaleCheckMatches;
    regex scaleRegex("scale\\(([^,]+)(?:,([^)]+))?\\)");
    smatch scaleMatches;

    if (regex_search(transformString, scaleCheckMatches, scaleCheck)) {
        string scalePart = scaleCheckMatches[1];
        if (scalePart.find(",") != string::npos) {
            check = true;
        }
        if (scalePart.find(", ") != string::npos) {
            check = true;
        }

    }

    if (regex_search(transformString, scaleMatches, scaleRegex)) {

        float num1 = stof(scaleMatches[1].str());
        if (check) {
            float num2 = stof(scaleMatches[2].str());
            transform.scaleX = num1;
            transform.scaleY = num2;
        }
        else {
            transform.scaleX = transform.scaleY = num1;
        }
    }
}

vector<string> mergeVector(vector<string> v1, vector<string> v2) {
    vector<string> v3;

    for (string s : v1) {
        bool found = false;
        for (string x : v2) {
            if (s == x) {
                found = true;

                break;
            }
        }

        if (!found) {
            //vector3.insert(vector3.begin(), s);
            v3.push_back(s);
        }
    }

    for (string s : v2) {
        v3.push_back(s);
    }
    return v3;
}

void parseTransformChild(const string& transformStr, Transform& transform, groupChild& groupChild) {
    parseTransform(transformStr, transform);
    if (!transformStr.empty()) {
        transform.translateX += groupChild.transform.translateX;
        transform.translateY += groupChild.transform.translateY;
        transform.rotateAngle += groupChild.transform.rotateAngle;
        transform.rotateAngle = (transform.rotateAngle > 360) ? (transform.rotateAngle - 360) : transform.rotateAngle;
        transform.scaleX *= groupChild.transform.scaleX;
        transform.scaleY *= groupChild.transform.scaleY;
        transform.transformOrder = mergeVector(groupChild.transform.transformOrder, transform.transformOrder);
    }
    else {
        transform = groupChild.transform;
    }
}

float zoomScale = 1.0f;
float offsetX = 0.0f;
float offsetY = 0.0f;

void ZoomGraphics(Graphics& graphics, float scale)
{
    graphics.ResetTransform();
    graphics.TranslateTransform(offsetX, offsetY);
    graphics.ScaleTransform(scale, scale);
}

GraphicsState TransformSVG(Graphics& graphics, Transform transform)
{
    GraphicsState state = graphics.Save();
    Matrix transformMatrix;

    for (const string& operation : transform.transformOrder)
    {
        if (operation == "scale")
            transformMatrix.Scale(transform.scaleX, transform.scaleY);
        else if (operation == "translate")
            transformMatrix.Translate(transform.translateX, transform.translateY);
        else if (operation == "rotate")
            transformMatrix.Rotate(transform.rotateAngle);
    }
    graphics.MultiplyTransform(&transformMatrix);
    return state;
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
        SolidBrush circleBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
        Pen circlePen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

        GraphicsState state = TransformSVG(graphics, transform);
        graphics.FillEllipse(&circleBrush, cx - r, cy - r, 2 * r, 2 * r);
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
        SolidBrush RectBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
        Pen RectBorderPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

        GraphicsState state = TransformSVG(graphics, transform);
        graphics.FillRectangle(&RectBrush, x, y, width, height);
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
            if (fillRGB.r == 256 && fillRGB.b == 256 && fillRGB.g == 256)
                graphics.DrawLines(&polylinePen, pointsArrayPtr, numPoints);
            else
            {
                graphics.FillPolygon(&polylineBrush, pointsArrayPtr, numPoints);
                graphics.DrawLines(&polylinePen, pointsArrayPtr, numPoints);
            }
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
            graphics.FillPolygon(&polygonBrush, pointsArrayPtr, numPoints, FillModeWinding);
            graphics.DrawPolygon(&polygonPen, pointsArrayPtr, numPoints);
        }
        graphics.Restore(state);
    }
};

class ClassPath : public Shape
{
private:
    Path path;
public:

    ClassPath(float fillOpacity, float strokeOpacity, float strokeWidth, RGB fill, RGB stroke, Transform transform, Path path) :
        Shape(fill, stroke, fillOpacity, strokeOpacity, strokeWidth, transform), path(path) {}

    virtual void Draw(Graphics& graphics) override
    {
        GraphicsPath pathToDraw;

        size_t j = 0;
        PointF lastPoint;
        for (size_t i = 0; i < path.type.size(); ++i)
        {
            char pathType = path.type[i];
            switch (pathType)
            {
            case 'M':
            case 'm':
            {
                if (j + 1 < path.value.size())
                {
                    pathToDraw.StartFigure();
                    pathToDraw.AddLine(path.value[j], path.value[j + 1], path.value[j], path.value[j + 1]);
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
                    pathToDraw.AddLine(lastPoint.X, lastPoint.Y, lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1]);
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
            case 'C':
            {
                if (j + 5 < path.value.size())
                {
                    pathToDraw.GetLastPoint(&lastPoint);
                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y, path.value[j], path.value[j + 1], path.value[j + 2], path.value[j + 3], path.value[j + 4], path.value[j + 5]);
                    j += 6;
                }
                break;
            }
            case 'c':
                if (j + 5 < path.value.size())
                {
                    pathToDraw.GetLastPoint(&lastPoint);
                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y, lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1],
                        lastPoint.X + path.value[j + 2], lastPoint.Y + path.value[j + 3],
                        lastPoint.X + path.value[j + 4], lastPoint.Y + path.value[j + 5]);
                    lastPoint.X += path.value[j + 4];
                    lastPoint.Y += path.value[j + 5];
                    j += 6;
                }
                break;

            case 'Z':
            case'z':
            {
                pathToDraw.CloseFigure();

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
            case 'S':
            {
                if (j + 3 < path.value.size())
                {
                    pathToDraw.GetLastPoint(&lastPoint);

                    float control1X = 2 * lastPoint.X - path.value[j];
                    float control1Y = 2 * lastPoint.Y - path.value[j + 1];

                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y, control1X, control1Y, path.value[j], path.value[j + 1], path.value[j + 2], path.value[j + 3]);

                    j += 4;
                }
                break;
            }
            case 's':
            {
                if (j + 3 < path.value.size())
                {
                    pathToDraw.GetLastPoint(&lastPoint);

                    float control1X = 2 * lastPoint.X - (lastPoint.X + path.value[j]);
                    float control1Y = 2 * lastPoint.Y - (lastPoint.Y + path.value[j + 1]);

                    pathToDraw.AddBezier(lastPoint.X, lastPoint.Y, control1X, control1Y, lastPoint.X + path.value[j], lastPoint.Y + path.value[j + 1], lastPoint.X + path.value[j + 2], lastPoint.Y + path.value[j + 3]);

                    lastPoint.X += path.value[j + 2];
                    lastPoint.Y += path.value[j + 3];

                    j += 4;
                }
                break;
            }
            }
        }
        SolidBrush pathBrush(Color(255 * fillOpacity, fillRGB.r, fillRGB.g, fillRGB.b));
        Pen pathPen(Color(255 * strokeOpacity, strokeRGB.r, strokeRGB.g, strokeRGB.b), strokeWidth);

        GraphicsState state = TransformSVG(graphics, transform);
        graphics.FillPath(&pathBrush, &pathToDraw);
        graphics.DrawPath(&pathPen, &pathToDraw);
        graphics.Restore(state);
    }
};

class Group : public Shape
{
private:
    vector<Shape*> shapes;
    int fontSize;

public:

    Group(vector<Shape*>& shapes, float strokeOpacity, float fillOpacity, RGB stroke, RGB fill, float strokeWidth, Transform transform, int fontSize)
        : Shape(fillRGB, strokeRGB, fillOpacity, strokeOpacity, strokeWidth, transform), shapes(shapes), fontSize(fontSize) {}
    virtual void Draw(Graphics& graphics) override
    {
        for (auto& shape : shapes)
        {
            shape->Draw(graphics);
        }
    }
};

bool isParsingGroup = false;

vector<bool> checkRGB;
void parseSVGNode(pugi::xml_node& node, vector<Shape*>& elements, groupChild groupChild) {

    string nodeName = node.name();
    static regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");

    if (nodeName == "circle")
    {
        float cx = node.attribute("cx").as_float();
        float cy = node.attribute("cy").as_float();
        float r = node.attribute("r").as_float();
        RGB fillRGB, strokeRGB;
        smatch matches;

        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? groupChild.fillOpacity : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 255,255,255 };
        }
        else if (!fill.empty()) {
            convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            fillRGB = groupChild.fillRGB;
        }

        //STROKE
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255,255,255 };
        }
        else if (!stroke.empty())
        {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            strokeRGB = groupChild.strokeRGB;
        }

        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();

        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Circle* circle = new Circle(cx, cy, r, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform);
        elements.push_back(circle);
    }

    else if (nodeName == "rect") {

        float x = node.attribute("x").as_float();
        float y = node.attribute("y").as_float();
        float width = node.attribute("width").as_float();
        float height = node.attribute("height").as_float();
        RGB fillRGB, strokeRGB;
        smatch matches;

        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? groupChild.fillOpacity : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 255,255,255 };
        }
        else if (!fill.empty()) {
            convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            fillRGB = groupChild.fillRGB;
        }

        //STROKE
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255,255,255 };
        }
        else if (!stroke.empty())
        {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            strokeRGB = groupChild.strokeRGB;
        }

        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();


        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Rect_* rect = new Rect_(x, y, width, height, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform);
        elements.push_back(rect);
    }

    else if (nodeName == "line")
    {
        float x1 = node.attribute("x1").as_float();
        float y1 = node.attribute("y1").as_float();
        float x2 = node.attribute("x2").as_float();
        float y2 = node.attribute("y2").as_float();
        RGB fillRGB, strokeRGB;
        smatch matches;

        //Line khong co fill
       //STROKE
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255,255,255 };
        }
        else if (!stroke.empty())
        {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            strokeRGB = groupChild.strokeRGB;
        }

        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();

        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Line* line = new Line(x1, y1, x2, y2, strokeOpacity, strokeRGB, strokeWidth, transform);
        elements.push_back(line);
    }

    else if (nodeName == "text")
    {

        float x = node.attribute("x").as_float();
        float y = node.attribute("y").as_float();
        float dx = node.attribute("dx").as_float();
        float dy = node.attribute("dy").as_float();
        string textAnchor = node.attribute("text-anchor").empty()
            ? "start" : node.attribute("text-anchor").value();
        string fontStyle = node.attribute("font-style").empty()
            ? "normal" : node.attribute("font-style").value();

        // Content extraction
        string content;
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        {
            if (child.type() == pugi::node_pcdata)
            {
                content = child.value();
                break;
            }
        }

        float fontSize = node.attribute("font-size").as_float();
        if (fontSize == 0) {
            fontSize = groupChild.fontSize;
        }
        RGB fillRGB, strokeRGB;
        smatch matches;

        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? groupChild.fillOpacity : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 255,255,255 };
        }
        else if (!fill.empty()) {
            convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            fillRGB = groupChild.fillRGB;
        }

        //STROKE
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255,255,255 };
        }
        else if (!stroke.empty()) {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            strokeRGB = groupChild.strokeRGB;
        }

        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();

        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        //fontFamily
        string fontFamily = node.attribute("font-family").empty()
            ? "Times New Roman" : node.attribute("font-family").value();
        Text* text = new Text(x, y, content, fontSize, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fontFamily, dx, dy, textAnchor, fontStyle);
        elements.push_back(text);
    }

    else if (nodeName == "polyline")
    {
        string points = node.attribute("points").value();
        RGB fillRGB, strokeRGB;
        smatch matches;

        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? groupChild.fillOpacity : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 256,256,256 };
        }
        else if (!fill.empty()) {
            convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else
        {
            fillRGB = groupChild.fillRGB;
        }

        //STROKE
        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (!stroke.empty() && stroke != "none") {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else
        {
            strokeRGB = groupChild.strokeRGB;
            auto temp = strokeOpacity;
            auto temp_ = strokeWidth;
            for (const auto& checkk : checkRGB)
            {
                if (checkk)
                {
                    strokeWidth = temp_;
                    strokeOpacity = temp;
                    break;
                }
                strokeWidth = strokeOpacity = 0;
            }
        }


        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Polyline_* polyline = new Polyline_(points, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform);
        elements.push_back(polyline);
    }

    else if (nodeName == "polygon")
    {
        string points = node.attribute("points").value();

        RGB fillRGB, strokeRGB;
        smatch matches;

        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? groupChild.fillOpacity : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (!fill.empty()) {
            convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            fillRGB = groupChild.fillRGB;
        }

        //STROKE
        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (!stroke.empty()) {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else
        {
            strokeRGB = groupChild.strokeRGB;
            auto temp = strokeOpacity;
            auto temp_ = strokeWidth;
            for (const auto& checkk : checkRGB)
            {
                if (checkk)
                {
                    strokeWidth = temp_;
                    strokeOpacity = temp;
                    break;
                }
                strokeWidth = strokeOpacity = 0;
            }
        }

        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Polygon_* polygon = new Polygon_(points, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform);
        elements.push_back(polygon);
    }

    else if (nodeName == "ellipse")
    {
        float cx = node.attribute("cx").as_float();
        float cy = node.attribute("cy").as_float();
        float rx = node.attribute("rx").as_float();
        float ry = node.attribute("ry").as_float();
        RGB fillRGB, strokeRGB;
        smatch matches;

        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? groupChild.fillOpacity : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 255,255,255 };
        }
        else if (!fill.empty()) {
            convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            fillRGB = groupChild.fillRGB;
        }

        //STROKE
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255,255,255 };
        }
        else if (!stroke.empty()) {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            strokeRGB = groupChild.strokeRGB;
        }

        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();

        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        Ellipse_* ellipse = new Ellipse_(cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform);
        elements.push_back(ellipse);
    }

    else if (nodeName == "path") {
        RGB fillRGB, strokeRGB;
        smatch matches;
        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? groupChild.fillOpacity : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 255,255,255 };
        }
        else if (!fill.empty()) {
            convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        }
        else {
            fillRGB = groupChild.fillRGB;
        }

        //STROKE
        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? groupChild.strokeOpacity : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255,255,255 };
        }
        else if (!stroke.empty()) {
            convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        }
        else {
            strokeRGB = groupChild.strokeRGB;
            if (groupChild.strokeRGB.r == groupChild.strokeRGB.g == groupChild.strokeRGB.b == strokeRGB.r == strokeRGB.g == strokeRGB.b)
                strokeOpacity = 0;
        }
        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransformChild(transformValue, transform, groupChild);

        //Path
        Path path_;
        string pathValue = node.attribute("d").value();
        convertPathToValue(pathValue, path_);
        ClassPath* path = new ClassPath(fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, path_);
        elements.push_back(path);
    }

    else if (nodeName == "g") {
        if (isParsingGroup) {
            return;
        }
        isParsingGroup = true;
        vector<Shape*> groupElements;
        RGB fillRGB, strokeRGB;
        smatch matches;

        float fontSize = node.attribute("font-size").as_float();
        //FILL
        float fillOpacity = node.attribute("fill-opacity").empty()
            ? 1 : node.attribute("fill-opacity").as_float();
        string fill = node.attribute("fill").value();
        if (fill == "none")
        {
            fillOpacity = 0;
            fillRGB = { 255,255,255 };
        }
        convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);
        if (!fill.empty())
        {
            groupChild.fillRGB = fillRGB;
        }
        //STROKE
        float strokeOpacity = node.attribute("stroke-opacity").empty()
            ? 1 : node.attribute("stroke-opacity").as_float();
        string stroke = node.attribute("stroke").value();
        if (stroke == "none")
        {
            strokeOpacity = 0;
            strokeRGB = { 255,255,255 };
        }

        convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
        if (!stroke.empty())
        {
            groupChild.strokeRGB = strokeRGB;
        }
        float strokeWidth = node.attribute("stroke-width").empty()
            ? groupChild.strokeWidth : node.attribute("stroke-width").as_float();

        //TRANSFORM
        string transformValue = node.attribute("transform").value();
        Transform transform = { 0, 0, 0, 1.0, 1.0 };
        parseTransform(transformValue, transform);
        //groupChild.transform = transform;
        groupChild.transform.translateX += transform.translateX;
        groupChild.transform.translateY += transform.translateY;
        groupChild.transform.rotateAngle += transform.rotateAngle;
        groupChild.transform.scaleX *= transform.scaleX;
        groupChild.transform.scaleY = transform.scaleY;

        groupChild.fillOpacity = fillOpacity;
        groupChild.strokeOpacity = strokeOpacity;
        groupChild.strokeWidth = strokeWidth;

        isParsingGroup = false;
        for (pugi::xml_node& childNode : node.children())
        {
            parseSVGNode(childNode, groupElements, groupChild);
        }
        Group* group = new Group(groupElements, strokeOpacity, fillOpacity, strokeRGB, fillRGB, strokeWidth, transform, fontSize);
        elements.push_back(group);
    }

}

void parseAndRenderSVG(const string& filePath, vector<Shape*>& elements)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    static regex rgbRegex("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");

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
                RGB fillRGB, strokeRGB;
                smatch matches;

                //FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                //STROKE
                string stroke = elementNode.attribute("stroke").value();
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                if (stroke == "none")
                {
                    strokeOpacity = 0;
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Circle* circle = new Circle(cx, cy, r, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform);
                elements.push_back(circle);
            }

            else if (elementName == "rect") {

                float x = elementNode.attribute("x").as_float();
                float y = elementNode.attribute("y").as_float();
                float width = elementNode.attribute("width").as_float();
                float height = elementNode.attribute("height").as_float();
                RGB fillRGB, strokeRGB;
                smatch matches;

                //FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                //STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                if (stroke == "none")
                {
                    strokeOpacity = 0;
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Rect_* rect = new Rect_(x, y, width, height, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform);
                elements.push_back(rect);
            }

            else if (elementName == "line")
            {
                float x1 = elementNode.attribute("x1").as_float();
                float y1 = elementNode.attribute("y1").as_float();
                float x2 = elementNode.attribute("x2").as_float();
                float y2 = elementNode.attribute("y2").as_float();
                RGB fillRGB, strokeRGB;
                smatch matches;

                //Line khong co fill
                //STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                if (stroke == "none")
                {
                    strokeOpacity = 0;
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Line* line = new Line(x1, y1, x2, y2, strokeOpacity, strokeRGB, strokeWidth, transform);
                elements.push_back(line);
            }

            else if (elementName == "text")
            {

                float x = elementNode.attribute("x").as_float();
                float y = elementNode.attribute("y").as_float();
                float dx = elementNode.attribute("dx").as_float();
                float dy = elementNode.attribute("dy").as_float();
                string textAnchor = elementNode.attribute("text-anchor").empty()
                    ? "start" : elementNode.attribute("text-anchor").value();
                string fontStyle = elementNode.attribute("font-style").empty()
                    ? "normal" : elementNode.attribute("font-style").value();

                // Content extraction
                string content;
                for (pugi::xml_node child = elementNode.first_child(); child; child = child.next_sibling())
                {
                    if (child.type() == pugi::node_pcdata)
                    {
                        content = child.value();
                        break;
                    }
                }

                float fontSize = elementNode.attribute("font-size").as_float();
                RGB fillRGB, strokeRGB;
                smatch matches;

                // FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                // STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                if (stroke == "none")
                {
                    strokeOpacity = 0;
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);

                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                //fontFamily
                string fontFamily = elementNode.attribute("font-family").empty()
                    ? "Times New Roman" : elementNode.attribute("font-family").value();

                Text* text = new Text(x, y, content, fontSize, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, fontFamily, dx, dy, textAnchor, fontStyle);
                elements.push_back(text);
            }

            else if (elementName == "polyline")
            {
                string points = elementNode.attribute("points").value();
                RGB fillRGB, strokeRGB;
                smatch matches;

                //FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 256,256,256 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                //STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                if (stroke == "none")
                {
                    strokeOpacity = 0;
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Polyline_* polyline = new Polyline_(points, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform);
                elements.push_back(polyline);
            }

            else if (elementName == "polygon")
            {
                string points = elementNode.attribute("points").value();

                RGB fillRGB, strokeRGB;
                smatch matches;

                //FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                //STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                if (stroke == "none") {
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Polygon_* polygon = new Polygon_(points, fillOpacity, strokeOpacity, fillRGB, strokeRGB, strokeWidth, transform);
                elements.push_back(polygon);
            }

            else if (elementName == "ellipse")
            {
                float cx = elementNode.attribute("cx").as_float();
                float cy = elementNode.attribute("cy").as_float();
                float rx = elementNode.attribute("rx").as_float();
                float ry = elementNode.attribute("ry").as_float();
                RGB fillRGB, strokeRGB;
                smatch matches;

                //FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                //STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                if (stroke == "none")
                {
                    strokeOpacity = 0;
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                Ellipse_* ellipse = new Ellipse_(cx, cy, rx, ry, fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform);
                elements.push_back(ellipse);
            }

            else if (elementName == "path") {
                RGB fillRGB, strokeRGB;
                smatch matches;
                //FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                //STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                if (stroke == "none" || stroke == "")
                {
                    strokeOpacity = 0;
                    strokeRGB = { 255,255,255 };
                }
                else
                    convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();


                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                //Path
                Path path_;
                string pathValue = elementNode.attribute("d").value();
                convertPathToValue(pathValue, path_);
                ClassPath* path = new ClassPath(fillOpacity, strokeOpacity, strokeWidth, fillRGB, strokeRGB, transform, path_);
                elements.push_back(path);
            }

            else if (elementName == "g")
            {
                RGB fillRGB, strokeRGB;
                groupChild groupChild_;
                smatch matches;

                //FONT SIZE               
                float fontSize = elementNode.attribute("font-size").as_float();

                //FILL
                float fillOpacity = elementNode.attribute("fill-opacity").empty()
                    ? 1 : elementNode.attribute("fill-opacity").as_float();
                string fill = elementNode.attribute("fill").value();
                if (fill == "none")
                {
                    fillOpacity = 0;
                    fillRGB = { 256,256,256 };
                }
                else
                    convert_String_to_RGB(fillRGB, fill, matches, rgbRegex);

                //STROKE
                float strokeOpacity = elementNode.attribute("stroke-opacity").empty()
                    ? 1 : elementNode.attribute("stroke-opacity").as_float();
                string stroke = elementNode.attribute("stroke").value();
                convert_String_to_RGB(strokeRGB, stroke, matches, rgbRegex);
                if (stroke == "")
                {
                    bool checked = false;
                    checkRGB.push_back(checked);
                }
                else
                {
                    bool checked = true;
                    checkRGB.push_back(checked);
                }
                float strokeWidth = elementNode.attribute("stroke-width").empty()
                    ? 1 : elementNode.attribute("stroke-width").as_float();

                //TRANSFORM
                string transformValue = elementNode.attribute("transform").value();
                Transform transform = { 0, 0, 0, 1.0, 1.0 };
                parseTransform(transformValue, transform);

                //push group child elements
                groupChild_.fillOpacity = fillOpacity;
                groupChild_.strokeOpacity = strokeOpacity;
                groupChild_.fillRGB = fillRGB;
                groupChild_.strokeRGB = strokeRGB;
                groupChild_.strokeWidth = strokeWidth;
                groupChild_.transform = transform;
                groupChild_.fontSize = fontSize;

                vector<Shape*> shapes;
                int i = 0;
                // Duyệt qua các node con bên trong group
                for (pugi::xml_node& childNode : elementNode.children())
                {
                    parseSVGNode(childNode, shapes, groupChild_);
                    isParsingGroup = false;
                }
                Group* group = new Group(shapes, strokeOpacity, fillOpacity, strokeRGB, fillRGB, strokeWidth, transform, fontSize);
                elements.push_back(group);
            }
        }
    }
}

string GetClassName(Shape* element)
{
    if (dynamic_cast<Circle*>(element) != NULL) {
        return "Circle";
    }
    else if (dynamic_cast<Rect_*>(element) != NULL) {
        return "Rect";
    }
    else if (dynamic_cast<Line*>(element) != NULL) {
        return "Line";
    }
    else if (dynamic_cast<Text*>(element) != NULL) {
        return "Text";
    }
    else if (dynamic_cast<Polyline_*>(element) != NULL) {
        return "Polyline";
    }
    else if (dynamic_cast<Polygon_*>(element) != NULL) {
        return "Polygon";
    }
    else if (dynamic_cast<Ellipse_*>(element) != NULL) {
        return "Ellipse";
    }
    else if (dynamic_cast<ClassPath*>(element) != NULL) {
        return "Path";
    }
    if (dynamic_cast<Group*>(element) != NULL) {
        return "Group";
    }
}

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    ZoomGraphics(graphics, zoomScale);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    vector<Shape*> elements;
    parseAndRenderSVG("svg-08.svg", elements);

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
int lastMouseX = 0;
int lastMouseY = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
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
    case WM_MOUSEMOVE: {
        if (wParam & MK_LBUTTON) {
            int mouseX = GET_X_LPARAM(lParam);
            int mouseY = GET_Y_LPARAM(lParam);

            float deltaX = static_cast<float>(mouseX - lastMouseX);
            float deltaY = static_cast<float>(mouseY - lastMouseY);

            offsetX += deltaX;
            offsetY += deltaY;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        lastMouseX = GET_X_LPARAM(lParam);
        lastMouseY = GET_Y_LPARAM(lParam);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    xml_document<> doc;
    xml_node<>* rootNode;

    ifstream file("svg-16.svg");
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);

    rootNode = doc.first_node();
    xml_node<>* node = rootNode->first_node();

    while (node != NULL) {
        char* nodeName = node->name();
        xml_attribute<>* firstAttribute = node->first_attribute();
        char* attributeName = firstAttribute->name();
        char* attributeValue = firstAttribute->value();
        xml_attribute<>* secondAttribute = firstAttribute->next_attribute();
        node = node->next_sibling();
    }

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

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain