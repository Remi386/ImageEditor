#pragma once
#include <QtWidgets>
#include "ActiveArea.h"
#include "ColorWidget.h"

class CentralWindow : public QWidget
{
	Q_OBJECT
public:
	CentralWindow(QWidget* parent = Q_NULLPTR);

	~CentralWindow();

private:
	void connectSignals();
	void connectActiveArea(ActiveArea* area);
	void createActions();
	void changeInstrument(Instrument* newInst);
	ActiveArea* GetActiveArea(int index = -1);
	void createNewTab(ActiveArea* actArea);

signals:
	void signalMouseMoved(QPoint mousePosition);
	void signalUndoStatus(bool status);
	void signalRedoStatus(bool status);
	void signalColorChanged(QColor newColor);

public slots:
	void slotMousePressed();
	void slotMouseMoved(QPoint mousePosition);
	void slotMouseReleased();

	void slotNewFile();
	void slotSaveAs();
	void slotSave();
	void slotOpenFile(const QString& fileName);
	void slotCloseRequest(int index);

	void slotColorChanged(const QColor& newColor);
	void slotPenSizeChanged(int newSize);

	void slotUndo();
	void slotRedo();

	void slotCurrentWidgetChanged();
private:
	QTabWidget* tabWidget;
	Instrument* currentInstrument;

	QHash<QString, int> arguments;
	QColor activeColor = Qt::black;

	int penSize = 1;
	bool isMousePressed = false;

	QAction* choosePen;
	QAction* chooseBrush;
	QAction* chooseEraser;
	QAction* chooseDropper;
	QAction* chooseFlood;
	QAction* chooseSpray;
	QAction* chooseLine;
	QAction* chooseColor;

	ColorWidget* color1;
	ColorWidget* color2;
};