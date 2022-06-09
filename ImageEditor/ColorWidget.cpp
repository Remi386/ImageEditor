#include "ColorWidget.h"

namespace {
	constexpr int BaseWidth = 60;
	constexpr int BaseHeight = 75;
	
	constexpr int colorRecWidth = 40;
	constexpr int colorRecHeight = 45;

	const int characterWidth = 5;
}

ColorWidget::ColorWidget(QString _text, QColor defaultColor, QWidget* parent /* = Q_NULLPTR */)
	: QWidget(parent), text(_text), mColor(defaultColor)
{
	setAttribute(Qt::WA_Hover);
	setMouseTracking(true);
	setMinimumSize(BaseWidth + 1, BaseHeight);
}

void ColorWidget::paintEvent(QPaintEvent* pe)
{
	QRect rec = pe->rect().adjusted(0, 0, -1, -1);

	if (_isActive) { //
		drawBackground(rec, QColor(90, 90, 90), QColor(50, 80, 90));
	}

	if (isHovered) {
		drawBackground(rec, QColor(100, 100, 100, 100), QColor(110, 110, 110, 150));
	}

	QPoint colorPosition = QPoint((rec.width() - colorRecWidth) / 2,
								   rec.height() / 5);

	QPainter painter(this);
	
	painter.setPen(Qt::gray);
	painter.drawRect(QRect(colorPosition, QSize(colorRecWidth, colorRecHeight)));

	painter.setPen(Qt::white);
	painter.drawRect(QRect(colorPosition + QPoint(1, 1), 
						   QSize(colorRecWidth - 2, colorRecHeight - 2)));

	painter.setBrush(QBrush(mColor, Qt::SolidPattern));
	painter.drawRect(QRect(colorPosition + QPoint(2, 2),
						   QSize(colorRecWidth - 4, colorRecHeight - 4)));

	painter.setPen(QColor(Qt::black));
	painter.drawText(rec.width() / 2 - (text.size() / 2) * characterWidth,
					 rec.height() - 3, text);

	QWidget::paintEvent(pe);
}

void ColorWidget::drawBackground(QRect area, QColor boundary, QColor body)
{
	QPainter painter(this);
	painter.setPen(boundary);
	painter.drawRect(area);

	painter.setBrush(body);
	painter.drawRect(area.adjusted(1, 1, -1, -1));
}

void ColorWidget::mousePressEvent(QMouseEvent* me)
{
	if (me->button() == Qt::RightButton) {
		mColor = QColorDialog::getColor(mColor, this, "Choose color");
		update();
	}

	if (me->button() == Qt::LeftButton) {
		_isActive = true;
		update();
	}

	if(_isActive)
		emit signalActiveColorChanged(mColor, false);
}

void ColorWidget::setActive(bool activeFlag /* = true*/)
{
	_isActive = activeFlag;
	update();
}

void ColorWidget::enterEvent(QEvent* e)
{
	isHovered = true;
	update();

	QWidget::enterEvent(e);
}

void ColorWidget::leaveEvent(QEvent* e)
{
	isHovered = false;
	update();

	QWidget::leaveEvent(e);
}

//This function called from central window
void ColorWidget::setColor(const QColor& newColor)
{
	mColor = newColor;
	update();
}

//Receiving this signal from another color widget 
void ColorWidget::slotActiveColorChanged(const QColor& newColor, bool shouldChangeColorWidget)
{
	_isActive = false;
	update();
}