#pragma once
#include "InstrumentBase.h"

class Spray : public Instrument{
public:
	virtual ~Spray() = default;

	virtual OpStatus DoOperation(QImage& image, QHash<QString, int>& arguments,
							     OperationType operType) override;

	virtual bool isScalable() const override { return false; };
};