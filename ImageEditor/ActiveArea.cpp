#include "ActiveArea.h"
#include <stdexcept>

ActiveArea::ActiveArea(QHash<QString, int>& args, QWidget* parent /* = Q_NULLPTR*/)
    :arguments(args), QWidget(parent)
{
    image.load("TestImage.jpg");
    resize(image.size());
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);
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

void ActiveArea::wheelEvent(QWheelEvent* we)
{
    if (QGuiApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
        //resize(size() * (1.0 + we->angleDelta().y() / 120. / 12.));
    }
    
    QWidget::wheelEvent(we);
}

void ActiveArea::paintEvent(QPaintEvent* pe)
{
    //resize(size() * zoomScale);
    QPainter painter(this);
    QRect rec = pe->rect();
    painter.drawImage(rec, image, rec);

    QWidget::paintEvent(pe);
}

//void ActiveArea::resizeEvent(QResizeEvent* event)
//{
//    if (width() > image.width() || height() > image.height()) {
//        int newWidth = qMax(width() + 128, image.width());
//        int newHeight = qMax(height() + 128, image.height());
//        resizeImage(&image, QSize(newWidth, newHeight));
//        update();
//    }
//
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
    return image.load(fileName);
}

bool ActiveArea::Draw(Instrument* instr, OperationType operType)
{
    try {
        instr->DoOperation(image, arguments, operType);
    }
    catch (std::exception& ex) {
        QMessageBox::information(this, "Unable to perform an operation",
                                 ex.what());
    }
    update();
    return true;
}

