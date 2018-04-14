#ifndef DRAW_AREA_H
#define DRAW_AREA_H

#include "constants.h"


class Tool;
class MainWindow;

class DrawArea : public QWidget
{
public:
    DrawArea(QWidget *parent, QPixmap *image, QPen *pen,
              QPen *line, QPen *eraser, QPen *rect);

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

    MainWindow* mainWindow;

    /** tools */
    Tool* currentTool;
    DrawType currentLineMode;

    QPen* penTool;
    QPen* lineTool;
    QPen* eraserTool;
    QPen* rectTool;

    QPixmap* image;
    QPixmap oldImage;

    bool drawing;
    bool drawingPoly;
};

#endif // DRAW_AREA_H
