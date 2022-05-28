#pragma once
#include "InstrumentBase.h"

class Pencil : public Instrument {

public:
	virtual ~Pencil() = default;

	virtual OpStatus DoOperation(QImage& image, QHash<QString, int>& arguments,
							     OperationType operType) override;
};