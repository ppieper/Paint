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
    // initialize the undo stack
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(100);

    // create the toolbar
    toolbar = new ToolBar(this);
    addToolBar(toolbar);

    // create the menu (needs toolbar to be created first!)
    createMenu();

    //create the image
    image = new QPixmap();

    // initialize colors
    foregroundColor = Qt::black;
    backgroundColor = Qt::white;

    // initialize tools
    penTool = new PenTool(QBrush(Qt::black),1,Qt::SolidLine,
                          Qt::RoundCap,image);
    lineTool = new LineTool(QBrush(Qt::black),1,Qt::SolidLine,
                            Qt::RoundCap,image);
    eraserTool = new EraserTool(QBrush(Qt::white),10,Qt::SolidLine,
                                Qt::RoundCap,image);
    rectTool = new RectTool(QBrush(Qt::black),1,Qt::SolidLine,Qt::
                            RoundCap,image);

    // set default tool
    currentTool = penTool;

    // create the DrawArea, which will receive the draw mouse events
    drawArea = new DrawArea(this, image, penTool, lineTool,
                                  eraserTool, rectTool, currentTool);
    drawArea->setStyleSheet("background-color:rgba(0,0,0,0)");

    // adjust window size, name, & stop context menu
    setWindowTitle(name);
    resize(QDesktopWidget().availableGeometry(this).size()*.6);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setCentralWidget(drawArea);
}

MainWindow::~MainWindow()
{
    delete image;
    delete penTool;
    delete lineTool;
    delete eraserTool;
    delete rectTool;
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
        // save a copy of the old image
        QPixmap old_image = image->copy();

        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();
        *image = QPixmap(QSize(width,height));
        image->fill(backgroundColor);
        drawArea->update();

        // for undo/redo
        saveDrawCommand(old_image);
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
        // save a copy of the old image
        QPixmap old_image = image->copy();

		image->load(s);
        drawArea->update();

        // for undo/redo
        saveDrawCommand(old_image);
	}
}

/**
 * @brief MainWindow::OnSaveImage - Open a QFileDialogue prompting user to
 *                                  enter a filename and save location.
 */
void MainWindow::OnSaveImage()
{
    if(image->isNull())
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
            // save a copy of the old image
            QPixmap old_image = image->copy();

            image->save(s, "BMP");

            // for undo/redo
            saveDrawCommand(old_image);
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
    drawArea->update();
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
    drawArea->update();
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
    QPixmap old_image = image->copy();

    image->fill(); // default is white
    drawArea->update(image->rect());

    // for undo/redo
    saveDrawCommand(old_image);
}

/**
 * @brief MainWindow::OnResizeImage - Change the dimensions of the image.
 *
 */
void MainWindow::OnResizeImage()
{
    if(image->isNull())
        return;

    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "Resize Image",
                                                       image->width(),
                                                       image->height());
    newCanvas->exec();
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
        // save a copy of the old image
        QPixmap old_image = image->copy();

        // get new dimension from dialog
        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();

        // if no change, do nothing
        if(image->size() == QSize(width, height))
        {
            delete newCanvas;
            return;
        }

        // else re-scale the image
        *image = image->scaled(QSize(width,height), Qt::IgnoreAspectRatio);
        drawArea->update();

        // for undo/redo
        saveDrawCommand(old_image);
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
       {
            foregroundColor = aColor;
            penTool->setColor(foregroundColor);
            lineTool->setColor(foregroundColor);
            rectTool->setColor(foregroundColor);
            if(rectTool->getFillMode() == foreground)
                rectTool->setFillColor(foregroundColor);
       }
       else
       {
           backgroundColor = aColor;
           eraserTool->setColor(backgroundColor);
           if(rectTool->getFillMode() == background)
               rectTool->setFillColor(backgroundColor);
       }
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
    switch(newTool)
    {
        case pen: currentTool = penTool;        break;
        case line: currentTool = lineTool;      break;
        case eraser: currentTool = eraserTool;  break;
        case rect_tool: currentTool = rectTool; break;
        default: break;
    }
    drawArea->setCurrentTool(currentTool); // notify observer
}

/**
 * @brief MainWindow::OnPenDialog - Open a PenDialog prompting the user to
 *                                  change pen settings.
 *
 */
void MainWindow::OnPenDialog()
{
    if(!penDialog)
        penDialog = new PenDialog(this);

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
        lineDialog = new LineDialog(this);

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
        eraserDialog = new EraserDialog(this);

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
        rectDialog = new RectDialog(this);

    if(rectDialog->isVisible())
        return;

    rectDialog->show();
}

void MainWindow::OnPenCapConfig(int capStyle)
{
    switch (capStyle)
    {
        case flat: penTool->setCapStyle(Qt::FlatCap);       break;
        case square: penTool->setCapStyle(Qt::SquareCap);   break;
        case round_cap: penTool->setCapStyle(Qt::RoundCap); break;
        default:                                            break;
    }
}

void MainWindow::OnPenSizeConfig(int value)
{
    penTool->setWidth(value);
}

void MainWindow::OnEraserConfig(int value)
{
    eraserTool->setWidth(value);
}

void MainWindow::OnLineStyleConfig(int lineStyle)
{
    switch (lineStyle)
    {
        case solid: lineTool->setStyle(Qt::SolidLine);                break;
        case dashed: lineTool->setStyle(Qt::DashLine);                break;
        case dotted: lineTool->setStyle(Qt::DotLine);                 break;
        case dash_dotted: lineTool->setStyle(Qt::DashDotLine);        break;
        case dash_dot_dotted: lineTool->setStyle(Qt::DashDotDotLine); break;
        default:                                                      break;
    }
}

