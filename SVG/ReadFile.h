#ifndef READFILE_H
#define READFILE_H
#define _USE_MATH_DEFINES
#include "Rectangle.h"
#include "Circle.h"
#include "Group.h"
#include "Line.h"
#include "Path.h"
#include "Polygon.h"
#include "Polyline.h"
#include "Text.h"
#include "Defs.h"
#include "Viewbox.h"
string chuanhoa(string);

void getNextNumberOfValues(string&, int&, int, Path&, char);

void convertPathToValue(string, Path&);

string toLower(string);

void convert_letters_to_RGB(RGB&, string);

string convert_String_to_RGB(RGB&, string, smatch, regex);


void convert_String_to_RGB_(RGB&, string, smatch, regex);
void convertStyle(string, string&, string&, RGB&, RGB&, float&, float&, float&);

void convertStyleChild(string, string&, string&, RGB&, RGB&, float&, float&, float&, groupChild);

void convertStyleGradient(string, string&, string&, string&);

vector<string> split(string&, char);

bool containsKeyword(string&, string);

string standardize(string);

void parseTransform(const string&, Transform&);

vector<string> mergeVector(vector<string>, vector<string>);

void parseTransformChild(const string&, Transform&, groupChild&);

void parseSVGNode(pugi::xml_node&, vector<Shape*>&, groupChild);

void parseAndRenderSVG(const string&, vector<Shape*>&, vector<Defs*>&, ViewBox*& viewBox);

#endif // !READFILE_H