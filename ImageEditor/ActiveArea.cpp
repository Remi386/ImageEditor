#include "ActiveArea.h"
#include <stdexcept>

ActiveArea::ActiveArea(QHash<QString, int>& args, int options /* = CreateNewImage*/,
                       QWidget* parent /* = Q_NULLPTR*/)
    :arguments(args), QWidget(parent)
{
    static int imageCounter = -1;

    if (options & ResetCounter) {
        imageCounter = -1;
    }

    if (options & CreateNewImage) {
        QImage newImage(QSize(600, 400), QImage::Format_RGB32);
        newImage.fill(qRgb(255, 255, 255));
        image = newImage;

        fileName = "Untitled";
        
        if (++imageCounter != 0)
            fileName += QString::number(imageCounter);
    }

    resize(image.size());
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);

    historyBuffer.addImage(image);
}

void ActiveArea::mousePressEvent(QMouseEvent* me)
{
    if (me->button() == Qt::LeftButton) {

        arguments["prevX"] = me->x();
        arguments["prevY"] = me->y();

        arguments["X"] = me->x();
        arguments["Y"] = me->y();

        emit signalMousePressed();
    }

    QWidget::mousePressEvent(me);
}

void ActiveArea::mouseMoveEvent(QMouseEvent* me)
{
    arguments["prevX"] = arguments["X"];
    arguments["prevY"] = arguments["Y"];

    arguments["X"] = me->x();
    arguments["Y"] = me->y();

    emit signalMouseMoved(me->pos());

    QWidget::mouseMoveEvent(me);
}

void ActiveArea::mouseReleaseEvent(QMouseEvent* me)
{
    if (me->button() == Qt::LeftButton) {
        arguments["X"] = me->x();
        arguments["Y"] = me->y();

        emit signalMouseReleased();
    }  

    QWidget::mouseReleaseEvent(me);
}

void ActiveArea::paintEvent(QPaintEvent* pe)
{
    QPainter painter(this);
    QRect rec = pe->rect();

    painter.drawImage(rec, image, rec);

    QWidget::paintEvent(pe);
}

void ActiveArea::leaveEvent(QEvent* e)
{
    emit signalMouseLeaved();
    QWidget::leaveEvent(e);
}

//pretty view of file name
QString ActiveArea::simplifyPath(QString path)
{
    int slashIndex = -1, pointIndex = -1;
    slashIndex = path.lastIndexOf("/");
    pointIndex = path.lastIndexOf('.');

    if (slashIndex != -1) {
        path = path.mid(slashIndex + 1, qMax(pointIndex - slashIndex - 1, -1));
    }

    return path;
}

bool ActiveArea::Save(const QString& supportedExtensions)
{
    if (path.isEmpty())
        return SaveAs(supportedExtensions);

    return isPossibleToClose = image.save(path);
}

bool ActiveArea::SaveAs(const QString& supportedExtensions)
{

    path = QFileDialog::getSaveFileName(this, tr("Save image"), 
                                        QDir::currentPath() + "/" + fileName,
                                        supportedExtensions);

    if (path.isEmpty())
        return false;

    if (image.save(path)) {
        fileName = simplifyPath(path);
        return isPossibleToClose = true;
    }
    else {
        QMessageBox::warning(this,
            tr("Warning"), tr("Unable to save a file: ") + path
        );
    }

    return false;
}

bool ActiveArea::Open(const QString& fName)
{
    if (image.load(fName)) {
        historyBuffer.clear();
        historyBuffer.addImage(image);

        resize(image.size());

        isPossibleToRedo = false;
        isPossibleToUndo = false;
        isPossibleToClose = true;

        emit signalRedoStatusChanged(isPossibleToRedo);
        emit signalUndoStatusChanged(isPossibleToUndo);

        path = fName;
        fileName = simplifyPath(path);

        return true;
    }

    return false;
}

QColor ActiveArea::getPixelColor()
{
    return image.pixelColor(arguments["X"], arguments["Y"]);
}

bool ActiveArea::CloseArea(const QString& supportedExtensions)
{
    if (!isPossibleToClose) {
        QMessageBox mesBox(QMessageBox::Information,
            "ImageEditor", "Do you want to save changes to " + fileName + "?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int status = mesBox.exec();

        if (status == QMessageBox::No)
            return true;

        if (status == QMessageBox::Cancel)
            return false;

        if (status == QMessageBox::Yes) {
            return Save(supportedExtensions);
        }
    }

    return true;
}

bool ActiveArea::Draw(Instrument* instr, OperationType operType)
{
    OpStatus operStatus = OpStatus::InProgress;

    try {
        operStatus = instr->DoOperation(image, arguments, operType);
    }
    catch (std::exception& ex) {
        QMessageBox::information(this, "Unable to perform an operation",
            ex.what());
    }

    switch (operStatus) {
    case OpStatus::Done:
        historyBuffer.addImage(image);

        isPossibleToRedo = false;
        isPossibleToUndo = true;
        isPossibleToClose = false;

        emit signalRedoStatusChanged(isPossibleToRedo);
        emit signalUndoStatusChanged(isPossibleToUndo);
        break;

    case OpStatus::ColorChanged:
        emit signalColorChanged(QColor(arguments["red"], 
                                arguments["green"], arguments["blue"]), true);
        break;
    case OpStatus::InProgress:
        break;
    }

    update();
    return true;
}

bool ActiveArea::Undo()
{
    isPossibleToUndo = historyBuffer.getPrev(image);
    update();

    isPossibleToRedo = true;
    return isPossibleToUndo;
}

bool ActiveArea::Redo()
{
    isPossibleToRedo = historyBuffer.getNext(image);
    update();

    isPossibleToUndo = true;
    return isPossibleToRedo;
}