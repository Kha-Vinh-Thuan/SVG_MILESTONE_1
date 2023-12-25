#include "Shapes.h"

Shape::Shape() {}

Shape::Shape(vector<string> transformOrder)
    : fillOpacity(1.0f), strokeOpacity(1.0f), strokeWidth(1.0f),
    fillRGB({ 255, 255, 255 }), strokeRGB({ 0, 0, 0 }),
    transform({ 0, 0, 0, 1.0f, 1.0f, transformOrder }) {}

Shape::Shape(float strokeOpacity, RGB strokeRGB, float strokeWidth, Transform transform, string stroke)
    : strokeOpacity(strokeOpacity), strokeRGB(strokeRGB), strokeWidth(strokeWidth), transform(transform), stroke(stroke) {}

Shape::Shape(RGB fillRGB, RGB strokeRGB, float fillOpacity, float strokeOpacity, float strokeWidth, Transform transform, string fill, string stroke)
    : fillRGB(fillRGB), strokeRGB(strokeRGB), fillOpacity(fillOpacity), strokeOpacity(strokeOpacity), strokeWidth(strokeWidth), transform(transform), fill(fill), stroke(stroke) {}