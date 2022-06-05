#pragma once
#include <QtWidgets>

class ColorWidget : public QWidget {
	Q_OBJECT
public:
	ColorWidget(QString text, QColor defaultColor, QWidget* parent = Q_NULLPTR);

	void setActive(bool activeFlag = true);

	bool isActive() { return _isActive; }

	void setColor(const QColor& newColor);

signals:
	void signalActiveColorChanged(const QColor& newColor);

public slots:
	void slotActiveColorChanged(const QColor& newColor);

protected:
	void paintEvent(QPaintEvent* pe) override;
	void mousePressEvent(QMouseEvent* me) override;
	void enterEvent(QEvent* e) override;
	void leaveEvent(QEvent* e) override;
	
private:
	QString text;
	QColor mColor;
	
	bool isHovered = false;
	bool _isActive = false;

	void drawBackground(QRect area, QColor boundary, QColor body);
};
