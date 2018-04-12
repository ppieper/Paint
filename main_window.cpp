#include <iostream>
using namespace std;

#include "main_window.h"
#include "commands.h"

const int TOOLBAR_HEIGHT = 39;

/**
 * @brief MainWindow::MainWindow - the main window, parent to every other widget.
 */
MainWindow::MainWindow(QWidget* parent, const char* name)
    :QMainWindow(parent)
{
    // initialize the undo stack
    undoStack = new QUndoStack(this);

    // default tool
    currentTool = pen;

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

    // adjust window size, name, & stop context menu
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
        // save a copy of the old image
        QPixmap old_image = image->copy(QRect());

        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();
        *image = QPixmap(QSize(width,height));
        image->fill(backgroundColor);
        this->repaint();

        // for undo/redo
        saveCommand(old_image);
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
        // save a copy of the old image
        QPixmap old_image = image->copy(QRect());

		image->load(s);
        this->repaint();

        // for undo/redo
        saveCommand(old_image);
	}
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
            // save a copy of the old image
            QPixmap old_image = image->copy(QRect());

            image->save(s, "BMP");

            // for undo/redo
            saveCommand(old_image);
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
    if(!undoStack->canUndo())
        return;

    undoStack->undo();
    this->repaint();
}

/**
 * @brief MainWindow::OnRedo - Redo a previously undone action.
 *
 */
void MainWindow::OnRedo()
{
    if(!undoStack->canRedo())
        return;

    undoStack->redo();
    this->repaint();
}

/**
 * @brief MainWindow::OnClearAll - Clear the image.
 *
 */
void MainWindow::OnClearAll()
{
    if(image->isNull())
        return;

    // save a copy of the old image
    QPixmap old_image = image->copy(QRect());

    image->fill(); // default is white
    this->repaint();

    // for undo/redo
    saveCommand(old_image);
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
        // save a copy of the old image
        QPixmap old_image = image->copy(QRect());

        // re-scale the image
        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();
        *image = image->scaled(QSize(width,height), Qt::IgnoreAspectRatio);
        this->repaint();

        // for undo/redo
        saveCommand(old_image);
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
    QColor aColor = colorDialog->getColor(which == foreground ? foregroundColor
                                                              : backgroundColor,
                                          this,
                                          which == foreground ? "Foreground Color"
                                                              : "Background Color",
                                          QColorDialog::DontUseNativeDialog);

    // if user hit 'OK' button, change the color
    if (aColor.isValid())
    {
       if(which == foreground)
           foregroundColor = aColor;
       else
           backgroundColor = aColor;
    }
    // done with the dialog, free it
    delete colorDialog;
}

/**
 * @brief MainWindow::OnChangeTool - Sets the current tool based on argument.
 *
 */
void MainWindow::OnChangeTool(int newTool)
{
    currentTool = (ToolType) newTool;
}

/**
 * @brief MainWindow::OnPenDialog - Open a PenDialog prompting the user to change pen settings.
 *
 */
void MainWindow::OnPenDialog()
{
    PenDialog *penDialog = new PenDialog(this);
    penDialog->exec();

    delete penDialog;
}

/**
 * @brief MainWindow::OnLineDialog - Open a LineDialog prompting the user to change line tool settings.
 *
 */
void MainWindow::OnLineDialog()
{
    LineDialog *lineDialog = new LineDialog(this);
    lineDialog->exec();

    delete lineDialog;
}

/**
 * @brief MainWindow::OnEraserDialog - Open a EraserDialog prompting the user to change eraser settings.
 *
 */
void MainWindow::OnEraserDialog()
{
    EraserDialog *eraserDialog = new EraserDialog(this);
    eraserDialog->exec();

    delete eraserDialog;
}

/**
 * @brief MainWindow::OnRectangleDialog - Open a RectDialog prompting the user to change rect tool settings.
 *
 */
void MainWindow::OnRectangleDialog()
{
    RectDialog *rectDialog = new RectDialog(this);
    rectDialog->exec();

    delete rectDialog;
}

/**
 * @brief MainWindow::openToolDialog - call the appropriate dialog function based on the current tool.
 *
 */
void MainWindow::openToolDialog()
{
    switch(currentTool)
    {
        case pen: OnPenDialog();             break;
        case line: OnLineDialog();           break;
        case eraser: OnEraserDialog();       break;
        case rect_tool: OnRectangleDialog(); break;
    }
}

/**
 * @brief MainWindow::mousePressEvent - On mouse click, draw or open dialog menu.
 *
 */
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        if(image->isNull())
            return;
        // use tools
    }
    else if(e->button() == Qt::RightButton) {
        openToolDialog();
    }
}

/**
 * @brief MainWindow::SaveCommand - Put together a DrawCommand
 *                                  and save it on the undo/redo stack.
 *
 */
void MainWindow::saveCommand(QPixmap old_image)
{
    // put the old and new image on the stack for undo/redo
    QUndoCommand *drawCommand = new DrawCommand(old_image, image);
    undoStack->push(drawCommand);
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

/**
 * @brief ToolBar::createActions - Create the toolbar actions and
 *                                 give each one an icon.
 */
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
    QSignalMapper *signalMapperC = new QSignalMapper(this);

    QAction *fcolor_action = new QAction(fcolor_icon, "Foreground Color", this);
    fcolor_action->setShortcut(tr("Ctrl+F"));
    connect(fcolor_action, SIGNAL(triggered()), signalMapperC, SLOT(map()));
    QAction *bcolor_action = new QAction(bcolor_icon, "Background Color", this);
    bcolor_action->setShortcut(tr("Ctrl+B"));
    connect(bcolor_action, SIGNAL(triggered()), signalMapperC, SLOT(map()));

    signalMapperC->setMapping(fcolor_action, foreground);
    signalMapperC->setMapping(bcolor_action, background);

    connect(signalMapperC, SIGNAL(mapped(int)), mainWindow, SLOT(OnPickColor(int)));

    // tool pickers
    QSignalMapper *signalMapperT = new QSignalMapper(this);

    QAction *pen_action = new QAction(pen_icon, "Pen Tool", this);
    connect(pen_action, SIGNAL(triggered()), signalMapperT, SLOT(map()));

    QAction *line_action = new QAction(line_icon, "Line Tool", this);
    connect(line_action, SIGNAL(triggered()), signalMapperT, SLOT(map()));

    QAction *eraser_action = new QAction(eraser_icon, "Eraser", this);
    connect(eraser_action, SIGNAL(triggered()), signalMapperT, SLOT(map()));

    QAction *rect_action = new QAction(rect_icon, "Rectangle Tool", this);
    connect(rect_action, SIGNAL(triggered()), signalMapperT, SLOT(map()));

    signalMapperT->setMapping(pen_action, pen);
    signalMapperT->setMapping(line_action, line);
    signalMapperT->setMapping(eraser_action, eraser);
    signalMapperT->setMapping(rect_action, rect_tool);

    connect(signalMapperT, SIGNAL(mapped(int)), mainWindow, SLOT(OnChangeTool(int)));

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
    this->addAction(pen_action);
    this->addAction(line_action);
    this->addAction(eraser_action);
    this->addAction(rect_action);
}

