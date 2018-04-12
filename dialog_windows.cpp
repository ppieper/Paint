#include "dialog_windows.h"

const int MAX_BRUSH_SIZE = 500;

/**
 * @brief CanvasSizeDialog::CanvasSizeDialog - Dialogue for creating a new canvas.
 *                                             Construct a dialog box containing
 *                                             QSpinBoxes and QPushButtons for this.
 */
CanvasSizeDialog::CanvasSizeDialog(QWidget* parent, const char* name, int width, int height)
    :QDialog(parent)
{
    createSpinBoxes(width,height);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(spinBoxesGroup);
    setLayout(layout);

    setWindowTitle(tr(name));
}

/**
 * @brief NewCanvasDialog::createSpinBoxes - Create the QSpinBoxes for the dialog
 *                                           box as well as the buttons.
 */
void CanvasSizeDialog::createSpinBoxes(int width, int height)
{
    spinBoxesGroup = new QGroupBox(tr("Image Size"), this);

    // the width field
    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setRange(1, 10000);
    widthSpinBox->setSingleStep(1);
    widthSpinBox->setValue(width);
    widthSpinBox->setSuffix("px");

    // the height field
    heightSpinBox = new QSpinBox(this);
    heightSpinBox->setRange(1, 10000);
    heightSpinBox->setSingleStep(1);
    heightSpinBox->setValue(height);
    heightSpinBox->setSuffix("px");

    // the buttons
    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    // put it all together
    QFormLayout *spinBoxLayout = new QFormLayout;
    spinBoxLayout->addRow(tr("Width: "), widthSpinBox);
    spinBoxLayout->addRow(tr("Height: "), heightSpinBox);
    spinBoxLayout->addRow(okButton);
    spinBoxLayout->addRow(cancelButton);
    spinBoxesGroup->setLayout(spinBoxLayout);
}

/**
 * @brief NewCanvasDialog::getWidthValue
 *
 * @return the width selected by the user
 */
int CanvasSizeDialog::getWidthValue() const
{
    return widthSpinBox->value();
}

/**
 * @brief NewCanvasDialog::getHeightValue
 *
 * @return the height selected by the user
 */
int CanvasSizeDialog::getHeightValue() const
{
    return heightSpinBox->value();
}

/**
 * @brief LineDialog::LineSizeDialog - Dialogue selecting what kind of line to draw.
 */
