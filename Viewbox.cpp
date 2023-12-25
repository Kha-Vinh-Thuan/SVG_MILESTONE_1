#include"Viewbox.h"
ViewBox::ViewBox(float x, float y, float width_in, float height_in, float width_out, float height_out, bool check_viewbox) :
    x(x), y(y), width_in(width_in), height_in(height_in), width_out(width_out), height_out(height_out), check_viewbox(check_viewbox), scale(1) {}

float ViewBox::getScale() { return scale; }
float ViewBox::getWidthOut() { return width_out; }
float ViewBox::getHeightOut() { return height_out; }
float ViewBox::getWidthIn() { return width_in; }
float ViewBox::getHeightIn() { return height_in; }
bool ViewBox::getcheck() { return check_viewbox; }