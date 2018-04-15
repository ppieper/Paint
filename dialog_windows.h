#ifndef DIALOG_WINDOWS_H
#define DIALOG_WINDOWS_H

#include <QSpinBox>
#include <QGroupBox>
#include <QDialog>
#include <QSlider>
#include <QButtonGroup>

#include "constants.h"
#include "tool.h"


class DrawArea;

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    CanvasSizeDialog(QWidget* parent, const char* name = 0,
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

class PenDialog : public QDialog
{
    Q_OBJECT

public:
    PenDialog(QWidget* parent, DrawArea* drawArea, CapStyle = round_cap,
              int size = DEFAULT_PEN_THICKNESS);

private:
    QGroupBox* createCapStyle(CapStyle);

    DrawArea* drawArea;
    QButtonGroup* capStyleG;
    QSlider* penSizeSlider;
};

class LineDialog : public QDialog
{
    Q_OBJECT

public:
    LineDialog(QWidget* parent, DrawArea* drawArea,
                                LineStyle lineStyle = solid,
                                CapStyle capStyle = round_cap,
                                DrawType = single,
                                int thickness = DEFAULT_PEN_THICKNESS);

private:
    QGroupBox* createLineStyle(LineStyle);
    QGroupBox* createCapStyle(CapStyle);
    QGroupBox* createDrawType(DrawType);

    DrawArea* drawArea;
    QButtonGroup* lineStyleG;
    QButtonGroup* capStyleG;
    QButtonGroup* drawTypeG;
    QSlider* lineThicknessSlider;
};

class EraserDialog : public QDialog
{
    Q_OBJECT

public:
    EraserDialog(QWidget* parent, DrawArea* drawArea,
                 int thickness = DEFAULT_ERASER_THICKNESS);

private:
    DrawArea* drawArea;
    QSlider* eraserThicknessSlider;
};

class RectDialog : public QDialog
{
    Q_OBJECT

public:
    RectDialog(QWidget* parent, DrawArea* drawArea,
                                LineStyle = solid, ShapeType = rectangle,
                                FillColor = no_fill, BoundaryType = miter_join,
                                int thickness = DEFAULT_PEN_THICKNESS,
                                int curve = DEFAULT_RECT_CURVE);

private:
    QGroupBox* createBoundaryStyle(LineStyle);
    QGroupBox* createShapeType(ShapeType);
    QGroupBox* createFillColor(FillColor);
    QGroupBox* createBoundaryType(BoundaryType);

    DrawArea* drawArea;
    QButtonGroup* boundaryStyleG;
    QButtonGroup* shapeTypeG;
    QButtonGroup* fillColorG;
    QButtonGroup* boundaryTypeG;
    QSlider* lineThicknessSlider;
    QSlider* rRectCurveSlider;
};

#endif // DIALOGS_H
