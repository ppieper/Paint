#include <iostream>
using namespace std;

#include "main_window.h"
#include "commands.h"
#include "draw_area.h"


/**
 * @brief MainWindow::MainWindow - the main window, parent to every other
 *                                 widget.
 */
MainWindow::MainWindow(QWidget* parent, const char* name)
    :QMainWindow(parent)
{
    // create the DrawArea, which will receive the draw mouse events
    drawArea = new DrawArea(this);
    drawArea->setStyleSheet("background-color:transparent");

    // get default tool
    currentTool = drawArea->getCurrentTool();

    // create the toolbar
    toolbar = new ToolBar(this, drawArea);
    addToolBar(toolbar);

    // create the menu (needs toolbar to be created first!)
    createMenu();

    // adjust window size, name, & stop context menu
    setWindowTitle(name);
    resize(QDesktopWidget().availableGeometry(this).size()*.6);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setCentralWidget(drawArea);
}

MainWindow::~MainWindow()
{
}

/**
 * @brief MainWindow::mousePressEvent - On mouse right click, open dialog menu.
 *
 */

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton) {
        openToolDialog();
    }
}

/**
 * @brief MainWindow::OnNewImage - Open a NewCanvasDialogue prompting user to
 *                                 enterthe dimensions for a new image.
 */
void MainWindow::OnNewImage()
{
    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "New Canvas");
    newCanvas->exec();
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
        QSize size = QSize(newCanvas->getWidthValue(),
                           newCanvas->getHeightValue());
        drawArea->createNewImage(size);
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
                                                    ".",
                                                    tr("BMP image (*.bmp)"));
	if (! s.isNull())
	{
        drawArea->loadImage(s);
	}
}

/**
 * @brief MainWindow::OnSaveImage - Open a QFileDialogue prompting user to
 *                                  enter a filename and save location.
 */
void MainWindow::OnSaveImage()
{
    if(drawArea->getImage()->isNull())
        return;

    // use custom dialog settings for appending suffixes
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter("BMP image (*.bmp)");
    fileDialog->setDefaultSuffix("bmp");
    fileDialog->exec();

    // if user hit 'OK' button, save file
    if (fileDialog->result())
    {
        QString s = fileDialog->selectedFiles().first();

        if (! s.isNull())
        {
            drawArea->saveImage(s);
        }
    }
    // done with the dialog, free it
    delete fileDialog;
}

/**
 * @brief MainWindow::OnResizeImage - Change the dimensions of the image.
 *
 */
void MainWindow::OnResizeImage()
{
    QPixmap *image = drawArea->getImage();
    if(image->isNull())
        return;

    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "Resize Image",
                                                       image->width(),
                                                       image->height());
    newCanvas->exec();
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
         drawArea->resizeImage(QSize(newCanvas->getWidthValue(),
                                     newCanvas->getHeightValue()));
    }
    // done with the dialog, free it
    delete newCanvas;
}

/**
 * @brief MainWindow::OnPickColor - Open a QColorDialog prompting the user to
 *                                  select a color.
 *
 */
void MainWindow::OnPickColor(int which)
{
    QColorDialog* colorDialog = new QColorDialog(this);
    QColor foregroundColor = drawArea->getForegroundColor();
    QColor backgroundColor = drawArea->getBackgroundColor();
    QColor color = colorDialog->getColor(which == foreground ? foregroundColor
                                                              : backgroundColor,
                                          this,
                                          which == foreground ? "Foreground Color"
                                                              : "Background Color",
                                          QColorDialog::DontUseNativeDialog);

    // if user hit 'OK' button, change the color
    if (color.isValid())
        drawArea->updateColorConfig(color, which);

    // done with the dialog, free it
    delete colorDialog;
}

/**
 * @brief MainWindow::OnChangeTool - Sets the current tool based on argument.
 *
 */
void MainWindow::OnChangeTool(int newTool)
{
    currentTool = drawArea->setCurrentTool(newTool); // notify observer
}

/**
 * @brief MainWindow::OnPenDialog - Open a PenDialog prompting the user to
 *                                  change pen settings.
 *
 */
void MainWindow::OnPenDialog()
{
    if(!penDialog)
        penDialog = new PenDialog(this, drawArea);

    if(penDialog && penDialog->isVisible())
        return;

    penDialog->show();
}

