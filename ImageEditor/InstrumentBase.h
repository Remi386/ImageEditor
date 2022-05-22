#pragma once
#include <tuple>

class QImage;
class QString;
class QColor;
class QPoint;

template<typename Type1, typename Type2>
class QHash;

//Defining from what event we drawing
enum class OperationType {
	Press, 
	Move,
	Release
};

class Instrument
{
public:
	virtual ~Instrument() = default;

	virtual bool DoOperation(QImage& image, QHash<QString, int>& arguments, 
							 OperationType operType) = 0;

	std::tuple<QPoint, QPoint, QColor, int> getParameters(QHash<QString, int>& args);

};