#pragma once
#include "InstrumentBase.h"
#include <QImage>
#include <QPoint>

class Line : public Instrument {
public:
	virtual ~Line() = default;

	virtual bool DoOperation(QImage& image, QHash<QString, int>& arguments,
		OperationType operType) override;

private:
	QImage imageCopy;
	QPoint prevMousePosition;
};