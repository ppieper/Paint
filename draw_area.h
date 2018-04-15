#ifndef DRAW_AREA_H
#define DRAW_AREA_H

#include <QUndoStack>


#include "constants.h"
#include "tool.h"


class DrawArea : public QWidget
{
    Q_OBJECT

public:
    DrawArea(QWidget *parent);
    ~DrawArea();

    QPixmap* getImage() { return image; }
    Tool* getCurrentTool() const { return currentTool; }
    QColor getForegroundColor() { return foregroundColor; }
    QColor getBackgroundColor() { return backgroundColor; }

    Tool* setCurrentTool(int);
    void setLineMode(const DrawType mode);

    /** image edit functions */
    void createNewImage(const QSize&);
    void loadImage(const QString&);
    void saveImage(const QString&);
    void resizeImage(const QSize&);
    void clearImage();
    void updateColorConfig(const QColor&, int);

    /** save a command to the undo stack */
    void saveDrawCommand(const QPixmap&);

public slots:
    /** toolbar actions */
    void OnUndo();
    void OnRedo();
    void OnClearAll();

    /** pen tool */
    void OnPenCapConfig(int);
    void OnPenSizeConfig(int);

    /** eraser tool */
    void OnEraserConfig(int);

    /** line tool */
    void OnLineStyleConfig(int);
    void OnLineCapConfig(int);
    void OnDrawTypeConfig(int);
    void OnLineThicknessConfig(int);

    /** rect tool */
    void OnRectBStyleConfig(int);
    void OnRectShapeTypeConfig(int);
    void OnRectFillConfig(int);
    void OnRectBTypeConfig(int);
    void OnRectLineConfig(int);
    void OnRectCurveConfig(int);

protected:
    /** mouse event handler */
    void virtual mousePressEvent(QMouseEvent *event) override;
    void virtual mouseMoveEvent(QMouseEvent *event) override;
    void virtual mouseReleaseEvent(QMouseEvent *event) override;
    void virtual mouseDoubleClickEvent(QMouseEvent *event) override;

    /** paint event handler */
    void virtual paintEvent(QPaintEvent *event) override;

private:
    void createTools();

    /** undo stack */
    QUndoStack* undoStack;

    /** reference to current tool & line mode */
    Tool* currentTool;
    DrawType currentLineMode;

    /** reference to image */
    QPixmap* image;
    QPixmap oldImage;

    /** background/foreground color */
    QColor foregroundColor;
    QColor backgroundColor;

    /** tools */
    PenTool* penTool;
    LineTool* lineTool;
    EraserTool* eraserTool;
    RectTool* rectTool;

    /** state variables */
    bool drawing;
    bool drawingPoly;

    /** Don't allow copying */
    DrawArea(const DrawArea&);
    DrawArea& operator=(const DrawArea&);
};

/** defined in draw_area.cpp */
extern bool imagesEqual(const QPixmap& image1, const QPixmap& image2);

#endif // DRAW_AREA_H
