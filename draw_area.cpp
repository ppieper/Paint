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
                     QPen *eraserTool, QPen *rectTool,
                     Tool* currentTool)
    : QWidget(parent)
{
    // initialize image and tools
    this->image = image;
    this->penTool = penTool;
    this->lineTool = lineTool;
    this->eraserTool = eraserTool;
    this->rectTool = rectTool;
    this->currentTool = currentTool;

    // get the reference to the main window
    mainWindow = (MainWindow*)this->parent();

    // default tool
    currentTool = (Tool*) penTool;

    // initialize state variables
    drawing = false;
    drawingPoly = false;
    currentLineMode = single;

    // small optimizations
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
            currentTool->setStartPoint(e->pos());

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

        ToolType type = currentTool->getType();
        if(type == line || type == rect_tool)
        {
            *image = oldImage;
            if(type == line && currentLineMode == poly)
            {
                drawingPoly = true;
            }
        }
        currentTool->drawTo(e->pos(), this);
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
            currentTool->setStartPoint(e->pos());
            //return;
        }
        if(currentTool->getType() == pen)
            currentTool->drawTo(e->pos(), this);

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
 * @brief DrawArea::setCurrentTool - Sets the current tool, unsetting
 *                                   poly mode if necessary.
 *
 */
void DrawArea::setCurrentTool(Tool* tool)
{
    // if no change, return --else cancel poly mode & set tool
    if(currentTool == tool)
        return;
    else if(currentTool->getType() == line)
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
