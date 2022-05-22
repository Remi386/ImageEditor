#include "Pencil.h"
#include <QImage>
#include <QHash>
#include <QString>
#include <QPainter>

bool Pencil::DoOperation(QImage& image, QHash<QString, int>& arguments, OperationType operType)
{
	QPainter painter(&image);
	
	auto [prevPos, curPos, color, penWidth] = getParameters(arguments);

	QPen pen;
	pen.setWidth(penWidth);
	pen.setColor(color);

	painter.setPen(pen);
	painter.setBrush(QBrush(color, Qt::SolidPattern));
	painter.setRenderHint(QPainter::Antialiasing);

	painter.drawLine(prevPos, curPos);
	return true;
}