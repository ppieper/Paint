#include <iostream>
using namespace std;

#include "main_window.h"
#include "dialog_windows.h"

const int TOOLBAR_HEIGHT = 39;
const int BACKGROUND = 1;
const int FOREGROUND = 0;

/**
 * @brief MainWindow::MainWindow - the main window, parent to every other widget.
 */
MainWindow::MainWindow(QWidget* parent, const char* name)
    :QMainWindow(parent)
{
    // create menu items
    QMenu* file = new QMenu("File", this);
    file->addAction("New image...", this, SLOT(OnNewImage()));
    file->addAction("Load image...", this, SLOT(OnLoadImage()));
    file->addAction("Save image...", this, SLOT(OnSaveImage()));
    menuBar()->addMenu(file);
    menuBar()->setNativeMenuBar(false);

    // create the toolbar
    toolbar = new ToolBar(this);
    addToolBar(toolbar);
    image = new QPixmap();

    setWindowTitle(name);
    resize(QDesktopWidget().availableGeometry(this).size()*.6);
    setContextMenuPolicy(Qt::PreventContextMenu);
}

MainWindow::~MainWindow()
{
    delete image;
}

void MainWindow::paintEvent(QPaintEvent* e)
{
	QPainter paint(this);
	if (! image->isNull())
	{
        paint.drawPixmap(1, menuBar()->height() + TOOLBAR_HEIGHT, (*image));
	}
}

/**
 * @brief MainWindow::OnNewImage - Open a NewCanvasDialogue prompting user to enter
 *                                  the dimensions for a new image.
 */
void MainWindow::OnNewImage()
{
    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "New Canvas");
    newCanvas->exec();
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
        delete image;
        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();
        image = new QPixmap(QSize(width,height));
        image->fill(backgroundColor);
        this->repaint();
    }
    // done with the dialog, free it
    delete newCanvas;
}

/**
 * @brief MainWindow::OnLoadImage - Open a QFileDialogue prompting user to
 *                                  browse for a file to open.
 */
void MainWindow::OnLoadImage()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("BMP image (*.bmp)"));
	if (! s.isNull())
	{
		image->load(s);
	}

    this->repaint();
}

/**
 * @brief MainWindow::OnSaveImage - Open a QFileDialogue prompting user to enter
 *                                  a filename and save location.
 */
void MainWindow::OnSaveImage()
{
    if(image->isNull())
        return;

    // appending suffixes doesn't work on native version, so use QT's instead
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setNameFilter("BMP image (*.bmp)");
    fileDialog->setDefaultSuffix("bmp");
    fileDialog->exec();

    // if user hit 'OK' button, save file
    if (fileDialog->result())
    {
        QString s = fileDialog->selectedFiles().first();

        if (! s.isNull())
        {
            image->save(s, "BMP");
        }
    }
    // done with the dialog, free it
    delete fileDialog;
}

/**
 * @brief MainWindow::OnSaveImage - Undo a previous action.
 *
 */
void MainWindow::OnUndo()
{
    //stub
}

/**
 * @brief MainWindow::OnRedo - Redo a previously undone action.
 *
 */
void MainWindow::OnRedo()
{
    //stub
}

/**
 * @brief MainWindow::OnClearAll - Clear the image.
 *
 */
void MainWindow::OnClearAll()
{
    if(image->isNull())
        return;

    image->fill(); // default is white
    this->repaint();
}

/**
 * @brief MainWindow::OnResizeImage - Change the dimensions of the image.
 *
 */
void MainWindow::OnResizeImage()
{
    if(image->isNull())
        return;

    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "Resize Canvas",
                                                       image->width(), image->height());
    newCanvas->exec();
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();
        *image = image->scaled(QSize(width,height), Qt::IgnoreAspectRatio);
        this->repaint();
    }
    // done with the dialog, free it
    delete newCanvas;
}

/**
 * @brief MainWindow::OnPickColor - Open a QColorDialog prompting the user to select a color.
 *
 */
void MainWindow::OnPickColor(int which)
{
    QColorDialog* colorDialog = new QColorDialog(this);
    QColor aColor = colorDialog->getColor(which == FOREGROUND ? foregroundColor
                                                              : backgroundColor,
                                          this,
                                          which == FOREGROUND ? "Foreground Color"
                                                              : "Background Color",
                                          QColorDialog::DontUseNativeDialog);

    // if user hit 'OK' button, change the color
    if (aColor.isValid())
    {
       if(which == FOREGROUND)
           foregroundColor = aColor;
       else
           backgroundColor = aColor;
    }
    // done with the dialog, free it
    delete colorDialog;
}

void MainWindow::mousePressEvent(QMouseEvent * e)
{
    //paintEvent(new QPaintEvent(QRect(0,0,640,480)));
}

/**
 * @brief ToolBar::ToolBar - Wrapper class for QToolBar.
 *                           construct a ToolBar with icons & actions.
 */
