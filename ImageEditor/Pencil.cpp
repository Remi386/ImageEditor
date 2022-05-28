#include "Pencil.h"
#include <QImage>
#include <QHash>
#include <QString>
#include <QPainter>

OpStatus Pencil::DoOperation(QImage& image, QHash<QString, int>& arguments, OperationType operType)
{
	QPainter painter(&image);
	
	auto [prevPos, curPos, color, penWidth] = getParameters(arguments);

	QPen pen;
	pen.setWidth(penWidth);
	pen.setColor(color);
	pen.setCapStyle(Qt::RoundCap);
	pen.setBrush(QBrush(color, Qt::SolidPattern));

	painter.setPen(pen);

	painter.drawLine(prevPos, curPos);

	if (operType == OperationType::Release)
		return OpStatus::Done;

	return OpStatus::InProgress;
}