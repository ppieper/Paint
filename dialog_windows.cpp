#include "dialog_windows.h"
#include "main_window.h"

const int MIN_BRUSH_SIZE = 1;
const int MAX_BRUSH_SIZE = 50;

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
 * @brief PenDialog::PenDialog - Dialogue for selecting pen size and cap style.
 *
 */
PenDialog::PenDialog(QWidget* parent, CapStyle capStyle, int size)
    :QDialog(parent)
{
    setWindowTitle(tr("Pen Dialog"));

    mainWindow = (MainWindow*)this->parent();

    QLabel *penSizeLabel = new QLabel(tr("Pen Size"), this);
    penSizeSlider = new QSlider(Qt::Horizontal, this);
    penSizeSlider->setMinimum(MIN_BRUSH_SIZE);
    penSizeSlider->setMaximum(MAX_BRUSH_SIZE);
    penSizeSlider->setSliderPosition(size);
    penSizeSlider->setTracking(false);
    connect(penSizeSlider, SIGNAL(valueChanged(int)), mainWindow, SLOT(OnPenSizeConfig(int)));

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(createCapStyle(capStyle));
    vbox->addWidget(penSizeLabel);
    vbox->addWidget(penSizeSlider);
    setLayout(vbox);
}

QGroupBox* PenDialog::createCapStyle(CapStyle capStyle)
{
    QGroupBox *capStyles = new QGroupBox(tr("Cap Style"), this);
    QRadioButton *flatButton = new QRadioButton(tr("Flat"), this);
    QRadioButton *squareButton = new QRadioButton(tr("Square"), this);
    QRadioButton *roundButton = new QRadioButton(tr("Round"), this);

    mainWindow = (MainWindow*)this->parent();

    capStyleG = new QButtonGroup(this);
    capStyleG->addButton(flatButton, 0);
    capStyleG->addButton(squareButton, 1);
    capStyleG->addButton(roundButton, 2);

    connect(capStyleG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnPenCapConfig(int)));

    switch(capStyle)
    {
    case flat: flatButton->setChecked(true);       break;
    case square: squareButton->setChecked(true);   break;
    case round_cap: roundButton->setChecked(true); break;
    default: break;
    }

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(flatButton);
    hbox->addWidget(squareButton);
    hbox->addWidget(roundButton);
    capStyles->setLayout(hbox);

    return capStyles;
}

/**
 * @brief LineDialog::LineSizeDialog - Dialogue for selecting what kind of line to draw.
 *
 */
LineDialog::LineDialog(QWidget* parent, LineStyle lineStyle,
                                        CapStyle capStyle,
                                        DrawType drawType,
                                        int thickness)
    :QDialog(parent)
{
    setWindowTitle(tr("Line Dialog"));

    QGroupBox*left = new QGroupBox(this);

    mainWindow = (MainWindow*)this->parent();

    QVBoxLayout *vboxL = new QVBoxLayout(this);
    vboxL->addWidget(createLineStyle(lineStyle));
    left->setLayout(vboxL);

    QGroupBox *right = new QGroupBox(this);

    QVBoxLayout *vboxR = new QVBoxLayout(this);
    vboxR->addWidget(createCapStyle(capStyle));
    vboxR->addWidget(createDrawType(drawType));
    right->setLayout(vboxR);

    QLabel *lineThicknessLabel = new QLabel(tr("Line Thickness"), this);
    lineThicknessSlider = new QSlider(Qt::Horizontal, this);
    lineThicknessSlider->setMinimum(MIN_BRUSH_SIZE);
    lineThicknessSlider->setMaximum(MAX_BRUSH_SIZE);
    lineThicknessSlider->setSliderPosition(thickness);
    lineThicknessSlider->setTracking(false);
    connect(lineThicknessSlider, SIGNAL(valueChanged(int)), mainWindow, SLOT(OnLineThicknessConfig(int)));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(left, 0,0);
    grid->addWidget(right, 0, 1);
    grid->addWidget(lineThicknessLabel, 2, 0, 1, 2);
    grid->addWidget(lineThicknessSlider, 3, 0, 1, 2);
    setLayout(grid);
}

