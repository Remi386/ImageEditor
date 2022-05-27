#pragma once
#include "InstrumentBase.h"

class Dropper : public Instrument {
public:
	virtual ~Dropper() = default;

	virtual bool DoOperation(QImage& image, QHash<QString, int>& arguments,
		OperationType operType) override;
};