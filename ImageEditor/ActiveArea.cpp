#include "ActiveArea.h"
#include <stdexcept>

ActiveArea::ActiveArea(QHash<QString, int>& args, QWidget* parent /* = Q_NULLPTR*/)
    :arguments(args), QWidget(parent)
{
    image.load("TestImage.jpg");
    resize(image.size());
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);

    historyBuffer.addImage(image);
}

void ActiveArea::mousePressEvent(QMouseEvent* me)
{
    arguments["prevX"] = me->x();
    arguments["prevY"] = me->y();

    arguments["X"] = me->x();
    arguments["Y"] = me->y();

    emit signalMousePressed();

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
    arguments["X"] = me->x();
    arguments["Y"] = me->y();

    emit signalMouseReleased();

    QWidget::mouseReleaseEvent(me);
}

//void ActiveArea::wheelEvent(QWheelEvent* we)
//{
//    if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
//        if (scaleFactor < 3.0 && scaleFactor > 0.33) {
//            scaleFactor += we->angleDelta().y() / 120. / 12.;
//            emit signalScaleFactorChanged(scaleFactor);
//        }
//    }
//    QWidget::wheelEvent(we);
//}

void ActiveArea::paintEvent(QPaintEvent* pe)
{
    QPainter painter(this);
    QRect rec = pe->rect();

    painter.drawImage(rec, image, rec);

    QWidget::paintEvent(pe);
}

//void ActiveArea::resizeEvent(QResizeEvent* event)
//{
//    /*if (width() > image.width() || height() > image.height()) {
//        int newWidth = qMax(width() + 128, image.width());
//        int newHeight = qMax(height() + 128, image.height());
//        resizeImage(&image, QSize(newWidth, newHeight));
//        update();
//    }*/
//    QWidget::resizeEvent(event);
//}

//void ActiveArea::resizeImage(QImage* image, const QSize& newSize)
//{
//    if (image->size() == newSize)
//        return;
//
//    QImage newImage(newSize, QImage::Format_RGB32);
//    newImage.fill(qRgb(255, 255, 255));
//    QPainter painter(&newImage);
//    painter.drawImage(QPoint(0, 0), *image);
//    *image = newImage;
//}

bool ActiveArea::SaveAs(const QString& fileName, const QString& extension)
{
    return image.save(fileName + "." + extension, extension.toLatin1().constData());
}

bool ActiveArea::Open(const QString& fileName)
{
    if (image.load(fileName)) {
        historyBuffer.clear();
        historyBuffer.addImage(image);
        emit signalRedoStatusChanged(false);
        emit signalUndoStatusChanged(false);
        resize(image.size());
        return true;
    }

    return false;
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
        emit signalRedoStatusChanged(false);
        emit signalUndoStatusChanged(true);
        break;

    case OpStatus::ColorChanged:
        emit signalColorChanged(QColor(arguments["red"], 
                                arguments["green"], arguments["blue"]));
        break;
    default:
        break;
    }

    update();
    return true;
}

bool ActiveArea::Undo()
{
    QImage newImage;

    bool statFlag = historyBuffer.getPrev(newImage);

    if (!newImage.isNull()) {
        image = newImage;
        update();
    }

    return statFlag;
}

bool ActiveArea::Redo()
{
    QImage newImage;

    bool statFlag = historyBuffer.getNext(newImage);

    if (!newImage.isNull()) {
        image = newImage;
        update();
    }

    return statFlag;
}