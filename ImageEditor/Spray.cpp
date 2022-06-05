#include "Spray.h"
#include <QImage>
#include <QHash>
#include <QString>
#include <QPainter>
#include <random>

namespace {
	std::mt19937 mersenne;
	std::uniform_real_distribution<> degree(0, 360);
	constexpr int pointsCount = 40;
}

OpStatus Spray::DoOperation(QImage& image, QHash<QString, int>& arguments, OperationType operType)
{
	QPainter painter(&image);

	auto [prevPos, curPos, color, penWidth] = getParameters(arguments);

	std::uniform_real_distribution<> radius(0, penWidth);

	QPen pen;
	pen.setWidth(1);
	pen.setBrush(QBrush(color, Qt::SolidPattern));

	painter.setPen(pen);

	//number of points depends on penWidth
	int points = qMin(pointsCount, penWidth * 5 / 2);

	for (int i = 0; i < points; ++i) {
		double distance = radius(mersenne);
		double degr = degree(mersenne);

		int x = curPos.x() + std::sin(degr) * distance;
		int y = curPos.y() + std::cos(degr) * distance;

		painter.drawPoint(QPoint(x, y));
	}

	if (operType == OperationType::Release)
		return OpStatus::Done;

	return OpStatus::InProgress;
}