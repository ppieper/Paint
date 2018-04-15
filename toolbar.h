#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QList>

#include "constants.h"


class QWidget;

class ToolBar : public QToolBar
{
public:
    ToolBar(QWidget* parent, QList<QAction*> imageActions,
                             QList<QAction*> toolActions);

private:
    void createActions();

    QList<QAction*> imageActions;
    QList<QAction*> toolActions;

    /** Don't allow copying */
    ToolBar(const ToolBar&);
    ToolBar& operator=(const ToolBar&);
};

#endif // TOOLBAR_H
