#include "Flood.h"
#include <QImage>
#include <QHash>
#include <QString>
#include <QPainter>
#include <stack>

constexpr double ColorDifferenceValue = 20.0;

//low cost approximation for finding distance
// between colors. source: https://www.compuphase.com/cmetric.htm
double colorDifference(const QColor& color1, const QColor& color2)
{
	int r1, r2, g1, g2, b1, b2;
	color1.getRgb(&r1, &g1, &b1);
	color2.getRgb(&r2, &g2, &b2);

	long rmean = (r1 + r2) / 2;
	long r = r1 - r2;
	long g = g1 - g2;
	long b = b1 - b2;
	return sqrt((((512 + rmean) * r * r) >> 8) + 4 * g * g + (((767 - rmean) * b * b) >> 8));
}

void push(std::stack<int>& stack, int x, int y)
{
	stack.push(x);
	stack.push(y);
}

bool pop(std::stack<int>& stack, int& x, int& y)
{
	if (stack.size() < 2) 
		return false;

	y = stack.top();
	stack.pop();
	x = stack.top();
	stack.pop();
	return true;
}

//The scanline floodfill algorithm 
bool floodFill(QImage& image, int x, int y, QColor oldColor, QColor newColor)
{
	if (colorDifference(newColor, oldColor) < ColorDifferenceValue)
		return false;

	int x1;
	bool spanAbove, spanBelow;
	int width = image.size().width();
	int height = image.size().height();

	std::stack<int> stack;
	push(stack, x, y);

	while (pop(stack, x, y))
	{
		x1 = x;

		while (x1 >= 0 && colorDifference(image.pixelColor(x1, y), oldColor) < ColorDifferenceValue) {
			x1--;
		}

		x1++;
		spanAbove = spanBelow = false;

		while (x1 < width && colorDifference(image.pixelColor(x1, y), oldColor) < ColorDifferenceValue)
		{
			image.setPixelColor({ x1, y }, newColor);
			double upperPixelDiff = colorDifference(image.pixelColor(x1, y - 1), oldColor);
			double lowerPixelDiff = colorDifference(image.pixelColor(x1, y + 1), oldColor);

			if (!spanAbove && y > 0 && upperPixelDiff < ColorDifferenceValue)
			{
				push(stack, x1, y - 1);
				spanAbove = true;
			}
			else if (spanAbove && y > 0 && upperPixelDiff > ColorDifferenceValue)
			{
				spanAbove = false;
			}

			if (!spanBelow && y < height - 1 && lowerPixelDiff < ColorDifferenceValue)
			{
				push(stack, x1, y + 1);
				spanBelow = true;
			}
			else if (spanBelow && y < height - 1 && lowerPixelDiff > ColorDifferenceValue)
			{
				spanBelow = false;
			}
			x1++;
		}
	}
	return true;
}

OpStatus Flood::DoOperation(QImage& image, QHash<QString, int>& arguments, OperationType operType)
{
	if (operType == OperationType::Press) {

		auto [_, curPos, activeColor, __] = getParameters(arguments);

		QColor colorToReplace = image.pixelColor(curPos);

		if(floodFill(image, curPos.x(), curPos.y(), colorToReplace, activeColor))
			return OpStatus::Done;
	}

	return OpStatus::InProgress;
}