ToolBar::ToolBar(QWidget *parent)
    : QToolBar(parent)
{
    // make sure we can't move or hide the toolbar
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setMovable(false);
    createActions();
}

void ToolBar::createActions()
{
    // load button icons from files
    QIcon new_icon("icons/new_icon.png");
    QIcon open_icon("icons/open_icon.png");
    QIcon save_icon("icons/save_icon.png");
    QIcon undo_icon("icons/undo_icon.png");
    QIcon redo_icon("icons/redo_icon.png");
    QIcon clear_icon("icons/clearall_icon.png");
    QIcon resize_icon("icons/resize_icon.png");
    QIcon fcolor_icon("icons/fcolor_icon.png");
    QIcon bcolor_icon("icons/bcolor_icon.png");
    QIcon pen_icon("icons/pen_icon.png");
    QIcon line_icon("icons/line_icon.png");
    QIcon eraser_icon("icons/eraser_icon.png");
    QIcon rect_icon("icons/rect_icon.png");

    MainWindow* mainWindow = (MainWindow*)this->parent();

    // create the actions for each button
    QAction *new_action = new QAction(new_icon, "New File", this);
    new_action->setShortcut(tr("Ctrl+N"));
    connect(new_action, &QAction::triggered, mainWindow, &MainWindow::OnNewImage);

    QAction *open_action = new QAction(open_icon, "Open File", this);
    open_action->setShortcut(tr("Ctrl+O"));
    connect(open_action, &QAction::triggered, mainWindow, &MainWindow::OnLoadImage);

    QAction *save_action = new QAction(save_icon, "Save File", this);
    save_action->setShortcut(tr("Ctrl+S"));
    connect(save_action, &QAction::triggered, mainWindow, &MainWindow::OnSaveImage);

    QAction *clear_action = new QAction(clear_icon, "Clear", this);
    clear_action->setShortcut(tr("Ctrl+C"));
    connect(clear_action, &QAction::triggered, mainWindow, &MainWindow::OnClearAll);

    QAction *resize_action = new QAction(resize_icon, "Resize", this);
    resize_action->setShortcut(tr("Ctrl+R"));
    connect(resize_action, &QAction::triggered, mainWindow, &MainWindow::OnResizeImage);

    QAction *undo_action = new QAction(undo_icon, "Undo", this);
    undo_action->setShortcut(tr("Ctrl+Z"));
    connect(undo_action, &QAction::triggered, mainWindow, &MainWindow::OnUndo);

    QAction *redo_action = new QAction(redo_icon, "Redo", this);
    redo_action->setShortcut(tr("Ctrl+Y"));
    connect(redo_action, &QAction::triggered, mainWindow, &MainWindow::OnRedo);

    // color pickers
    QSignalMapper *signalMapper = new QSignalMapper(this);

    QAction *fcolor_action = new QAction(fcolor_icon, "Foreground Color", this);
    fcolor_action->setShortcut(tr("Ctrl+F"));
    connect(fcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    QAction *bcolor_action = new QAction(bcolor_icon, "Background Color", this);
    bcolor_action->setShortcut(tr("Ctrl+B"));
    connect(bcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));

    signalMapper->setMapping(fcolor_action, FOREGROUND);
    signalMapper->setMapping(bcolor_action, BACKGROUND);

    connect(signalMapper, SIGNAL(mapped(int)), mainWindow, SLOT(OnPickColor(int)));

    // add the actions
    this->addAction(new_action);
    this->addAction(open_action);
    this->addAction(save_action);
    this->addAction(undo_action);
    this->addAction(redo_action);
    this->addAction(clear_action);
    this->addAction(resize_action);
    this->addAction(fcolor_action);
    this->addAction(bcolor_action);
    this->addSeparator();
    this->addAction(pen_icon, "Pen Tool");
    this->addAction(line_icon, "Line Tool");
    this->addAction(eraser_icon, "Eraser");
    this->addAction(rect_icon, "Rectangle Tool");
}

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
                                        double thickness)
    :QDialog(parent)
{

    setWindowTitle(tr("Line Dialog"));
}

/**
 * @brief PenDialog::PenDialog - Dialogue for selecting pen size and cap style.
 *
 */
PenDialog::PenDialog(QWidget* parent, CapStyle capStyle, double size)
    :QDialog(parent)
{

    setWindowTitle(tr("Pen Dialog"));
}

/**
 * @brief EraserDialog::EraserDialog - Dialogue for choosing eraser thickness.
 *
 */
EraserDialog::EraserDialog(QWidget* parent, double thickness)
    :QDialog(parent)
{
    setWindowTitle(tr("Eraser Dialog"));
}

/**
 * @brief RectDialog::RectDialog - Dialogue for selecting what kind of rectangle to draw.
 *
 */
RectDialog::RectDialog(QWidget* parent, LineStyle boundaryStyle, ShapeType shapeType,
                                        FillColor fillColor, BoundaryType boundaryType,
                                        double thickness, double curve)
    :QDialog(parent)
{
    setWindowTitle(tr("Rectangle Dialog"));
}

