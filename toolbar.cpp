#include "main_window.h"
#include "draw_area.h"


/**
 * @brief ToolBar::ToolBar - Wrapper class for QToolBar.
 *                           construct a ToolBar with icons & actions.
 */
ToolBar::ToolBar(QWidget *parent, DrawArea* drawArea)
    : QToolBar(parent)
{
    this->drawArea = drawArea;

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
    QIcon new_icon(":/icons/newIcon");
    QIcon open_icon(":/icons/openIcon");
    QIcon save_icon(":/icons/saveIcon");
    QIcon undo_icon(":/icons/undoIcon");
    QIcon redo_icon(":/icons/redoIcon");
    QIcon clear_icon(":/icons/clearAllIcon");
    QIcon resize_icon(":/icons/resizeIcon");
    QIcon fcolor_icon(":/icons/fColorIcon");
    QIcon bcolor_icon(":/icons/bColorIcon");
    QIcon pen_icon(":/icons/penIcon");
    QIcon line_icon(":/icons/lineIcon");
    QIcon eraser_icon(":/icons/eraserIcon");
    QIcon rect_icon(":/icons/rectIcon");

    MainWindow* mainWindow = (MainWindow*)this->parent();

    // create the actions for each button
    QAction *new_action = new QAction(new_icon, "New File", this);
    connect(new_action, &QAction::triggered, mainWindow, &MainWindow::OnNewImage);

    QAction *open_action = new QAction(open_icon, "Open File", this);
    connect(open_action, &QAction::triggered, mainWindow, &MainWindow::OnLoadImage);

    QAction *save_action = new QAction(save_icon, "Save File", this);
    connect(save_action, &QAction::triggered, mainWindow, &MainWindow::OnSaveImage);

    QAction *clear_action = new QAction(clear_icon, "Clear", this);
    connect(clear_action, &QAction::triggered, drawArea, &DrawArea::OnClearAll);

    QAction *resize_action = new QAction(resize_icon, "Resize", this);
    connect(resize_action, &QAction::triggered, mainWindow, &MainWindow::OnResizeImage);

    QAction *undo_action = new QAction(undo_icon, "Undo", this);
    connect(undo_action, &QAction::triggered, drawArea, &DrawArea::OnUndo);

    QAction *redo_action = new QAction(redo_icon, "Redo", this);
    connect(redo_action, &QAction::triggered, drawArea, &DrawArea::OnRedo);

    // color pickers
    QSignalMapper *signalMapperC = new QSignalMapper(this);

    QAction *fcolor_action = new QAction(fcolor_icon, "Foreground Color", this);
    connect(fcolor_action, SIGNAL(triggered()), signalMapperC, SLOT(map()));

    QAction *bcolor_action = new QAction(bcolor_icon, "Background Color", this);
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

