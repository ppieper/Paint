#include "dialog_windows.h"

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H


enum ToolType {pen, line, eraser, rect_tool};

class ToolBar : public QToolBar
{
    Q_OBJECT

public:
    ToolBar(QWidget* parent);

public slots:

private:
    void createActions();
};

class MainWindow: public QMainWindow {
	// All classes that contain signals or slots
	// must mention Q_OBJECT in their declaration.
	Q_OBJECT

	public:
    MainWindow(QWidget* parent = 0, const char* name = 0);
    ~MainWindow();

	/** mouse event handler */
	void mousePressEvent (QMouseEvent *);

	/** paint handler */
	void paintEvent(QPaintEvent *);

    /** tool dialog dispatcher */
    void openToolDialog();

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

	private:
    void createMenu();
    void saveCommand(QPixmap);

    QUndoStack* undoStack;
    QPixmap* image;
    ToolBar* toolbar;
    QColor foregroundColor = Qt::white;
    QColor backgroundColor = Qt::white;
    ToolType currentTool;

    /** dialog pointers - init to 0 */
    PenDialog *penDialog = 0;
    LineDialog *lineDialog = 0;
    EraserDialog *eraserDialog = 0;
    RectDialog *rectDialog = 0;
};

#endif
