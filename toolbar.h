#ifndef TOOLBAR_H
#define TOOLBAR_H


class ToolBar : public QToolBar
{
public:
    ToolBar(QWidget* parent);

private:
    void createActions();
};

#endif // TOOLBAR_H