QGroupBox* LineDialog::createLineStyle(LineStyle lineStyle)
{
    QGroupBox *lineStyles = new QGroupBox(tr("Line Style"), this);
    QRadioButton *solidButton = new QRadioButton(tr("Solid"), this);
    QRadioButton *dashedButton = new QRadioButton(tr("Dashed"), this);
    QRadioButton *dottedButton = new QRadioButton(tr("Dotted"), this);
    QRadioButton *dashDottedButton = new QRadioButton(tr("Dash-Dotted"), this);
    QRadioButton *dashDotDottedButton = new QRadioButton(tr("Dash-Dot-Dotted"), this);

    lineStyleG = new QButtonGroup(this);
    lineStyleG->addButton(solidButton, 0);
    lineStyleG->addButton(dashedButton, 1);
    lineStyleG->addButton(dottedButton, 2);
    lineStyleG->addButton(dashDottedButton, 3);
    lineStyleG->addButton(dashDotDottedButton, 4);

    connect(lineStyleG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnLineStyleConfig(int)));

    switch(lineStyle)
    {
    case solid: solidButton->setChecked(true);                   break;
    case dashed: dashedButton->setChecked(true);                 break;
    case dotted: dottedButton->setChecked(true);                 break;
    case dash_dotted: dashDottedButton->setChecked(true);        break;
    case dash_dot_dotted: dashDotDottedButton->setChecked(true); break;
    default: break;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(solidButton);
    vbox->addWidget(dashedButton);
    vbox->addWidget(dottedButton);
    vbox->addWidget(dashDottedButton);
    vbox->addWidget(dashDotDottedButton);
    lineStyles->setLayout(vbox);

    return lineStyles;
}

QGroupBox* LineDialog::createCapStyle(CapStyle capStyle)
{
    QGroupBox *capStyles = new QGroupBox(tr("Cap Style"), this);
    QRadioButton *flatButton = new QRadioButton(tr("Flat"), this);
    QRadioButton *squareButton = new QRadioButton(tr("Square"), this);
    QRadioButton *roundButton = new QRadioButton(tr("Round"), this);

    capStyleG = new QButtonGroup(this);
    capStyleG->addButton(flatButton, 0);
    capStyleG->addButton(squareButton, 1);
    capStyleG->addButton(roundButton, 2);

    connect(capStyleG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnLineCapConfig(int)));

    switch(capStyle)
    {
    case flat: flatButton->setChecked(true);       break;
    case square: squareButton->setChecked(true);   break;
    case round_cap: roundButton->setChecked(true); break;
    default: break;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(flatButton);
    vbox->addWidget(squareButton);
    vbox->addWidget(roundButton);
    capStyles->setLayout(vbox);

    return capStyles;
}

QGroupBox* LineDialog::createDrawType(DrawType drawType)
{
    QGroupBox *drawTypes = new QGroupBox(tr("Draw Type"), this);
    QRadioButton *singleButton = new QRadioButton(tr("Single"), this);
    QRadioButton *polyButton = new QRadioButton(tr("Poly"), this);

    drawTypeG = new QButtonGroup(this);
    drawTypeG->addButton(singleButton, 0);
    drawTypeG->addButton(polyButton, 1);

    connect(drawTypeG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnDrawTypeConfig(int)));

    switch(drawType)
    {
    case single: singleButton->setChecked(true); break;
    case poly: polyButton->setChecked(true);     break;
    default: break;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(singleButton);
    vbox->addWidget(polyButton);
    drawTypes->setLayout(vbox);

    return drawTypes;
}

/**
 * @brief EraserDialog::EraserDialog - Dialogue for choosing eraser thickness.
 *
 */
