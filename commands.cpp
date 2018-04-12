#include "commands.h"
#include <QtWidgets>

/**
 * @brief DrawCommand::DrawCommand - A command that keeps a copy of the image
 *                                   before and after something is drawn.
 */
DrawCommand::DrawCommand(QPixmap old_image, QPixmap *image, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_image = image;
    m_old_image = old_image;
    m_new_image = image->copy(QRect());
}

/**
 * @brief DrawCommand::undo - Undo a draw command, restoring the old image.
 */
void DrawCommand::undo()
{
    *m_image = m_old_image.copy(QRect());
}

/**
 * @brief TDrawCommand::redo - 'Undo' an undo, restoring the new image.
 */
void DrawCommand::redo()
{
    *m_image = m_new_image.copy(QRect());
}
