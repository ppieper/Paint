#include <qapplication.h>
#include "main_window.h"


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget* w = new MainWindow(0, "Paint");
    w->show();
    int exitCode = a.exec();
    delete w;
    return exitCode;
}
