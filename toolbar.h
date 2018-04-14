#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "constants.h"


class ToolBar : public QToolBar
{
public:
    ToolBar(QWidget* parent);

private:
    void createActions();

    /** Don't allow copying */
    ToolBar(const ToolBar&);
    ToolBar& operator=(const ToolBar&);
};

#endif // TOOLBAR_H
