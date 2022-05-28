#include "InstrumentBase.h"
#include <QString>
#include <QHash>
#include <QColor>
#include <QPoint>
#include <stdexcept>

std::tuple<QPoint, QPoint, QColor, int> Instrument::getParameters(QHash<QString, int>& args)
{
#ifdef QT_DEBUG

	if (!args.contains("prevX") || !args.contains("prevY"))
		throw std::runtime_error("Error while extracting previous mouse position");

	if (!args.contains("X") || !args.contains("Y"))
		throw std::runtime_error("Error while extracting current mouse position");

	if (!args.contains("red") || !args.contains("green") || !args.contains("blue"))
		throw std::runtime_error("Error while extracting color");
	
	if (!args.contains("penSize"))
		throw std::runtime_error("Error while extracting instrument's width");

#endif // QT_DEBUG
	
	QColor color(args.value("red"), args.value("green"), args.value("blue"));
	
	QPoint prevPos(args.value("prevX"), args.value("prevY"));
	
	QPoint curPos(args.value("X"), args.value("Y"));

	return std::make_tuple(prevPos, curPos, color, args.value("penSize"));
}
