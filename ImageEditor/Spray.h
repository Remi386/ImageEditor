#pragma once
#include "InstrumentBase.h"

class Spray : public Instrument{
public:
	virtual ~Spray() = default;

	virtual bool DoOperation(QImage& image, QHash<QString, int>& arguments,
		OperationType operType) override;
};