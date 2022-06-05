#pragma once
#include <tuple>

class QImage;
class QString;
class QColor;
class QPoint;

template<typename Type1, typename Type2>
class QHash;

enum class OpStatus {
	Done,
	InProgress,
	ColorChanged // For dropper
};

//Defining from what event we are drawing
enum class OperationType {
	Press, 
	Move,
	Release
};

class Instrument
{
public:
	virtual ~Instrument() = default;

	virtual OpStatus DoOperation(QImage& image, QHash<QString, int>& arguments,
							     OperationType operType) = 0;

	/// <summary>
	/// Returns a tuple with previous position of mouse, current position,
	/// active color and pen width
	/// </summary>
	std::tuple<QPoint, QPoint, QColor, int> getParameters(QHash<QString, int>& args);

	virtual bool isScalable() const = 0;
};