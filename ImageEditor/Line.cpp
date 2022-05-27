#include "Line.h"
#include <QHash>
#include <QString>
#include <QPainter>

bool Line::DoOperation(QImage& image, QHash<QString, int>& arguments, OperationType operType)
{

	auto [_, curPos, color, penWidth] = getParameters(arguments);

	if (operType == OperationType::Press) {
		prevMousePosition = curPos;
		imageCopy = image;
	}

	image = imageCopy;

	QPainter painter(&image);

	QPen pen;
	pen.setWidth(penWidth);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setBrush(QBrush(color, Qt::SolidPattern));

	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.drawLine(prevMousePosition, curPos);

	return true;
}