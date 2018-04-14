#ifndef TOOL_H
#define TOOL_H

#include <QtWidgets>

#include "constants.h"


class DrawArea;

class Tool : public QPen
{
public:
    Tool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
         Qt::PenCapStyle c = Qt::RoundCap, QPixmap *i = 0,
         Qt::PenJoinStyle j = Qt::BevelJoin)
        : QPen(brush, width, s, c, j) { image = i; }
    virtual ~Tool() {}

    virtual ToolType getType() const = 0;
    virtual void drawTo(const QPoint&, DrawArea*) {}

    QPixmap* getImage() const { return image; }
    QPoint getStartPoint() const { return startPoint; }
    void setStartPoint(QPoint point) { startPoint = point; }

private:
    QPixmap* image;
    QPoint startPoint;

    /** Don't allow copying */
    Tool(const Tool&);
    Tool& operator=(const Tool&);
};

class PenTool : public Tool
{
public:
    PenTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
            Qt::PenCapStyle c = Qt::RoundCap, QPixmap *i = 0,
            Qt::PenJoinStyle j = Qt::BevelJoin)
       : Tool(brush, width, s, c, i, j) {}

    virtual ToolType getType() const { return pen; }
    virtual void drawTo(const QPoint&, DrawArea*);

private:
    /** Don't allow copying */
    PenTool(const PenTool&);
    PenTool& operator=(const PenTool&);
};

class LineTool : public Tool
{
public:
    LineTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
             Qt::PenCapStyle c = Qt::RoundCap, QPixmap *i = 0,
             Qt::PenJoinStyle j = Qt::BevelJoin)
       : Tool(brush, width, s, c, i, j) {}
    virtual ToolType getType() const { return line; }
    virtual void drawTo(const QPoint&, DrawArea*);

private:
    /** Don't allow copying */
    LineTool(const LineTool&);
    LineTool& operator=(const LineTool&);
};

class EraserTool : public PenTool
{
public:
    EraserTool(const QBrush &brush, qreal width,
               Qt::PenStyle s = Qt::SolidLine,
               Qt::PenCapStyle c = Qt::RoundCap, QPixmap *i = 0,
               Qt::PenJoinStyle j = Qt::BevelJoin)
       : PenTool(brush, width, s, c, i, j) {}

    virtual ToolType getType() const { return eraser; }

private:
    /** Don't allow copying */
    EraserTool(const EraserTool&);
    EraserTool& operator=(const EraserTool&);
};

class RectTool : public Tool
{
public:
    RectTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
             Qt::PenCapStyle c = Qt::RoundCap, QPixmap *i = 0,
             Qt::PenJoinStyle j = Qt::BevelJoin,
             QColor fill = QColor(Qt::transparent),
             ShapeType shape = rectangle,
             FillColor mode = no_fill,
             int roundedCurve = DEFAULT_RECT_CURVE);

    virtual ToolType getType() const { return rect_tool; }
    virtual void drawTo(const QPoint&, DrawArea*);

    FillColor getFillMode() const { return fillMode; }
    void setFillMode(FillColor mode) { fillMode = mode; }
    void setShapeType(ShapeType shape) { shapeType = shape; }
    void setFillColor(QColor color) { fillColor = color; }
    void setCurve(int value) { roundedCurve = value; }
    QRect adjustPoints(const QPoint&);

private:
    ShapeType shapeType;
    QColor fillColor;
    FillColor fillMode;
    int roundedCurve;

    /** Don't allow copying */
    RectTool(const RectTool&);
    RectTool& operator=(const RectTool&);
};

#endif // TOOL_H
