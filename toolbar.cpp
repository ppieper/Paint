#include "main_window.h"
#include "draw_area.h"


/**
 * @brief ToolBar::ToolBar - Wrapper class for QToolBar.
 *                           construct a ToolBar with icons & actions.
 */
ToolBar::ToolBar(QWidget *parent, const QList<QAction*> &imageActions,
                                  const QList<QAction*> &toolActions)
    : QToolBar(parent)
{
    this->imageActions = imageActions;
    this->toolActions = toolActions;

    // make sure we can't move or hide the toolbar
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setMovable(false);
    createActions();
}

/**
 * @brief ToolBar::createActions - Populate the toolbar with actions
 */
void ToolBar::createActions()
{
    addActions(imageActions);
    addSeparator();
    addActions(toolActions);
}

