#ifndef TEXT_H
#define TEXT_H
#include "Shapes.h"

class Text : public Shape
{
private:
    float dx, dy;
    float x, y, fontSize;
    string content, fontFamily, textAnchor, fontStyle;
    bool checkk;
    Transform trans;

public:
    Text(float, float, const string&, float, float, float, float, RGB, RGB, Transform, const string&, float, float, string, string, bool, string, string, Transform);
    void Draw(Graphics&, vector<Defs*>&) override;
};

bool IsStringValidForFont(const wchar_t*, HFONT);
string normalizeTextContent(string);
#endif // !TEXT_H