EraserDialog::EraserDialog(QWidget* parent, int thickness)
    :QDialog(parent)
{
    setWindowTitle(tr("Eraser Dialog"));

    MainWindow* mainWindow = (MainWindow*)this->parent();

    QLabel *eraserThicknessLabel = new QLabel(tr("Eraser Thickness"), this);
    eraserThicknessSlider = new QSlider(Qt::Horizontal, this);
    eraserThicknessSlider->setMinimum(MIN_BRUSH_SIZE);
    eraserThicknessSlider->setMaximum(MAX_BRUSH_SIZE);
    eraserThicknessSlider->setSliderPosition(thickness);
    eraserThicknessSlider->setTracking(false);
    connect(eraserThicknessSlider, SIGNAL(valueChanged(int)), mainWindow, SLOT(OnEraserConfig(int)));

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(eraserThicknessLabel);
    vbox->addWidget(eraserThicknessSlider);
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

    QGroupBox*left = new QGroupBox(this);

    QVBoxLayout *vboxL = new QVBoxLayout(this);
    vboxL->addWidget(createBoundaryStyle(boundaryStyle));
    left->setLayout(vboxL);

    QGroupBox *right = new QGroupBox(this);

    QVBoxLayout *vboxR = new QVBoxLayout(this);
    vboxR->addWidget(createShapeType(shapeType));
    vboxR->addWidget(createFillColor(fillColor));
    vboxR->addWidget(createBoundaryType(boundaryType));
    right->setLayout(vboxR);

    QLabel *lineThicknessLabel = new QLabel(tr("Line Thickness"), this);
    lineThicknessSlider = new QSlider(Qt::Horizontal, this);
    lineThicknessSlider->setMinimum(MIN_BRUSH_SIZE);
    lineThicknessSlider->setMaximum(MAX_BRUSH_SIZE);
    lineThicknessSlider->setSliderPosition(thickness);
    lineThicknessSlider->setTracking(false);
    //connect(lineThicknessSlider, SIGNAL(valueChanged(int)), mainWindow, SLOT(OnRectLineConfig(int)));

    QLabel *rRectCurveLabel = new QLabel(tr("Rounded Rectangle Curve"), this);
    rRectCurveSlider = new QSlider(Qt::Horizontal, this);
    rRectCurveSlider->setMinimum(MIN_BRUSH_SIZE);
    rRectCurveSlider->setMaximum(MAX_BRUSH_SIZE);
    rRectCurveSlider->setSliderPosition(curve);
    rRectCurveSlider->setTracking(false);
    //connect(rRectCurveSlider, SIGNAL(valueChanged(int)), mainWindow, SLOT(OnRectCurveConfig(int)));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(left, 0,0);
    grid->addWidget(right, 0, 1);
    grid->addWidget(lineThicknessLabel, 1, 0, 1, 2);
    grid->addWidget(lineThicknessSlider, 2, 0, 1, 2);
    grid->addWidget(rRectCurveLabel, 3, 0, 1, 2);
    grid->addWidget(rRectCurveSlider, 4, 0, 1, 2);
    setLayout(grid);
}

