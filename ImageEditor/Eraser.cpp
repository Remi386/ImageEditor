#include "Eraser.h"

#include <QImage>
#include <QHash>
#include <QString>
#include <QPainter>

bool Eraser::DoOperation(QImage& image, QHash<QString, int>& arguments,	OperationType operType)
{
	QPainter painter(&image);

	auto [prevPos, curPos, _, penWidth] = getParameters(arguments);

	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
	QRect rect = QRect(curPos.x() - penWidth / 2, curPos.y() - penWidth / 2, penWidth, penWidth);
	painter.drawRect(rect);

	return true;
}