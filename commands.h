#ifndef COMMANDS_H
#define COMMANDS_H

#include <QPixmap>
#include <QUndoCommand>


class DrawCommand : public QUndoCommand
{
public:
    DrawCommand(const QPixmap &oldImage, QPixmap *image, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;
private:
    QPixmap* image;
    QPixmap oldImage;
    QPixmap newImage;
};

#endif // COMMANDS_H
