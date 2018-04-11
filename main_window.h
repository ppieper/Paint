#include <QtWidgets>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include "dialog_windows.h"

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

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

	public slots:
    void OnNewImage();
	void OnLoadImage();
    void OnSaveImage();
    void OnUndo();
    void OnRedo();
    void OnClearAll();
    void OnResizeImage();
    void OnPickColor(int);

	private:
    void saveCommand(QPixmap);

    QUndoStack* undoStack;
	QPixmap* image;
    ToolBar* toolbar;
    QColor foregroundColor = Qt::white;
    QColor backgroundColor = Qt::white;
};

#endif
