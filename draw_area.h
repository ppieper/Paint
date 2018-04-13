#ifndef PAINT_AREA_H
#define PAINT_AREA_H

#include "dialog_windows.h"


enum ToolType {pen, line, eraser, rect_tool};

class DrawArea : public QWidget
{
public:
    DrawArea(QWidget *parent, QPixmap *image, QPen *pen,
              QPen *line, QPen *eraser, QPen *rect);

    void setCurrentTool(const ToolType tool);
    void setLineMode(const DrawType mode);

protected:
    /** mouse event handler */
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /** paint event handler */
    void paintEvent(QPaintEvent *event) override;

private:
    void drawTo(const QPoint &endPoint);
    QRect getLineRect(const QPoint &startPoint,
                      const QPoint &endPoint);

    MainWindow* mainWindow;

    /** tools */
    ToolType currentTool;
    DrawType currentLineMode;

    QPen* penTool;
    QPen* lineTool;
    QPen* eraserTool;
    QPen* rectTool;

    QPixmap* image;
    QPixmap oldImage;
    QPoint startPoint;
    bool drawing;
    bool drawingPoly;
};

#endif
