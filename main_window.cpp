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

    // create the toolbar
    toolbar = new ToolBar(this);
    addToolBar(toolbar);

    // create the menu
    createMenu();

    //create the image
    image = new QPixmap();

    // initialize tools
    penTool = new QPen(QBrush(Qt::black),1,Qt::SolidLine, Qt::SquareCap);
    eraserTool = new QPen(QBrush(Qt::white),1,Qt::SolidLine, Qt::SquareCap);

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
       /* QRect paintedArea = paint.matrix().inverted()
                .mapRect(e->rect())
                .adjusted(-1, -1, 1, 1);*/
        //paint.drawPixmap(paintedArea, *image, paintedArea);
        paint.drawPixmap(1, menuBar()->height() + TOOLBAR_HEIGHT, (*image));
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

        // save a copy of the old image
        old_image = image->copy(QRect());
    }
    else if(e->button() == Qt::RightButton) {
        openToolDialog();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {
        if(image->isNull())
            return;

        QPoint pos = e->pos();
        pos.setY(pos.y() - menuBar()->height() - TOOLBAR_HEIGHT);
        old_pos = pos;
        draw(pos);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        if(image->isNull())
            return;

        // for undo/redo
        saveCommand();
    }
}

void MainWindow::draw(const QPoint& pos)
{
    switch(currentTool)
    {
        case pen:
        {
            QPainter painter(image);
            painter.setPen(*penTool);
            painter.drawLine(old_pos, pos);
            old_pos = pos;
            repaint();
            //int rad = (penTool->width() / 2) + 2;
            //update(QRect(old_pos, pos).normalized().adjusted(-rad, -rad, +rad, +rad));
        } break;
        case line:
        {
        } break;
        case eraser:
        {
            QPainter painter(image);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(*eraserTool);
            painter.drawPoint(pos);
            repaint();
        } break;
        case rect_tool:
            {
            } break;
        default: break;
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
        old_image = image->copy(QRect());

        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();
        *image = QPixmap(QSize(width,height));
        image->fill(backgroundColor);
        this->repaint();

        // for undo/redo
        saveCommand();
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
        old_image = image->copy(QRect());

		image->load(s);
        this->repaint();

        // for undo/redo
        saveCommand();
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
            old_image = image->copy(QRect());

            image->save(s, "BMP");

            // for undo/redo
            saveCommand();
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
    old_image = image->copy(QRect());

    image->fill(); // default is white
    this->repaint();

    // for undo/redo
    saveCommand();
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
                                                       image->width(), image->height());
    newCanvas->exec();
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
        // save a copy of the old image
        old_image = image->copy(QRect());

        // get new dimension from dialog
        int width = newCanvas->getWidthValue();
        int height = newCanvas->getHeightValue();

        // no change
        if(image->size() == QSize(width, height))
            return;

        // re-scale the image
        *image = image->scaled(QSize(width,height), Qt::IgnoreAspectRatio);
        this->repaint();

        // for undo/redo
        saveCommand();
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
    if(!penDialog)
        penDialog = new PenDialog(this);

    if(penDialog && penDialog->isVisible())
        return;

    penDialog->show();
}

/**
 * @brief MainWindow::OnLineDialog - Open a LineDialog prompting the user to change line tool settings.
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
 * @brief MainWindow::OnEraserDialog - Open a EraserDialog prompting the user to change eraser settings.
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
 * @brief MainWindow::OnRectangleDialog - Open a RectDialog prompting the user to change rect tool settings.
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
 * @brief MainWindow::SaveCommand - Put together a DrawCommand
 *                                  and save it on the undo/redo stack.
 *
 */
void MainWindow::saveCommand()
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
