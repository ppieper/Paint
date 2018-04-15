#include <QPainter>
#include <QPaintEvent>

#include "commands.h"
#include "draw_area.h"
#include "main_window.h"


/**
 * @brief DrawArea::DrawArea - constructor for our Draw Area.
 *                             Pointers to the MainWindow's
 *                             tools and image are mandatory
 *
 */
DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent)
{
    // initialize the undo stack
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(UNDO_LIMIT);

    // initialize image
    image = new QPixmap();

    //create the pen, line, eraser, & rect tools
    createTools();

    // initialize colors
    foregroundColor = Qt::black;
    backgroundColor = Qt::white;

    // initialize state variables
    drawing = false;
    drawingPoly = false;
    currentLineMode = single;

    // small optimizations
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_StaticContents);
}

DrawArea::~DrawArea()
{
    delete image;
    delete penTool;
    delete lineTool;
    delete eraserTool;
    delete rectTool;
}


/**
 * @brief DrawArea::paintEvent - redraw part of the image based
 *                               on what was modified
 *
 */
void DrawArea::paintEvent(QPaintEvent *e)

{
    QPainter painter(this);
    QRect modifiedArea = e->rect(); // only need to redraw a small area
    painter.drawPixmap(modifiedArea, *image, modifiedArea);
}

/**
 * @brief DrawArea::mousePressEvent - left-click initiates a draw
 *
 *                                  - right-click opens a dialog
 *                                    menu for the current tool
 *
 */
void DrawArea::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton)
    {
        // open the dialog menu
        static_cast<MainWindow*>(parent())->mousePressEvent(e);
    }
    else if (e->button() == Qt::LeftButton)
    {
        if(image->isNull())
            return;

        drawing = true;

        if(!drawingPoly)
            currentTool->setStartPoint(e->pos());

        // save a copy of the old image
        oldImage = image->copy(QRect());
    }
}

/**
 * @brief DrawArea::mouseMoveEvent - draw
 *
 */
void DrawArea::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && drawing)
    {
        if(image->isNull())
            return;

        ToolType type = currentTool->getType();
        if(type == line || type == rect_tool)
        {
            *image = oldImage;
            if(type == line && currentLineMode == poly)
            {
                drawingPoly = true;
            }
        }
        currentTool->drawTo(e->pos(), this, image);
    }
}

/**
 * @brief DrawArea::mouseReleaseEvent - finish drawing
 *
 */
void DrawArea::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && drawing)
    {
        drawing = false;

        if(image->isNull())
            return;

        if(drawingPoly)
        {
            currentTool->setStartPoint(e->pos());
            //return;
        }
        if(currentTool->getType() == pen)
            currentTool->drawTo(e->pos(), this, image);

        // for undo/redo - make sure there was a change
        // (in case drawing began off-image)
        if(oldImage.toImage() != image->toImage())
            saveDrawCommand(oldImage);
    }
}

/**
 * @brief DrawArea::mouseDoubleClickEvent - cancel poly mode
 *
 */
void DrawArea::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if(drawingPoly)
            drawingPoly = false;
    }
}

/**
 * @brief DrawArea::OnSaveImage - Undo a previous action
 *
 */
void DrawArea::OnUndo()
{
    if(!undoStack->canUndo())
        return;

    undoStack->undo();
    update();
}

/**
 * @brief DrawArea::OnRedo - Redo a previously undone action
 *
 */
void DrawArea::OnRedo()
{
    if(!undoStack->canRedo())
        return;

    undoStack->redo();
    update();
}

/**
 * @brief DrawArea::OnClearAll - Clear the image
 *
 */
void DrawArea::OnClearAll()
{
    if(image->isNull())
        return;

    clearImage();
}

/**
 * @brief DrawArea::OnPenCapConfig - Update cap style for pen tool
 *
 */
void DrawArea::OnPenCapConfig(int capStyle)
{
    switch (capStyle)
    {
        case flat: penTool->setCapStyle(Qt::FlatCap);       break;
        case square: penTool->setCapStyle(Qt::SquareCap);   break;
        case round_cap: penTool->setCapStyle(Qt::RoundCap); break;
        default:                                            break;
    }
}

/**
 * @brief DrawArea::OnPenSizeConfig - Update pen size
 *
 */
void DrawArea::OnPenSizeConfig(int value)
{
    penTool->setWidth(value);
}

/**
 * @brief DrawArea::OnEraserConfig - Update eraser thickness
 *
 */
