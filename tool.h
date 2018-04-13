#ifndef TOOL_H
#define TOOL_H

#endif // TOOL_H

#include <QtWidgets>

#include "constants.h"


class DrawArea;

class Tool : public QPen
{
public:
    Tool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
         QPixmap *image = 0, Qt::PenCapStyle c = Qt::RoundCap,
         Qt::PenJoinStyle j = Qt::BevelJoin)
        : QPen(brush, width, s, c, j) { this->image = image; }
    virtual ~Tool() {}

    virtual ToolType getType() const = 0;
    virtual void drawTo(const QPoint&, DrawArea*) {}

    QPixmap* getImage() const { return image; }
    QPoint getStartPoint() const { return startPoint; }
    void setStartPoint(QPoint point) { startPoint = point; }

private:
    QPixmap* image;
    QPoint startPoint;
};

class PenTool : public Tool
{
public:
    PenTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
            QPixmap *image = 0, Qt::PenCapStyle c = Qt::RoundCap,
            Qt::PenJoinStyle j = Qt::BevelJoin);

    virtual ToolType getType() const { return pen; }
    virtual void drawTo(const QPoint&, DrawArea*);

private:
};

class LineTool : public Tool
{
public:
    LineTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
             QPixmap *image = 0, Qt::PenCapStyle c = Qt::RoundCap,
             Qt::PenJoinStyle j = Qt::BevelJoin,
             DrawType shape = single);

    virtual ToolType getType() const { return line; }
    virtual void drawTo(const QPoint&, DrawArea*);

    void setDrawType(DrawType mode) { drawType = mode; }

private:
    DrawType drawType;
};

class EraserTool : public Tool
{
public:
    EraserTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
               QPixmap *image = 0, Qt::PenCapStyle c = Qt::RoundCap,
               Qt::PenJoinStyle j = Qt::BevelJoin);

    virtual ToolType getType() const { return eraser; }
    virtual void drawTo(const QPoint&, DrawArea*);

private:
};

class RectTool : public Tool
{
public:
    RectTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
             QPixmap *image = 0,Qt::PenCapStyle c = Qt::RoundCap,
             Qt::PenJoinStyle j = Qt::BevelJoin,
             QColor fill = QColor(Qt::transparent),
             ShapeType shape = rectangle,
             FillColor mode = no_fill,
             int roundedCurve = DEFAULT_RECT_CURVE);

    virtual ToolType getType() const { return rect_tool; }

    FillColor getFillMode() const { return fillMode; }
    void setFillMode(FillColor mode) { fillMode = mode; }
    void setShapeType(ShapeType shape) { shapeType = shape; }
    void setFillColor(QColor color) { fillColor = color; }
    void setCurve(int value) { roundedCurve = value; }

    virtual void drawTo(const QPoint&, DrawArea*);

private:
    ShapeType shapeType;
    QColor fillColor;
    FillColor fillMode;
    int roundedCurve;
};
