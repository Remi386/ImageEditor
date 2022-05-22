#pragma once
#include <QtWidgets>
#include "Pencil.h"

class ActiveArea : public QWidget {
	Q_OBJECT
public:
	ActiveArea(QHash<QString, int>& arguments, QWidget* parent = Q_NULLPTR);

	bool SaveAs(const QString& fileName, const QString& extension);

	bool Open(const QString& fileName);

	bool Draw(Instrument* instr, OperationType operType);

private:
	QImage image;
	QString fileName;
	QHash<QString, int>& arguments;

signals:
	void signalMousePressed();
	void signalMouseMoved(QPoint mousePosition);
	void signalMouseReleased();

protected:
	void mousePressEvent(QMouseEvent* me) override;
	void mouseMoveEvent(QMouseEvent* me) override;
	void mouseReleaseEvent(QMouseEvent* me) override;

	void paintEvent(QPaintEvent* pe) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void resizeImage(QImage* image, const QSize& newSize);
};