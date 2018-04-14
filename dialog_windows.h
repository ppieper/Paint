#ifndef DIALOG_WINDOWS_H
#define DIALOG_WINDOWS_H

#include <QtWidgets>

#include "constants.h"
#include "tool.h"


class MainWindow;

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    CanvasSizeDialog(QWidget* parent = 0, const char* name = 0,
                     int width = DEFAULT_IMG_WIDTH,
                     int height = DEFAULT_IMG_HEIGHT);

    int getWidthValue() const { return widthSpinBox->value(); }
    int getHeightValue() const { return heightSpinBox->value(); }

private:
    QGroupBox* createSpinBoxes(int,int);

    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QGroupBox *spinBoxesGroup;
};

class LineDialog : public QDialog
{
    Q_OBJECT

public:
    LineDialog(QWidget* parent, LineStyle lineStyle = solid,
                                CapStyle capStyle = round_cap,
                                DrawType = single,
                                int thickness = DEFAULT_PEN_THICKNESS);

private:
    QGroupBox* createLineStyle(LineStyle);
    QGroupBox* createCapStyle(CapStyle);
    QGroupBox* createDrawType(DrawType);

    MainWindow* mainWindow;
    QButtonGroup* lineStyleG;
    QButtonGroup* capStyleG;
    QButtonGroup* drawTypeG;
    QSlider* lineThicknessSlider;
};

class PenDialog : public QDialog
{
    Q_OBJECT

public:
    PenDialog(QWidget* parent, CapStyle = round_cap,
              int size = DEFAULT_PEN_THICKNESS);

private:
    QGroupBox* createCapStyle(CapStyle);

    MainWindow* mainWindow;
    QButtonGroup* capStyleG;
    QSlider* penSizeSlider;
};

class EraserDialog : public QDialog
{
    Q_OBJECT

public:
    EraserDialog(QWidget* parent,
                 int thickness = DEFAULT_ERASER_THICKNESS);

private:
    MainWindow* mainWindow;
    QSlider* eraserThicknessSlider;
};

class RectDialog : public QDialog
{
    Q_OBJECT

public:
    RectDialog(QWidget* parent, LineStyle = solid, ShapeType = rectangle,
                                FillColor = no_fill, BoundaryType = miter_join,
                                int thickness = DEFAULT_PEN_THICKNESS,
                                int curve = DEFAULT_RECT_CURVE);

private:
    QGroupBox* createBoundaryStyle(LineStyle);
    QGroupBox* createShapeType(ShapeType);
    QGroupBox* createFillColor(FillColor);
    QGroupBox* createBoundaryType(BoundaryType);

    MainWindow* mainWindow;
    QButtonGroup* boundaryStyleG;
    QButtonGroup* shapeTypeG;
    QButtonGroup* fillColorG;
    QButtonGroup* boundaryTypeG;
    QSlider* lineThicknessSlider;
    QSlider* rRectCurveSlider;
};

#endif // DIALOGS_H
