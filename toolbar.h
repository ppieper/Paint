#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "constants.h"


class DrawArea;

class ToolBar : public QToolBar
{
public:
    ToolBar(QWidget* parent, DrawArea* drawArea);

private:
    void createActions();

    DrawArea* drawArea;

    /** Don't allow copying */
    ToolBar(const ToolBar&);
    ToolBar& operator=(const ToolBar&);
};

#endif // TOOLBAR_H
