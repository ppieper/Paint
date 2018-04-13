#include <QtWidgets>

#include "draw_area.h"
#include "main_window.h"


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

    setAttribute(Qt::WA_StaticContents);
}

void DrawArea::paintEvent(QPaintEvent *e)

{
    QPainter painter(this);
    QRect modifiedArea = e->rect(); // only need to redraw a small area
    painter.drawPixmap(modifiedArea, *image, modifiedArea);
}

void DrawArea::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton)
    {
        // open the dialog menu
        MainWindow* mainWindow = (MainWindow*)this->parent();
        mainWindow->mousePressEvent(e);
    }
    else
    {
        // save a copy of the old image
        oldImage = image->copy(QRect());
        startPoint = e->pos();
    }
}

void DrawArea::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        if(image->isNull())
            return;

        drawTo(e->pos());
    }
}

void DrawArea::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if(image->isNull())
            return;

        drawTo(e->pos());

        // for undo/redo
        MainWindow* mainWindow = (MainWindow*)this->parent();
        mainWindow->saveDrawCommand(oldImage);
    }
}

void DrawArea::drawTo(const QPoint &endPoint)
{
    switch(currentTool)
    {
        case pen:
        {
            QPainter painter(image);
            //painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(*penTool);
            painter.drawLine(startPoint, endPoint);

            int rad = (penTool->width() / 2) + 2;
            update(QRect(startPoint, endPoint).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad));
            startPoint = endPoint;
        } break;
        case line:
        {
        } break;
        case eraser:
        {
            QPainter painter(image);
            //painter.setRenderHint(QPainter::Antialiasing);
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