void MainWindow::OnLineCapConfig(int capStyle)
{
    switch (capStyle)
    {
        case flat: lineTool->setCapStyle(Qt::FlatCap);       break;
        case square: lineTool->setCapStyle(Qt::SquareCap);   break;
        case round_cap: lineTool->setCapStyle(Qt::RoundCap); break;
        default:                                             break;
    }
}

void MainWindow::OnDrawTypeConfig(int drawType)
{
    switch (drawType)
    {
        case single: drawArea->setLineMode(single); break;
        case poly:   drawArea->setLineMode(poly);   break;
        default:     break;
    }
}

void MainWindow::OnLineThicknessConfig(int value)
{
    lineTool->setWidth(value);
}

void MainWindow::OnRectBStyleConfig(int boundaryStyle)
{
    switch (boundaryStyle)
    {
        case solid: rectTool->setStyle(Qt::SolidLine);                break;
        case dashed: rectTool->setStyle(Qt::DashLine);                break;
        case dotted: rectTool->setStyle(Qt::DotLine);                 break;
        case dash_dotted: rectTool->setStyle(Qt::DashDotLine);        break;
        case dash_dot_dotted: rectTool->setStyle(Qt::DashDotDotLine); break;
        default:                                                      break;
    }
}

void MainWindow::OnRectShapeTypeConfig(int shape)
{
    switch (shape)
    {
        case rectangle: rectTool->setShapeType(rectangle);                 break;
        case rounded_rectangle: rectTool->setShapeType(rounded_rectangle); break;
        case ellipse: rectTool->setShapeType(ellipse);                     break;
        default:                                                           break;
    }
}

void MainWindow::OnRectFillConfig(int fillType)
{
    switch (fillType)
    {
        case foreground: rectTool->setFillMode(foreground);
                         rectTool->setFillColor(foregroundColor);      break;
        case background: rectTool->setFillMode(background);
                         rectTool->setFillColor(backgroundColor);      break;
        case no_fill: rectTool->setFillMode(no_fill);
                      rectTool->setFillColor(QColor(Qt::transparent)); break;
        default:                                                       break;
    }
}

void MainWindow::OnRectBTypeConfig(int boundaryType)
{
    switch (boundaryType)
    {
        case miter_join: rectTool->setJoinStyle(Qt::MiterJoin);  break;
        case bevel_join: rectTool->setJoinStyle(Qt::BevelJoin);  break;
        case round_join: rectTool->setJoinStyle(Qt::RoundJoin);  break;
        default:                                                 break;
    }
}

void MainWindow::OnRectLineConfig(int value)
{
    rectTool->setWidth(value);
}

void MainWindow::OnRectCurveConfig(int value)
{
    rectTool->setCurve(value);
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
 * @brief MainWindow::SaveDrawCommand - Put together a DrawCommand
 *                                  and save it on the undo/redo stack.
 *
 */
void MainWindow::saveDrawCommand(QPixmap old_image)
{
    // put the old and new image on the stack for undo/redo
    QUndoCommand *drawCommand = new DrawCommand(old_image, image);
    undoStack->push(drawCommand);
}

/**
 * @brief MainWindow::createMenu - create and return a menu
 *
 */
void MainWindow::createMenu()
{
    // File
    QMenu* file = new QMenu("File", this);
    file->addAction("New image...", this, SLOT(OnNewImage()), tr("Ctrl+N"));
    file->addAction("Load image...", this, SLOT(OnLoadImage()), tr("Ctrl+O"));
    file->addAction("Save image...", this, SLOT(OnSaveImage()), tr("Ctrl+S"));
    file->addAction("Quit", this, SLOT(close()), tr("Ctrl+Q"));

    // Edit
    QMenu* edit = new QMenu("Edit", this);
    edit->addAction("Undo", this, SLOT(OnUndo()), tr("Ctrl+Z"));
    edit->addAction("Redo", this, SLOT(OnRedo()), tr("Ctrl+Y"));
    edit->addAction("Clear Canvas", this, SLOT(OnClearAll()), tr("Ctrl+C"));
    edit->addAction("Resize Image...", this, SLOT(OnResizeImage()), tr("Ctrl+R"));

    // color pickers (still under Edit)
    QSignalMapper *signalMapper = new QSignalMapper(this);

    QAction *fcolor_action = new QAction("Foreground Color...", this);
    connect(fcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    fcolor_action->setShortcut(tr("Ctrl+F"));

    QAction *bcolor_action = new QAction("Background Color...", this);
    connect(bcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    bcolor_action->setShortcut(tr("Ctrl+B"));

    signalMapper->setMapping(fcolor_action, foreground);
    signalMapper->setMapping(bcolor_action, background);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnPickColor(int)));

    edit->addAction(fcolor_action);
    edit->addAction(bcolor_action);

    // Tools
    QMenu* tools = new QMenu("Tools", this);
    tools->addAction("Pen Properties...", this, SLOT(OnPenDialog()));
    tools->addAction("Line Properties...", this, SLOT(OnLineDialog()));
    tools->addAction("Eraser Properties...", this, SLOT(OnEraserDialog()));
    tools->addAction("Rectangle Properties...", this, SLOT(OnRectangleDialog()));

    // View
    QMenu* view = new QMenu("View", this);
    QAction *toggleToolbar = toolbar->toggleViewAction();
    toggleToolbar->setText("Show Toolbar");
    toggleToolbar->setShortcut(tr("Ctrl+T"));

    view->addAction(toggleToolbar);

    menuBar()->addMenu(file);
    menuBar()->addMenu(edit);
    menuBar()->addMenu(tools);
    menuBar()->addMenu(view);
    menuBar()->setNativeMenuBar(false);
}
