#pragma once
#include "InstrumentBase.h"

constexpr double ColorDifferenceValue = 20.0;

double colorDifference(const QColor& color1, const QColor& color2);

class Flood : public Instrument {
public:
	virtual ~Flood() = default;

	virtual OpStatus DoOperation(QImage& image, QHash<QString, int>& arguments,
								 OperationType operType) override;

	virtual bool isScalable() const override { return false; };
};