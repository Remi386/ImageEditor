#include "ActiveArea.h"

ActiveArea::ActiveArea(QWidget* parent /* = Q_NULLPTR*/)
    :QWidget(parent)
{
    image.load("TestImage.jpg");
    resize(image.size());
    setAttribute(Qt::WA_StaticContents);
}

//void ActiveArea::mousePressEvent(QMouseEvent* me)
//{
//    
//}
//
//void ActiveArea::mouseMoveEvent(QMouseEvent* me) 
//{
//
//}
//
//void ActiveArea::mouseReleaseEvent(QMouseEvent* me)
//{
//
//}

void ActiveArea::paintEvent(QPaintEvent* pe)
{
    QPainter painter(this);
    QRect rec = pe->rect();
    painter.drawImage(rec, image, rec);

    QWidget::paintEvent(pe);
}

void ActiveArea::resizeEvent(QResizeEvent* event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }

    QWidget::resizeEvent(event);
}

void ActiveArea::resizeImage(QImage* image, const QSize& newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ActiveArea::slotColorChanged(QColor& col)
{
    activeColor = col;
}

bool ActiveArea::SaveAs(const QString& fileName, const QString& extension)
{
    return image.save(fileName + "." + extension, extension.toLatin1().constData());
}

bool ActiveArea::Open(const QString& fileName)
{
    return image.load(fileName);
}