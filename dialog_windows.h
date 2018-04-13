#ifndef DIALOG_WINDOWS_H
#define DIALOG_WINDOWS_H

#include <QtWidgets>


const int DEFAULT_IMG_WIDTH = 640;
const int DEFAULT_IMG_HEIGHT = 480;
const int DEFAULT_PEN_THICKNESS = 1;
const int DEFAULT_ERASER_THICKNESS = 10;

enum LineStyle {solid, dashed, dotted, dash_dotted, dash_dot_dotted};
enum CapStyle {flat, square, round_cap};
enum DrawType {single, poly};
enum ShapeType {rectangle, rounded_rectangle, ellipse};
enum FillColor {foreground, background, no_fill};
enum BoundaryType {miter_join, bevel_join, round_join};

class MainWindow;

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    CanvasSizeDialog(QWidget* parent = 0, const char* name = 0,
                     int width = DEFAULT_IMG_WIDTH,
                     int height = DEFAULT_IMG_HEIGHT);

    int getWidthValue() const;
    int getHeightValue() const;

private:
    void createSpinBoxes(int,int);

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

public slots:

private:
    MainWindow* mainWindow;
    QButtonGroup* lineStyleG;
    QButtonGroup* capStyleG;
    QButtonGroup* drawTypeG;
    QSlider* lineThicknessSlider;
    QGroupBox* createLineStyle(LineStyle);
    QGroupBox* createCapStyle(CapStyle);
    QGroupBox* createDrawType(DrawType);
};

class PenDialog : public QDialog
{
    Q_OBJECT

public:
    PenDialog(QWidget* parent, CapStyle = round_cap,
              int size = DEFAULT_PEN_THICKNESS);
public slots:

private:
    MainWindow* mainWindow;
    QButtonGroup* capStyleG;
    QSlider* penSizeSlider;
    QGroupBox* createCapStyle(CapStyle);
};

class EraserDialog : public QDialog
{
    Q_OBJECT

public:
    EraserDialog(QWidget* parent,
                 int thickness = DEFAULT_ERASER_THICKNESS);

public slots:

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
                                int curve = DEFAULT_PEN_THICKNESS);

public slots:

private:
    MainWindow* mainWindow;
    QButtonGroup* boundaryStyleG;
    QButtonGroup* shapeTypeG;
    QButtonGroup* fillColorG;
    QButtonGroup* boundaryTypeG;
    QSlider* lineThicknessSlider;
    QSlider* rRectCurveSlider;
    QGroupBox* createBoundaryStyle(LineStyle);
    QGroupBox* createShapeType(ShapeType);
    QGroupBox* createFillColor(FillColor);
    QGroupBox* createBoundaryType(BoundaryType);
};

#endif // DIALOGS_H
