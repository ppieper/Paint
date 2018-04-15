#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QList>
#include <QAction>
#include <QWidget>

#include "dialog_windows.h"
#include "draw_area.h"
#include "toolbar.h"


class Tool;

class MainWindow: public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(QWidget* parent = 0, const char* name = 0);
    ~MainWindow();

    /** mouse event handler */
    void virtual mousePressEvent (QMouseEvent *) override;

public slots:
    /** toolbar actions */
    void OnNewImage();
	void OnLoadImage();
    void OnSaveImage();
    void OnResizeImage();
    void OnPickColor(int);
    void OnChangeTool(int);
    /** tool dialogs */
    void OnPenDialog();
    void OnLineDialog();
    void OnEraserDialog();
    void OnRectangleDialog();

private:
    void createMenuActions();
    void createToolBarToggle();
    void createMenuAndToolBar();

    /** tool dialog dispatcher */
    void openToolDialog();

    /** the drawArea */
    DrawArea* drawArea;

    /** actions */
    QList<QAction*> imageActions;
    QList<QAction*> toolActions;

    /** main toolbar */
    ToolBar* toolbar;

    /** current tool */
    Tool* currentTool;

    /** dialog pointers - init to 0 */
    PenDialog* penDialog = 0;
    LineDialog* lineDialog = 0;
    EraserDialog* eraserDialog = 0;
    RectDialog* rectDialog = 0;

    /** Don't allow copying */
    MainWindow(const MainWindow&);
    MainWindow& operator=(const MainWindow&);
};

#endif // MAIN_WINDOW_H