/**
 * @brief MainWindow::OnLineDialog - Open a LineDialog prompting the user to
 *                                   change line tool settings.
 *
 */
void MainWindow::OnLineDialog()
{
    if(!lineDialog)
        lineDialog = new LineDialog(this, drawArea);

    if(lineDialog && lineDialog->isVisible())
        return;

    lineDialog->show();
}

/**
 * @brief MainWindow::OnEraserDialog - Open a EraserDialog prompting the user
 *                                     to change eraser settings.
 *
 */
void MainWindow::OnEraserDialog()
{
    if (!eraserDialog)
        eraserDialog = new EraserDialog(this, drawArea);

    if(eraserDialog->isVisible())
        return;

    eraserDialog->show();
}

/**
 * @brief MainWindow::OnRectangleDialog - Open a RectDialog prompting the user
 *                                        to change rect tool settings.
 *
 */
void MainWindow::OnRectangleDialog()
{
    if (!rectDialog)
        rectDialog = new RectDialog(this, drawArea);

    if(rectDialog->isVisible())
        return;

    rectDialog->show();
}

/**
 * @brief MainWindow::openToolDialog - call the appropriate dialog function
 *                                     based on the current tool.
 *
 */
void MainWindow::openToolDialog()
{
    switch(currentTool->getType())
    {
        case pen: OnPenDialog();             break;
        case line: OnLineDialog();           break;
        case eraser: OnEraserDialog();       break;
        case rect_tool: OnRectangleDialog(); break;
    }
}

/**
 * @brief MainWindow::createMenu - create and return a menu
 *
 */
void MainWindow::createMenu()
{
    // File
    QMenu* file = new QMenu(tr("&File"), this);
    file->addAction("New image...", this, SLOT(OnNewImage()), tr("Ctrl+N"));
    file->addAction("Load image...", this, SLOT(OnLoadImage()), tr("Ctrl+O"));
    file->addAction("Save image...", this, SLOT(OnSaveImage()), tr("Ctrl+S"));
    file->addAction("Quit", this, SLOT(close()), tr("Ctrl+Q"));

    // Edit
    QMenu* edit = new QMenu(tr("&Edit"), this);
    edit->addAction("Undo", drawArea, SLOT(OnUndo()), tr("Ctrl+Z"));
    edit->addAction("Redo", drawArea, SLOT(OnRedo()), tr("Ctrl+Y"));
    edit->addAction("Clear Canvas", drawArea, SLOT(OnClearAll()), tr("Ctrl+C"));
    edit->addAction("Resize Image...", this, SLOT(OnResizeImage()), tr("Ctrl+R"));

    // color pickers (still under Edit)
    QSignalMapper *signalMapper = new QSignalMapper(this);

    QAction *fcolor_action = new QAction(tr("Foreground Color..."), this);
    connect(fcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    fcolor_action->setShortcut(tr("Ctrl+F"));

    QAction *bcolor_action = new QAction(tr("Background Color..."), this);
    connect(bcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    bcolor_action->setShortcut(tr("Ctrl+B"));

    signalMapper->setMapping(fcolor_action, foreground);
    signalMapper->setMapping(bcolor_action, background);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnPickColor(int)));

    edit->addAction(fcolor_action);
    edit->addAction(bcolor_action);

    // Tools
    QMenu* tools = new QMenu(tr("&Tools"), this);
    tools->addAction("Pen Properties...", this, SLOT(OnPenDialog()));
    tools->addAction("Line Properties...", this, SLOT(OnLineDialog()));
    tools->addAction("Eraser Properties...", this, SLOT(OnEraserDialog()));
    tools->addAction("Rectangle Properties...", this, SLOT(OnRectangleDialog()));

    // View
    QMenu* view = new QMenu(tr("&View"), this);
    QAction *toggleToolbar = toolbar->toggleViewAction();
    toggleToolbar->setText(tr("Show &Toolbar"));
    toggleToolbar->setShortcut(tr("Ctrl+T"));

    view->addAction(toggleToolbar);

    menuBar()->addMenu(file);
    menuBar()->addMenu(edit);
    menuBar()->addMenu(tools);
    menuBar()->addMenu(view);
    menuBar()->setNativeMenuBar(false);
}