QGroupBox* RectDialog::createBoundaryStyle(LineStyle boundaryStyle)
{
    QGroupBox *boundaryStyles = new QGroupBox(tr("Boundary Style"), this);
    QRadioButton *solidButton = new QRadioButton(tr("Solid"), this);
    QRadioButton *dashedButton = new QRadioButton(tr("Dashed"), this);
    QRadioButton *dottedButton = new QRadioButton(tr("Dotted"), this);
    QRadioButton *dashDottedButton = new QRadioButton(tr("Dash-Dotted"), this);
    QRadioButton *dashDotDottedButton = new QRadioButton(tr("Dash-Dot-Dotted"), this);

    boundaryStyleG = new QButtonGroup(this);
    boundaryStyleG->addButton(solidButton, 0 );
    boundaryStyleG->addButton(dashedButton, 1);
    boundaryStyleG->addButton(dottedButton, 2);
    boundaryStyleG->addButton(dashDottedButton, 3);
    boundaryStyleG->addButton(dashDotDottedButton, 4);

    //connect(boundaryStyleG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnRectBStyleConfig(int)));

    switch(boundaryStyle)
    {
    case solid: solidButton->setChecked(true);                   break;
    case dashed: dashedButton->setChecked(true);                 break;
    case dotted: dottedButton->setChecked(true);                 break;
    case dash_dotted: dashDottedButton->setChecked(true);        break;
    case dash_dot_dotted: dashDotDottedButton->setChecked(true); break;
    default: break;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(solidButton);
    vbox->addWidget(dashedButton);
    vbox->addWidget(dottedButton);
    vbox->addWidget(dashDottedButton);
    vbox->addWidget(dashDotDottedButton);
    boundaryStyles->setLayout(vbox);

    return boundaryStyles;
}

QGroupBox* RectDialog::createShapeType(ShapeType shapeType)
{
    QGroupBox *shapeTypes = new QGroupBox(tr("Shape Type"), this);
    QRadioButton *rectangleButton = new QRadioButton(tr("Rectangle"), this);
    QRadioButton *rRectangleButton = new QRadioButton(tr("Rounded Rectangle"), this);
    QRadioButton *ellipseButton = new QRadioButton(tr("Ellipse"), this);

    shapeTypeG = new QButtonGroup(this);
    shapeTypeG->addButton(rectangleButton, 0);
    shapeTypeG->addButton(rRectangleButton, 1);
    shapeTypeG->addButton(ellipseButton, 2);

    //connect(shapeTypeG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnShapeTypeConfig(int)));

    switch(shapeType)
    {
    case rectangle: rectangleButton->setChecked(true);          break;
    case rounded_rectangle: rRectangleButton->setChecked(true); break;
    case ellipse: ellipseButton->setChecked(true);              break;
    default: break;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(rectangleButton);
    vbox->addWidget(rRectangleButton);
    vbox->addWidget(ellipseButton);
    shapeTypes->setLayout(vbox);

    return shapeTypes;
}

QGroupBox* RectDialog::createFillColor(FillColor fillColor)
{
    QGroupBox *fillColors = new QGroupBox(tr("Fill Color"), this);
    QRadioButton *foregroundButton = new QRadioButton(tr("Foreground"), this);
    QRadioButton *backgroundButton = new QRadioButton(tr("Background"), this);
    QRadioButton *noFillButton = new QRadioButton(tr("No Fill"), this);

    fillColorG = new QButtonGroup(this);
    fillColorG->addButton(foregroundButton, 0);
    fillColorG->addButton(backgroundButton, 1);
    fillColorG->addButton(noFillButton, 2);

    //connect(fillColorG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnRectFillConfig(int)));

    switch(fillColor)
    {
    case foreground: foregroundButton->setChecked(true); break;
    case background: backgroundButton->setChecked(true); break;
    case no_fill: noFillButton->setChecked(true);        break;
    default: break;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(foregroundButton);
    vbox->addWidget(backgroundButton);
    vbox->addWidget(noFillButton);
    fillColors->setLayout(vbox);

    return fillColors;

}

QGroupBox* RectDialog::createBoundaryType(BoundaryType boundaryType)
{
    QGroupBox *boundaryTypes = new QGroupBox(tr("Boundary Type"), this);
    QRadioButton *miterJoinButton = new QRadioButton(tr("Miter Join"), this);
    QRadioButton *bevelJoinButton = new QRadioButton(tr("Bevel Join"), this);
    QRadioButton *roundJoinButton = new QRadioButton(tr("Round Join"), this);

    boundaryTypeG = new QButtonGroup(this);
    boundaryTypeG->addButton(miterJoinButton, 0);
    boundaryTypeG->addButton(bevelJoinButton, 1);
    boundaryTypeG->addButton(roundJoinButton, 2);

    //connect(boundaryTypeG, SIGNAL(buttonClicked(int)), mainWindow, SLOT(OnRectBTypeConfig(int)));

    switch(boundaryType)
    {
    case miter_join: miterJoinButton->setChecked(true); break;
    case bevel_join: bevelJoinButton->setChecked(true); break;
    case round_join: roundJoinButton->setChecked(true); break;
    default: break;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(miterJoinButton);
    vbox->addWidget(bevelJoinButton);
    vbox->addWidget(roundJoinButton);
    boundaryTypes->setLayout(vbox);

    return boundaryTypes;
}
