#include <QtWidgets>

#include "draw_area.h"
#include "main_window.h"


/**
 * @brief DrawArea::DrawArea - constructor for our Draw Area.
 *                             Pointers to the MainWindow's
 *                             tools and image are mandatory.
 *
 */
DrawArea::DrawArea(QWidget *parent, QPixmap *image,
                     QPen *penTool, QPen *lineTool,
                     QPen *eraserTool, QPen *rectTool)
    : QWidget(parent)
{
    // initialize image and tools
    this->image = image;
    this->penTool = penTool;
    this->lineTool = lineTool;
    this->eraserTool = eraserTool;
    this->rectTool = rectTool;

    mainWindow = (MainWindow*)this->parent();

    drawing = false;
    drawingPoly = false;
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_StaticContents);
}

/**
 * @brief DrawArea::paintEvent - redraw part of the image based
 *                               on what was modified.
 *
 */
void DrawArea::paintEvent(QPaintEvent *e)

{
    QPainter painter(this);
    QRect modifiedArea = e->rect(); // only need to redraw a small area
    painter.drawPixmap(modifiedArea, *image, modifiedArea);
}

/**
 * @brief DrawArea::mousePressEvent - left-click initiates a draw
 *
 *                                  - right-click opens a dialog
 *                                    menu for the current tool
 *
 */
void DrawArea::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton)
    {
        // open the dialog menu
        mainWindow->mousePressEvent(e);
    }
    else if (e->button() == Qt::LeftButton)
    {
        if(image->isNull())
            return;

        drawing = true;

        if(!drawingPoly)
            startPoint = e->pos();

        // save a copy of the old image
        oldImage = image->copy(QRect());
    }
}

/**
 * @brief DrawArea::mouseMoveEvent
 *
 */
void DrawArea::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && drawing)
    {
        if(image->isNull())
            return;

        if(currentTool == line)
        {
            *image = oldImage;
            if(currentLineMode == poly)
            {
                drawingPoly = true;
            }
        }
        drawTo(e->pos());
    }
}

/**
 * @brief DrawArea::mouseReleaseEvent
 *
 */
void DrawArea::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && drawing)
    {
        drawing = false;

        if(image->isNull())
            return;

        if(drawingPoly)
        {
            startPoint = e->pos();
            //return;
        }
        if(currentTool == pen)
            drawTo(e->pos());

        // for undo/redo
        mainWindow->saveDrawCommand(oldImage);
    }
}

/**
 * @brief DrawArea::mouseDoubleClickEvent
 *
 */
void DrawArea::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if(drawingPoly)
            drawingPoly = false;
    }
}

/**
 * @brief DrawArea::drawTo - Draws from startPoint to endPoint. How these values are
 *                           determined depends on the drawing tool/mode.
 *
 */
void DrawArea::drawTo(const QPoint &endPoint)
{
    switch(currentTool)
    {
        case pen:
        {
            QPainter painter(image);
            painter.setPen(*penTool);
            painter.drawLine(startPoint, endPoint);

            int rad = (penTool->width() / 2) + 2;
            update(QRect(startPoint, endPoint).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad));
            startPoint = endPoint;
        } break;
        case line:
        {
            QPainter painter(image);
            painter.setPen(*lineTool);
            painter.drawLine(startPoint, endPoint);

            //update(getLineRect(startPoint, endPoint));
            update();
        } break;
        case eraser:
        {
            QPainter painter(image);
            painter.setPen(*eraserTool);
            painter.drawLine(startPoint, endPoint);

            int rad = (eraserTool->width() / 2) + 2;
            update(QRect(startPoint, endPoint).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad));
            startPoint = endPoint;
        } break;
        case rect_tool:
            {
            } break;
        default: break;
    }
}

/**
 * @brief DrawArea::setCurrentTool - Sets the current tool, unsetting
 *                                   poly mode if necessary.
 *
 */
void DrawArea::setCurrentTool(const ToolType tool)
{
    // if no change, return --else cancel poly mode & set tool
    if(currentTool == tool)
        return;
    else if(currentTool == line)
        drawingPoly = false;

    currentTool = tool;
}

/**
 * @brief DrawArea::setLineMode - Sets the current line draw mode,
 *                                unsetting poly mode if necessary.
 *
 */
void DrawArea::setLineMode(const DrawType mode)
{
    if(mode == single)
        drawingPoly = false;
    currentLineMode = mode;
}

/**
 * @brief DrawArea::getLineRect - Calculate true distance between start/end points
 *                                and return a rectangle that extends a radius of that
 *                                distance from the line.
 *
 */
QRect DrawArea::getLineRect(const QPoint &startPoint,
                            const QPoint &endPoint)
{
    QPoint point = endPoint - startPoint;
    int rad = (lineTool->width() / 2) + sqrt(pow(point.x(), 2) + pow(point.y(), 2));
    return QRect(startPoint, endPoint).normalized()
                                      .adjusted(-rad, -rad, +rad, +rad);
}
