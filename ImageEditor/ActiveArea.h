#pragma once
#include <QtWidgets>

class ActiveArea : public QWidget {
	Q_OBJECT
public:
	ActiveArea(QWidget* parent = Q_NULLPTR);

	bool SaveAs(const QString& fileName, const QString& extension);

	bool Open(const QString& fileName);

private:
	//QWidget* area;
	QImage image;
	QColor activeColor = Qt::black;
	QString fileName;

public slots:
	void slotColorChanged(QColor& color);

protected:
	//void mousePressEvent(QMouseEvent* me) override;
	//void mouseMoveEvent(QMouseEvent* me) override;
	//void mouseReleaseEvent(QMouseEvent* me) override;

	void paintEvent(QPaintEvent* pe) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void resizeImage(QImage* image, const QSize& newSize);
};