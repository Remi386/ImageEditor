#pragma once
#include "InstrumentBase.h"

class Brush : public Instrument {
public:

	virtual ~Brush() = default;

	virtual bool DoOperation(QImage & image, QHash<QString, int>&arguments,
		OperationType operType) override;
};