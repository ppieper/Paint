#include <iostream>
using namespace std;

#include "my_widget.h"

#include <qfiledialog.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <QMenuBar>


MyMainWindow::MyMainWindow(QWidget* parent, const char* name)
:QMainWindow(parent)
{

    QMenu* file = new QMenu("File", this);
    file->addAction("Load image...", this, SLOT(OnLoadImage()));
    menuBar()->addMenu(file);
	image = new QPixmap();
}


MyMainWindow::~MyMainWindow()
{
	delete image;
}

void MyMainWindow::paintEvent(QPaintEvent* e)
{
	QPainter paint(this);
	if (! image->isNull())
	{
		paint.drawPixmap(0, menuBar()->height(), (*image));
	}
}

void MyMainWindow::OnLoadImage()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.bmp)"));
	if (! s.isNull())
	{
		image->load(s);
	}

     this->repaint();
}

void MyMainWindow::mousePressEvent(QMouseEvent * e)
{
	QMessageBox::about( this, "bitmap", "Mouse click event!");
}