LineDialog::LineDialog(QWidget* parent, LineStyle lineStyle,
                                        CapStyle capStyle,
                                        DrawType drawType,
                                        int thickness)
    :QDialog(parent)
{

    setWindowTitle(tr("Line Dialog"));

    QGroupBox *lineStyles = new QGroupBox(tr("Line Style"), this);
    QRadioButton *solidButton = new QRadioButton(tr("Solid"), this);
    QRadioButton *dashedButton = new QRadioButton(tr("Dashed"), this);
    QRadioButton *dottedButton = new QRadioButton(tr("Dotted"), this);
    QRadioButton *dashDottedButton = new QRadioButton(tr("Dash-Dotted"), this);
    QRadioButton *dashDotDottedButton = new QRadioButton(tr("Dash-Dot-Dotted"), this);

    switch(lineStyle)
    {
    case solid: solidButton->setChecked(true); break;
    case dashed: dashedButton->setChecked(true);break;
    case dotted: dottedButton->setChecked(true);break;
    case dash_dotted: dashDottedButton->setChecked(true); break;
    case dash_dot_dotted: dashDotDottedButton->setChecked(true);break;
    default: break;
    }

    QVBoxLayout *vbox0 = new QVBoxLayout(this);
    vbox0->addWidget(solidButton);
    vbox0->addWidget(dashedButton);
    vbox0->addWidget(dottedButton);
    vbox0->addWidget(dashDottedButton);
    vbox0->addWidget(dashDotDottedButton);
    lineStyles->setLayout(vbox0);

    QGroupBox*left = new QGroupBox(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(lineStyles);
    left->setLayout(vbox);

    QGroupBox *capStyles = new QGroupBox(tr("Cap Style"), this);
    QRadioButton *flatButton = new QRadioButton(tr("Flat"), this);
    QRadioButton *squareButton = new QRadioButton(tr("Square"), this);
    QRadioButton *roundButton = new QRadioButton(tr("Round"), this);

    switch(capStyle)
    {
    case flat: flatButton->setChecked(true); break;
    case square: squareButton->setChecked(true);break;
    case round_cap: roundButton->setChecked(true);break;
    default: break;
    }

    QVBoxLayout *vbox1 = new QVBoxLayout(this);
    vbox1->addWidget(flatButton);
    vbox1->addWidget(squareButton);
    vbox1->addWidget(roundButton);
    capStyles->setLayout(vbox1);

    QGroupBox *drawTypes = new QGroupBox(tr("Draw Type"), this);
    QRadioButton *singleButton = new QRadioButton(tr("Single"), this);
    QRadioButton *polyButton = new QRadioButton(tr("Poly"), this);

    switch(drawType)
    {
    case single: singleButton->setChecked(true); break;
    case poly: polyButton->setChecked(true);break;
    default: break;
    }

    QVBoxLayout *vbox2 = new QVBoxLayout(this);
    vbox2->addWidget(singleButton);
    vbox2->addWidget(polyButton);
    drawTypes->setLayout(vbox2);

    QGroupBox *right = new QGroupBox(this);

    QVBoxLayout *vbox3 = new QVBoxLayout(this);
    vbox3->addWidget(capStyles);
    vbox3->addWidget(drawTypes);
    right->setLayout(vbox3);

    QLabel *lineThicknessLabel = new QLabel(tr("Line Thickness"), this);
    QSlider *lineThickness = new QSlider(Qt::Horizontal, this);
    lineThickness->setMaximum(MAX_BRUSH_SIZE);
    lineThickness->setSliderPosition(thickness);

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(left, 0,0);
    grid->addWidget(right, 0, 1);
    grid->addWidget(lineThicknessLabel, 2, 0, 1, 2);
    grid->addWidget(lineThickness, 3, 0, 1, 2);
    setLayout(grid);
}

/**
 * @brief PenDialog::PenDialog - Dialogue for selecting pen size and cap style.
 *
 */
PenDialog::PenDialog(QWidget* parent, CapStyle capStyle, int size)
    :QDialog(parent)
{
    setWindowTitle(tr("Pen Dialog"));

    QGroupBox *capStyles = new QGroupBox(tr("Cap Style"), this);
    QRadioButton *flatButton = new QRadioButton(tr("Flat"), this);
    QRadioButton *squareButton = new QRadioButton(tr("Square"), this);
    QRadioButton *roundButton = new QRadioButton(tr("Round"), this);

    switch(capStyle)
    {
    case flat: flatButton->setChecked(true); break;
    case square: squareButton->setChecked(true);break;
    case round_cap: roundButton->setChecked(true);break;
    default: break;
    }

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(flatButton);
    hbox->addWidget(squareButton);
    hbox->addWidget(roundButton);
    capStyles->setLayout(hbox);

    QLabel *penSizeLabel = new QLabel(tr("Pen Size"), this);
    QSlider *penSize = new QSlider(Qt::Horizontal, this);
    penSize->setMaximum(MAX_BRUSH_SIZE);
    penSize->setSliderPosition(size);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(capStyles);
    vbox->addWidget(penSizeLabel);
    vbox->addWidget(penSize);
    setLayout(vbox);
}

/**
 * @brief EraserDialog::EraserDialog - Dialogue for choosing eraser thickness.
 *
 */
EraserDialog::EraserDialog(QWidget* parent, int thickness)
    :QDialog(parent)
{
    setWindowTitle(tr("Eraser Dialog"));

    QLabel *eraserThicknessLabel = new QLabel(tr("Eraser Thickness"), this);
    QSlider *eraserThickness = new QSlider(Qt::Horizontal, this);
    eraserThickness->setMaximum(MAX_BRUSH_SIZE);
    eraserThickness->setSliderPosition(thickness);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(eraserThicknessLabel);
    vbox->addWidget(eraserThickness);
    setLayout(vbox);
}

/**
 * @brief RectDialog::RectDialog - Dialogue for selecting what kind of rectangle to draw.
 *
 */
RectDialog::RectDialog(QWidget* parent, LineStyle boundaryStyle, ShapeType shapeType,
                                        FillColor fillColor, BoundaryType boundaryType,
                                        int thickness, int curve)
    :QDialog(parent)
{
    setWindowTitle(tr("Rectangle Dialog"));

    QGroupBox *lineStyles = new QGroupBox(tr("Boundary Style"), this);
    QRadioButton *solidButton = new QRadioButton(tr("Solid"), this);
    QRadioButton *dashedButton = new QRadioButton(tr("Dashed"), this);
    QRadioButton *dottedButton = new QRadioButton(tr("Dotted"), this);
    QRadioButton *dashDottedButton = new QRadioButton(tr("Dash-Dotted"), this);
    QRadioButton *dashDotDottedButton = new QRadioButton(tr("Dash-Dot-Dotted"), this);

    switch(boundaryStyle)
    {
    case solid: solidButton->setChecked(true); break;
    case dashed: dashedButton->setChecked(true);break;
    case dotted: dottedButton->setChecked(true);break;
    case dash_dotted: dashDottedButton->setChecked(true); break;
    case dash_dot_dotted: dashDotDottedButton->setChecked(true);break;
    default: break;
    }

    QVBoxLayout *vbox0 = new QVBoxLayout(this);
    vbox0->addWidget(solidButton);
    vbox0->addWidget(dashedButton);
    vbox0->addWidget(dottedButton);
    vbox0->addWidget(dashDottedButton);
    vbox0->addWidget(dashDotDottedButton);
    lineStyles->setLayout(vbox0);

    QGroupBox*left = new QGroupBox(this);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(lineStyles);
    left->setLayout(vbox);

    QGroupBox *shapeTypes = new QGroupBox(tr("Shape Type"), this);
    QRadioButton *rectangleButton = new QRadioButton(tr("Rectangle"), this);
    QRadioButton *rRectangleButton = new QRadioButton(tr("Rounded Rectangle"), this);
    QRadioButton *ellipseButton = new QRadioButton(tr("Ellipse"), this);

    switch(shapeType)
    {
    case rectangle: rectangleButton->setChecked(true); break;
    case rounded_rectangle: rRectangleButton->setChecked(true); break;
    case ellipse: ellipseButton->setChecked(true); break;
    default: break;
    }

    QVBoxLayout *vbox1 = new QVBoxLayout(this);
    vbox1->addWidget(rectangleButton);
    vbox1->addWidget(rRectangleButton);
    vbox1->addWidget(ellipseButton);
    shapeTypes->setLayout(vbox1);

    QGroupBox *fillColors = new QGroupBox(tr("Fill Color"), this);
    QRadioButton *foregroundButton = new QRadioButton(tr("Foreground"), this);
    QRadioButton *backgroundButton = new QRadioButton(tr("Background"), this);
    QRadioButton *noFillButton = new QRadioButton(tr("No Fill"), this);

    switch(fillColor)
    {
    case foreground: foregroundButton->setChecked(true); break;
    case background: backgroundButton->setChecked(true); break;
    case no_fill: noFillButton->setChecked(true); break;
    default: break;
    }

    QVBoxLayout *vbox2 = new QVBoxLayout(this);
    vbox2->addWidget(foregroundButton);
    vbox2->addWidget(backgroundButton);
    vbox2->addWidget(noFillButton);
    fillColors->setLayout(vbox2);

    QGroupBox *boundaryTypes = new QGroupBox(tr("Boundary Type"), this);
    QRadioButton *miterJoinButton = new QRadioButton(tr("Miter Join"), this);
    QRadioButton *bevelJoinButton = new QRadioButton(tr("Bevel Join"), this);
    QRadioButton *roundJoinButton = new QRadioButton(tr("Round Join"), this);

    switch(boundaryType)
    {
    case miter_join: miterJoinButton->setChecked(true); break;
    case bevel_join: bevelJoinButton->setChecked(true); break;
    case round_join: roundJoinButton->setChecked(true); break;
    default: break;
    }

    QVBoxLayout *vbox4 = new QVBoxLayout(this);
    vbox4->addWidget(miterJoinButton);
    vbox4->addWidget(bevelJoinButton);
    vbox4->addWidget(roundJoinButton);
    boundaryTypes->setLayout(vbox4);

    QGroupBox *right = new QGroupBox(this);

    QVBoxLayout *vbox3 = new QVBoxLayout(this);
    vbox3->addWidget(shapeTypes);
    vbox3->addWidget(fillColors);
    vbox3->addWidget(boundaryTypes);
    right->setLayout(vbox3);

    QLabel *lineThicknessLabel = new QLabel(tr("Line Thickness"), this);
    QSlider *lineThickness = new QSlider(Qt::Horizontal, this);
    lineThickness->setMaximum(MAX_BRUSH_SIZE);
    lineThickness->setSliderPosition(thickness);

    QLabel *rRectCurveLabel = new QLabel(tr("Rounded Rectangle Curve"), this);
    QSlider *rRectCurve = new QSlider(Qt::Horizontal, this);
    rRectCurve->setMaximum(MAX_BRUSH_SIZE);
    rRectCurve->setSliderPosition(curve);

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(left, 0,0);
    grid->addWidget(right, 0, 1);
    grid->addWidget(lineThicknessLabel, 3, 0, 1, 2);
    grid->addWidget(lineThickness, 4, 0, 1, 2);
    grid->addWidget(rRectCurveLabel, 5, 0, 1, 2);
    grid->addWidget(rRectCurve, 6, 0, 1, 2);
    setLayout(grid);
}
