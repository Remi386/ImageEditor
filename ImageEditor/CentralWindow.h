#pragma once
#include <QtWidgets>
#include "ActiveArea.h"

class CentralWindow : public QWidget
{
	Q_OBJECT
public:
	CentralWindow(QWidget* parent = Q_NULLPTR);

	~CentralWindow();

private:
	void connectSignals();
	void createActions();
	void changeInstrument(Instrument* newInst);

signals:
	void signalMouseMoved(QPoint mousePosition);

public slots:
	void slotMousePressed();
	void slotMouseMoved(QPoint mousePosition);
	void slotMouseReleased();

	void slotSaveAs(const QString& fileName, const QString& fileExtension);
	void slotOpenFile(const QString& fileName);

	void slotColorChanged(const QColor& newColor);
	void slotPenSizeChanged(int newSize);

private:
	ActiveArea* area;
	Instrument* currentInstrument;

	QHash<QString, int> arguments;
	QColor activeColor = Qt::black;
	QColor secondColor = Qt::white;

	int penSize = 1;
	bool isMousePressed = false;

	QAction* choosePen;
	QAction* chooseBrush;
	QAction* chooseEraser;
	QAction* chooseDropper;
	QAction* chooseHand;
	QAction* chooseRotate;
	QAction* chooseSpray;
	QAction* chooseLine;
	QAction* chooseColor;

};