#include "commands.h"
#include "qrect.h"


/**
 * @brief DrawCommand::DrawCommand - A command that keeps a copy of the image
 *                                   before and after something is drawn
 */
DrawCommand::DrawCommand(const QPixmap &oldImage, QPixmap *image,
                               QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->image = image;
    this->oldImage = oldImage;
    newImage = image->copy(QRect());
}

/**
 * @brief DrawCommand::undo - Undo a draw command, restoring the old image
 */
void DrawCommand::undo()
{
    *image = oldImage.copy(QRect());
}

/**
 * @brief DrawCommand::redo - 'Undo' an undo, restoring the new image
 */
void DrawCommand::redo()
{
    *image = newImage.copy(QRect());
}
