#include "Brush.h"

#include <QImage>
#include <QHash>
#include <QString>
#include <QPainter>

bool Brush::DoOperation(QImage& image, QHash<QString, int>& arguments, OperationType operType)
{
	QPainter painter(&image);

	auto [prevPos, curPos, color, penWidth] = getParameters(arguments);

	QPen pen;
	pen.setWidth(penWidth);
	pen.setColor(color);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setBrush(QBrush(color, Qt::SolidPattern));

	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.drawLine(prevPos, curPos);
	return true;
}