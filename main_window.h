#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include "dialog_windows.h"
#include "toolbar.h"
#include "draw_area.h"


class MainWindow: public QMainWindow {

	Q_OBJECT

	public:

    MainWindow(QWidget* parent = 0, const char* name = 0);
    ~MainWindow();

    /** mouse event handler */
    void mousePressEvent (QMouseEvent *);

    /** save a command to the undo stack */
    void saveDrawCommand(QPixmap);

	public slots:

    /** toolbar actions */
    void OnNewImage();
	void OnLoadImage();
    void OnSaveImage();
    void OnUndo();
    void OnRedo();
    void OnClearAll();
    void OnResizeImage();
    void OnPickColor(int);
    void OnChangeTool(int);
    /** tool dialogs */
    void OnPenDialog();
    void OnLineDialog();
    void OnEraserDialog();
    void OnRectangleDialog();
    /** pen tool */
    void OnPenCapConfig(int);
    void OnPenSizeConfig(int);
    /** eraser tool */
    void OnEraserConfig(int);
    /** line tool */
    void OnLineStyleConfig(int);
    void OnLineCapConfig(int);
    void OnDrawTypeConfig(int);
    void OnLineThicknessConfig(int);

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
