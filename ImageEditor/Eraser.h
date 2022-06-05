#pragma once
#include "InstrumentBase.h"

class Eraser : public Instrument {
public:
	virtual ~Eraser() = default;

	virtual OpStatus DoOperation(QImage& image, QHash<QString, int>& arguments,
							     OperationType operType) override;

	virtual bool isScalable() const override { return true; };
};