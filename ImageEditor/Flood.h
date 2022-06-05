#pragma once
#include "InstrumentBase.h"

class Flood : public Instrument {
public:
	virtual ~Flood() = default;

	virtual OpStatus DoOperation(QImage& image, QHash<QString, int>& arguments,
								 OperationType operType) override;

	virtual bool isScalable() const override { return false; };
};