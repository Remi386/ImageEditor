#pragma once
#include "InstrumentBase.h"
#include <QImage>
#include <QPoint>

class Line : public Instrument {
public:
	virtual ~Line() = default;

	virtual OpStatus DoOperation(QImage& image, QHash<QString, int>& arguments,
							     OperationType operType) override;

	virtual bool isScalable() const override { return true; };

private:
	QImage imageCopy;
	QPoint prevMousePosition;
};