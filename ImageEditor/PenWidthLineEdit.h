#pragma once

#include <QtWidgets/QLineEdit>

class PenWidthLineEdit : public QLineEdit {
	Q_OBJECT
public:
	PenWidthLineEdit(const QString& text, QWidget* parent = Q_NULLPTR)
		: QLineEdit(text, parent)
	{}

public slots:
	void setTextFromValue(int value)
	{
		QString newText = QString::number(value);
		
		if (newText != text())
			setText(newText);
	}
	
};