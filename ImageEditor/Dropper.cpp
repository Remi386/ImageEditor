#include "Dropper.h"

#include <QImage>
#include <QHash>
#include <QString>

bool Dropper::DoOperation(QImage& image, QHash<QString, int>& arguments, OperationType operType)
{
	auto [_, curPos, __, ___] = getParameters(arguments);
	
	QColor color = image.pixelColor(curPos);
	
	color.getRgb(&arguments["red"], &arguments["green"], &arguments["blue"]);

	return true;
}