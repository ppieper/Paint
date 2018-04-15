#include <QPainter>

#include "tool.h"
#include "draw_area.h"


/**
 * @brief PenTool::drawTo - Draws line from startPoint to endPoint, where
 *                          startpoint is either:
 *
 *                          -where mouse was clicked (the initial left click)
 *                          -where mouse was moved FROM (last event's endPoint)
 *
 *                          -endPoint is where the mouse was moved TO on this event.
 *
 */
void PenTool::drawTo(const QPoint &endPoint, DrawArea *drawArea, QPixmap *image)
{
    QPainter painter(image);
    painter.setPen(static_cast<QPen>(*this));
    painter.drawLine(getStartPoint(), endPoint);

    // speed things up a bit by only updating the immediate
    // radius of the DrawArea
    int rad = (this->width() / 2) + 2;
    drawArea->update(QRect(getStartPoint(), endPoint).normalized()
                                .adjusted(-rad, -rad, +rad, +rad));
    setStartPoint(endPoint);
}

/**
 * @brief LineTool::drawTo - Draws line from startPoint to endPoint, where:
 *                           -startpoint is where mouse was clicked, and
 *                           -endPoint is where the mouse was released
 *
 */
void LineTool::drawTo(const QPoint &endPoint,  DrawArea *drawArea, QPixmap *image)
{
    QPainter painter(image);
    painter.setPen(static_cast<QPen>(*this));
    painter.drawLine(getStartPoint(), endPoint);
    drawArea->update();
}

/**
 * @brief RectTool::RectTool - Constructor for a rectangle tool.
 *
 */
RectTool::RectTool(const QBrush &brush, qreal width, Qt::PenStyle s,
                   Qt::PenCapStyle c, Qt::PenJoinStyle j, QColor fill,
                   ShapeType shape, FillColor mode, int curve)
    : Tool(brush, width, s, c, j)
{
    fillColor = fill;
    fillMode = mode;
    shapeType = shape;
    roundedCurve = curve;
}

/**
 * @brief RectTool::drawTo - Draws shape from startPoint to endPoint, where:
 *                           -startpoint is where mouse was clicked, and
 *                           -endPoint is where the mouse was released
 *
 */
void RectTool::drawTo(const QPoint &endPoint,  DrawArea *drawArea, QPixmap *image)
{
    QPainter painter(image);
    painter.setPen(static_cast<QPen>(*this));
    QRect rect = adjustPoints(endPoint);

    //draw a rectangle, square, or ellipse--fill or no fill--based on settings
    switch(shapeType)
    {
        case rectangle:
        {
            if(fillColor != no_fill)
                painter.fillRect(rect, fillColor);
            painter.drawRect(rect);
        } break;
        case rounded_rectangle:
        {
            if(fillMode != no_fill)
                painter.setBrush(QBrush(fillColor));
            painter.drawRoundedRect(rect, roundedCurve, roundedCurve,
                                          Qt::RelativeSize); break;
        }
        case ellipse:
        {
            if(fillMode != no_fill)
                painter.setBrush(QBrush(fillColor));
            painter.drawEllipse(rect);
        } break;
        default:
          break;
    }
    drawArea->update();
}

/**
 * @brief RectTool::adjustPoints - adjusts the points when constructing
 *                                 a rectangle
 *
 */
QRect RectTool::adjustPoints(const QPoint &endPoint)
{
    // 'top left' and 'bottom right' are relative, so we may need to
    // switch the points
    QRect rect;
    if(endPoint.x() < getStartPoint().x())
        rect = QRect(endPoint, getStartPoint());
    else
        rect = QRect(getStartPoint(), endPoint);
    return rect;
}
