#include <qmessagebox.h>

#ifndef DIALOG_WINDOWS_H
#define DIALOG_WINDOWS_H

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

class QSpinBox;
class QGroupBox;
class QLabel;

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    CanvasSizeDialog(QWidget* parent, const char* name = 0,
                     int width = DEFAULT_WIDTH,
                     int height = DEFAULT_HEIGHT);

public slots:
    int getWidthValue() const;
    int getHeightValue() const;

private:
    void createSpinBoxes(int,int);

    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QGroupBox *spinBoxesGroup;
};

#endif // DIALOGS_H
