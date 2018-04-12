#include <qmessagebox.h>
#include <QtWidgets>

#ifndef DIALOG_WINDOWS_H
#define DIALOG_WINDOWS_H

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

enum LineStyle {solid, dashed, dotted, dash_dotted, dash_dot_dotted};
enum CapStyle {flat, square, round_cap};
enum DrawType {single, poly};
enum ShapeType {rectangle, rounded_rectangle, ellipse};
enum FillColor {foreground, background, no_fill};
enum BoundaryType {miter_join, bevel_join, round_join};

class QSpinBox;
class QGroupBox;
class QLabel;

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    CanvasSizeDialog(QWidget* parent, const char* name = 0,
                     int width = DEFAULT_WIDTH,
                     int height = DEFAULT_HEIGHT);

public slots:
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
                                int thickness = 0);

public slots:

private:
    void createSpinBoxes(int,int);
};

class PenDialog : public QDialog
{
    Q_OBJECT

public:
    PenDialog(QWidget* parent, CapStyle = round_cap, int size = 0);

public slots:

private:
    void createSpinBoxes(int,int);

};

class EraserDialog : public QDialog
{
    Q_OBJECT

public:
    EraserDialog(QWidget* parent, int thickness = 0);

public slots:

private:
    void createSpinBoxes(int,int);

};

class RectDialog : public QDialog
{
    Q_OBJECT

public:
    RectDialog(QWidget* parent, LineStyle = solid, ShapeType = rectangle,
                                FillColor = no_fill, BoundaryType = miter_join,
                                int thickness = 0, int curve = 0);

public slots:

private:
    void createSpinBoxes(int,int);

};

#endif // DIALOGS_H
