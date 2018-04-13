#ifndef PAINT_AREA_H
#define PAINT_AREA_H

#include <QPixmap>
#include <QPoint>
#include <QWidget>


enum ToolType {pen, line, eraser, rect_tool};

class DrawArea : public QWidget
{
public:
    DrawArea(QWidget *parent = 0, QPixmap *image = 0, QPen *pen = 0,
              QPen *line = 0, QPen *eraser = 0, QPen *rect = 0);

    void setCurrentTool(ToolType tool) { currentTool = tool; }

protected:
    /** mouse event handler */
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    /** paint event handler */
    void paintEvent(QPaintEvent *event) override;

private:
    void drawTo(const QPoint &endPoint);

    /** tools */

    ToolType currentTool;
    QPen* penTool;
    QPen* lineTool;
    QPen* eraserTool;
    QPen* rectTool;

    QPixmap* image;
    QPixmap oldImage;
    QPoint startPoint;
};

#endif
