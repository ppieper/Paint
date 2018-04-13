#ifndef CONSTANTS_H
#define CONSTANTS_H


const int DEFAULT_IMG_WIDTH = 640;
const int DEFAULT_IMG_HEIGHT = 480;
const int DEFAULT_PEN_THICKNESS = 1;
const int DEFAULT_ERASER_THICKNESS = 10;
const int DEFAULT_RECT_CURVE = 10;

const int MIN_PEN_SIZE = 1;
const int MAX_PEN_SIZE = 50;
const int MIN_RECT_CURVE = 0;
const int MAX_RECT_CURVE = 100;

enum ToolType {pen, line, eraser, rect_tool};
enum LineStyle {solid, dashed, dotted, dash_dotted, dash_dot_dotted};
enum CapStyle {flat, square, round_cap};
enum DrawType {single, poly};
enum ShapeType {rectangle, rounded_rectangle, ellipse};
enum FillColor {foreground, background, no_fill};
enum BoundaryType {miter_join, bevel_join, round_join};

#endif // CONSTANTS_H
