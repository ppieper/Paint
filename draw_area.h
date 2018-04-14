#ifndef DRAW_AREA_H
#define DRAW_AREA_H

#include "constants.h"


class Tool;
class QPen;
class QPixmap;
class QWidget;
class QPoint;
class QRect;
class QMouseEvent;
class MainWindow;

class DrawArea : public QWidget
{
public:
    DrawArea(QWidget *parent, QPixmap *image, QPen *pen,
              QPen *line, QPen *eraser, QPen *rect, Tool* current);

    void setCurrentTool(Tool* tool);
    void setLineMode(const DrawType mode);

protected:
    /** mouse event handler */
    void virtual mousePressEvent(QMouseEvent *event) override;
    void virtual mouseMoveEvent(QMouseEvent *event) override;
    void virtual mouseReleaseEvent(QMouseEvent *event) override;
    void virtual mouseDoubleClickEvent(QMouseEvent *event) override;

    /** paint event handler */
    void virtual paintEvent(QPaintEvent *event) override;

private:
    void drawTo(const QPoint &endPoint);
    QRect getLineRect(const QPoint &startPoint,
                      const QPoint &endPoint);

    /** reference to the main window */
    MainWindow* mainWindow;

    /** reference to current tool & line mode */
    Tool* currentTool;
    DrawType currentLineMode;

    /** reference to image */
    QPixmap* image;
    QPixmap oldImage;

    /** tools */
    QPen* penTool;
    QPen* lineTool;
    QPen* eraserTool;
    QPen* rectTool;

    /** state variables */
    bool drawing;
    bool drawingPoly;
};

#endif // DRAW_AREA_H
