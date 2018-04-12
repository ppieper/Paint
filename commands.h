#include <qpixmap.h>
#include <QUndoCommand>

#ifndef COMMANDS_H
#define COMMANDS_H


class DrawCommand : public QUndoCommand
{
public:
    DrawCommand(QPixmap old_image, QPixmap *image, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;
private:
    QPixmap* m_image;
    QPixmap m_old_image;
    QPixmap m_new_image;
};

#endif // COMMANDS_H
