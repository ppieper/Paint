#include <iostream>
using namespace std;

#include <QDesktopWidget>
#include <QMouseEvent>
#include <QFileDialog>
#include <QColorDialog>
#include <QSignalMapper>
#include <QMenuBar>
#include <QMenu>

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

    // create the menu and toolbar
    createMenuAndToolBar();

    // adjust window size, name, & stop context menu
    setWindowTitle(name);
    resize(QDesktopWidget().availableGeometry(this).size()*.6);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setCentralWidget(drawArea);
}

MainWindow::~MainWindow()
{
    imageActions.clear();
    toolActions.clear();
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
 * @brief ToolBar::createMenuAndToolBar() - ensure that everything gets
 *                                          created in the correct order
 *
 */
void MainWindow::createMenuAndToolBar()
{
    // create actions and add them to the menu
    createMenuActions();

    // create the toolbar
    toolbar = new ToolBar(this, imageActions, toolActions);
    addToolBar(toolbar);

    // add a toolbar toggle action to the menu
    createToolBarToggle();
}

/**
 * @brief MainWindow::createMenu - create the actions that appear in the menu
 *
 */
void MainWindow::createMenuActions()
{
    // load button icons from files
    QIcon newIcon(":/icons/newIcon");
    QIcon openIcon(":/icons/openIcon");
    QIcon saveIcon(":/icons/saveIcon");
    QIcon undoIcon(":/icons/undoIcon");
    QIcon redoIcon(":/icons/redoIcon");
    QIcon clearIcon(":/icons/clearAllIcon");
    QIcon resizeIcon(":/icons/resizeIcon");
    QIcon fColorIcon(":/icons/fColorIcon");
    QIcon bColorIcon(":/icons/bColorIcon");
    QIcon penIcon(":/icons/penIcon");
    QIcon lineIcon(":/icons/lineIcon");
    QIcon eraserIcon(":/icons/eraserIcon");
    QIcon rectIcon(":/icons/rectIcon");

    // File
    QMenu* file = new QMenu(tr("File"), this);
    QAction* newAction = file->addAction(newIcon, tr("New image..."),
                                this, SLOT(OnNewImage()), tr("Ctrl+N"));
    QAction* openAction = file->addAction(openIcon, tr("Load image..."),
                                 this, SLOT(OnLoadImage()), tr("Ctrl+O"));
    QAction* saveAction = file->addAction(saveIcon, tr("Save image..."),
                                 this, SLOT(OnSaveImage()), tr("Ctrl+S"));
    file->addAction("Quit", this, SLOT(close()), tr("Ctrl+Q"));

    // Edit
    QMenu* edit = new QMenu(tr("Edit"), this);
    QAction* undoAction = edit->addAction(undoIcon, tr("Undo"),
                                 drawArea, SLOT(OnUndo()), tr("Ctrl+Z"));
    QAction* redoAction = edit->addAction(redoIcon, tr("Redo"),
                                 drawArea, SLOT(OnRedo()), tr("Ctrl+Y"));
    QAction* clearAction = edit->addAction(clearIcon, tr("Clear Canvas"),
                                  drawArea, SLOT(OnClearAll()), tr("Ctrl+C"));
    QAction* resizeAction = edit->addAction(resizeIcon, tr("Resize Image..."),
                                   this, SLOT(OnResizeImage()), tr("Ctrl+R"));

    // color pickers (still under >Edit)
    QSignalMapper *signalMapper = new QSignalMapper(this);

    QAction* fColorAction = new QAction(fColorIcon, tr("Foreground Color..."), this);
    connect(fColorAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));
    fColorAction->setShortcut(tr("Ctrl+F"));

    QAction* bColorAction = new QAction(bColorIcon, tr("Background Color..."), this);
    connect(bColorAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));
    bColorAction->setShortcut(tr("Ctrl+B"));

    signalMapper->setMapping(fColorAction, foreground);
    signalMapper->setMapping(bColorAction, background);

    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(OnPickColor(int)));

    edit->addAction(fColorAction);
    edit->addAction(bColorAction);

    // Tool pickers

    QSignalMapper *signalMapperT = new QSignalMapper(this);

    QAction* penAction = new QAction(penIcon, tr("Pen Tool"), this);
    connect(penAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    penAction->setShortcut(tr("W"));

    QAction* lineAction = new QAction(lineIcon, tr("Line Tool"), this);
    connect(lineAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    lineAction->setShortcut(tr("F"));

    QAction* eraserAction = new QAction(eraserIcon, tr("Eraser"), this);
    connect(eraserAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    eraserAction->setShortcut(tr("E"));

    QAction* rectAction = new QAction(rectIcon, tr("Rectangle Tool"), this);
    connect(rectAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    rectAction->setShortcut(tr("R"));

    signalMapperT->setMapping(penAction, pen);
    signalMapperT->setMapping(lineAction, line);
    signalMapperT->setMapping(eraserAction, eraser);
    signalMapperT->setMapping(rectAction, rect_tool);

    connect(signalMapperT, SIGNAL(mapped(int)), this, SLOT(OnChangeTool(int)));

    // Tool dialogs
    QMenu* tools = new QMenu(tr("Tools"), this);
    tools->addAction(penAction);
    tools->addAction(lineAction);
    tools->addAction(eraserAction);
    tools->addAction(rectAction);
    tools->addAction(tr("Pen Properties..."),
                     this, SLOT(OnPenDialog()));
    tools->addAction(tr("Line Properties..."),
                     this, SLOT(OnLineDialog()));
    tools->addAction(tr("Eraser Properties..."),
                     this, SLOT(OnEraserDialog()));
    tools->addAction(tr("Rectangle Properties..."),
                     this, SLOT(OnRectangleDialog()));

    // store the actions in QLists for convenience
    imageActions.append(newAction);
    imageActions.append(openAction);
    imageActions.append(saveAction);
    imageActions.append(undoAction);
    imageActions.append(redoAction);
    imageActions.append(clearAction);
    imageActions.append(resizeAction);
    imageActions.append(fColorAction);
    imageActions.append(bColorAction);

    toolActions.append(penAction);
    toolActions.append(lineAction);
    toolActions.append(eraserAction);
    toolActions.append(rectAction);

    // populate the menubar with menu items
    menuBar()->addMenu(file);
    menuBar()->addMenu(edit);
    menuBar()->addMenu(tools);
    menuBar()->setNativeMenuBar(false);
}

/**
 * @brief ToolBar::createBarToggle - Create the toggle toolbar action and add
 *                                     it to the menu under "View"
 *
 */
void MainWindow::createToolBarToggle()
{
    // View
    QMenu* view = new QMenu(tr("View"), this);
    QAction *toggleToolbar = toolbar->toggleViewAction();
    toggleToolbar->setText(tr("Show &Toolbar"));
    toggleToolbar->setShortcut(tr("Ctrl+T"));

    view->addAction(toggleToolbar);
    menuBar()->addMenu(view);
}
