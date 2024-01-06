#ifndef VIEWBOX_H
#define VIEWBOX_H
#include "Shapes.h"
class ViewBox {
private:
    float x, y, width_in, height_in;
    float width_out, height_out;
    float scale;
    bool check_viewbox;
public:
    ViewBox(float, float, float, float, float, float, bool);
    float getScale();
    float getWidthOut();
    float getHeightOut();
    float getWidthIn();
    float getHeightIn();
    bool getcheck();
    void DrawViewBox();
};
#endif