void DrawArea::OnEraserConfig(int value)
{
    eraserTool->setWidth(value);
}

/**
 * @brief DrawArea::OnLineStyleConfig - Update line style for line tool
 *
 */
void DrawArea::OnLineStyleConfig(int lineStyle)
{
    switch (lineStyle)
    {
        case solid: lineTool->setStyle(Qt::SolidLine);                break;
        case dashed: lineTool->setStyle(Qt::DashLine);                break;
        case dotted: lineTool->setStyle(Qt::DotLine);                 break;
        case dash_dotted: lineTool->setStyle(Qt::DashDotLine);        break;
        case dash_dot_dotted: lineTool->setStyle(Qt::DashDotDotLine); break;
        default:                                                      break;
    }
}

/**
 * @brief DrawArea::OnLineCapConfig - Update cap style for line tool
 *
 */
void DrawArea::OnLineCapConfig(int capStyle)
{
    switch (capStyle)
    {
        case flat: lineTool->setCapStyle(Qt::FlatCap);       break;
        case square: lineTool->setCapStyle(Qt::SquareCap);   break;
        case round_cap: lineTool->setCapStyle(Qt::RoundCap); break;
        default:                                             break;
    }
}

/**
 * @brief DrawArea::OnDrawTypeConfig - Update draw type for line tool
 *
 */
void DrawArea::OnDrawTypeConfig(int drawType)
{
    switch (drawType)
    {
        case single: setLineMode(single); break;
        case poly:   setLineMode(poly);   break;
        default:     break;
    }
}

/**
 * @brief DrawArea::OnLineThicknessConfig - Update line thickness for line tool
 *
 */
void DrawArea::OnLineThicknessConfig(int value)
{
    lineTool->setWidth(value);
}

/**
 * @brief DrawArea::OnRectBStyleConfig - Update rectangle boundary line style
 *
 */
void DrawArea::OnRectBStyleConfig(int boundaryStyle)
{
    switch (boundaryStyle)
    {
        case solid: rectTool->setStyle(Qt::SolidLine);                break;
        case dashed: rectTool->setStyle(Qt::DashLine);                break;
        case dotted: rectTool->setStyle(Qt::DotLine);                 break;
        case dash_dotted: rectTool->setStyle(Qt::DashDotLine);        break;
        case dash_dot_dotted: rectTool->setStyle(Qt::DashDotDotLine); break;
        default:                                                      break;
    }
}

/**
 * @brief DrawArea::OnRectShapeTypeConfig - Update rectangle shape setting
 *
 */
void DrawArea::OnRectShapeTypeConfig(int shape)
{
    switch (shape)
    {
        case rectangle: rectTool->setShapeType(rectangle);                 break;
        case rounded_rectangle: rectTool->setShapeType(rounded_rectangle); break;
        case ellipse: rectTool->setShapeType(ellipse);                     break;
        default:                                                           break;
    }
}

/**
 * @brief DrawArea::OnRectFillConfig - Update rectangle fill setting
 *
 */
void DrawArea::OnRectFillConfig(int fillType)
{
    switch (fillType)
    {
        case foreground: rectTool->setFillMode(foreground);
                         rectTool->setFillColor(foregroundColor);      break;
        case background: rectTool->setFillMode(background);
                         rectTool->setFillColor(backgroundColor);      break;
        case no_fill: rectTool->setFillMode(no_fill);
                      rectTool->setFillColor(QColor(Qt::transparent)); break;
        default:                                                       break;
    }
}

/**
 * @brief DrawArea::OnRectBTypeConfig - Update rectangle join style
 *
 */
void DrawArea::OnRectBTypeConfig(int boundaryType)
{
    switch (boundaryType)
    {
        case miter_join: rectTool->setJoinStyle(Qt::MiterJoin);  break;
        case bevel_join: rectTool->setJoinStyle(Qt::BevelJoin);  break;
        case round_join: rectTool->setJoinStyle(Qt::RoundJoin);  break;
        default:                                                 break;
    }
}

/**
 * @brief DrawArea::OnRectLineConfig - Update rectangle line width
 *
 */
void DrawArea::OnRectLineConfig(int value)
{
    rectTool->setWidth(value);
}

/**
 * @brief DrawArea::OnRectCurveConfig - Update rounded rectangle curve
 *
 */
void DrawArea::OnRectCurveConfig(int value)
{
    rectTool->setCurve(value);
}

