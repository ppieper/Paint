#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include "dialog_windows.h"
#include "draw_area.h"


class ToolBar : public QToolBar
{
public:
    ToolBar(QWidget* parent);

private:
    void createActions();
};

class MainWindow: public QMainWindow {

	Q_OBJECT

	public:

    MainWindow(QWidget* parent = 0, const char* name = 0);
    ~MainWindow();

    /** mouse event handler */
    void mousePressEvent (QMouseEvent *);

    void saveDrawCommand(QPixmap);

	public slots:

    void OnNewImage();
	void OnLoadImage();
    void OnSaveImage();
    void OnUndo();
    void OnRedo();
    void OnClearAll();
    void OnResizeImage();
    void OnChangeTool(int);
    void OnPenDialog();
    void OnLineDialog();
    void OnEraserDialog();
    void OnRectangleDialog();
    void OnPickColor(int);
    void OnPenCapConfig(int);
    void OnPenSizeConfig(int);
    void OnEraserConfig(int);

	private:

    /** tool dialog dispatcher */
    void openToolDialog();

    void createMenu();

    /** undo stack */
    QUndoStack* undoStack;

    /** the paintArea */
    DrawArea* drawArea;

    /** Main toolbar */
    ToolBar* toolbar;

    /** the image */
    QPixmap* image;

    /** background/foreground color */
    QColor foregroundColor = Qt::white;
    QColor backgroundColor = Qt::white;

    /** tools */
    ToolType currentTool;
    QPen* penTool;
    QPen* lineTool;
    QPen* eraserTool;
    QPen* rectTool;

    /** dialog pointers - init to 0 */
    PenDialog* penDialog = 0;
    LineDialog* lineDialog = 0;
    EraserDialog* eraserDialog = 0;
    RectDialog* rectDialog = 0;
};

#endif
