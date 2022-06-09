#pragma once

#include <QtWidgets/QSlider>

class PenWidthSlider : public QSlider {
	Q_OBJECT
public:
	PenWidthSlider(Qt::Orientation orient = Qt::Vertical, QWidget* parent = Q_NULLPTR) 
		: QSlider(orient, parent)
	{}

public slots:
	void setValueFromString(const QString& text)
	{
		int newInt = text.toInt();

		if(newInt != value())
			setValue(newInt);
	}
};