/**
 * @brief DrawArea::createNewImage - creates a new image of
 *                                   user-specified dimensions
 *
 */
void DrawArea::createNewImage(QSize size)
{
    // save a copy of the old image
    oldImage = image->copy();

    *image = QPixmap(size);
    image->fill(backgroundColor);
    update();

    // for undo/redo
    if(!imagesEqual(oldImage, *image))
        saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::loadImage - Load an image from a user-specified file
 *
 */
void DrawArea::loadImage(QString fileName)
{
    // save a copy of the old image
    oldImage = image->copy();

    image->load(fileName);
    update();

    // for undo/redo
    if(!imagesEqual(oldImage, *image))
        saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::saveImage - Save an image to user-specified file
 *
 */
void DrawArea::saveImage(QString fileName)
{
    image->save(fileName, "BMP");
}

/**
 * @brief DrawArea::resizeImage - Resize image to user-specified dimensions
 *
 */
void DrawArea::resizeImage(QSize size)
{
    // save a copy of the old image
    oldImage = image->copy();

    // if no change, do nothing
    if(image->size() == size)
    {
        return;
    }

    // else re-scale the image
    *image = image->scaled(size, Qt::IgnoreAspectRatio);
    update();

    // for undo/redo
    saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::clearImage - clears an image by filling it with
 *                               the background color
 *
 */
void DrawArea::clearImage()
{
    // save a copy of the old image
    oldImage = image->copy();

    image->fill(backgroundColor);
    update(image->rect());

    // for undo/redo
    if(!imagesEqual(oldImage, *image))
        saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::updateColorConfig - Updates the tools' colors
 *                                      as appropriate
 *
 */
void DrawArea::updateColorConfig(QColor color, int which)
{
    if(which == foreground)
    {
         foregroundColor = color;
         penTool->setColor(foregroundColor);
         lineTool->setColor(foregroundColor);
         rectTool->setColor(foregroundColor);

         if(rectTool->getFillMode() == foreground)
             rectTool->setFillColor(foregroundColor);
    }
    else
    {
        backgroundColor = color;
        eraserTool->setColor(backgroundColor);

        if(rectTool->getFillMode() == background)
            rectTool->setFillColor(backgroundColor);
    }
}

/**
 * @brief DrawArea::setCurrentTool - Sets the current tool, unsetting
 *                                   poly mode if necessary
 *
 */
Tool* DrawArea::setCurrentTool(int newType)
{
    // get the current tool's type
    int currType = currentTool->getType();

    // if no change, return --else cancel poly mode & set tool
    if(newType == currType)
        return currentTool;

    if(currType == line)
        drawingPoly = false;

    switch(newType)
    {
        case pen: currentTool = penTool;        break;
        case line: currentTool = lineTool;      break;
        case eraser: currentTool = eraserTool;  break;
        case rect_tool: currentTool = rectTool; break;
        default:                                break;
    }
    return currentTool;
}

/**
 * @brief DrawArea::setLineMode - Sets the current line draw mode,
 *                                unsetting poly mode if necessary
 *
 */
void DrawArea::setLineMode(const DrawType mode)
{
    if(mode == single)
        drawingPoly = false;

    currentLineMode = mode;
}

/**
 * @brief DrawArea::SaveDrawCommand - Put together a DrawCommand
 *                                  and save it on the undo/redo stack.
 *
 */
void DrawArea::saveDrawCommand(QPixmap old_image)
{
    // put the old and new image on the stack for undo/redo
    QUndoCommand *drawCommand = new DrawCommand(old_image, image);
    undoStack->push(drawCommand);
}

/**
 * @brief DrawArea::createTools - takes care of creating the tools
 *
 */
void DrawArea::createTools()
{
    // create the tools
    penTool = new PenTool(QBrush(Qt::black), DEFAULT_PEN_THICKNESS);
    lineTool = new LineTool(QBrush(Qt::black), DEFAULT_PEN_THICKNESS);
    eraserTool = new EraserTool(QBrush(Qt::white), DEFAULT_ERASER_THICKNESS);
    rectTool = new RectTool(QBrush(Qt::black), DEFAULT_PEN_THICKNESS);

    // set default tool
    currentTool = static_cast<Tool*>(penTool);
}

/**
 * @brief imagesEqual - returns true if the two images are the same
 *
 */
bool imagesEqual(const QPixmap &image1, const QPixmap &image2)
{
    return image1.toImage() == image2.toImage();
}
