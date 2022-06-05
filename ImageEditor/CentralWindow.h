#pragma once
#include <QtWidgets>
#include "ActiveArea.h"

class ColorWidget;

class CentralWindow : public QWidget
{
	Q_OBJECT
public:
	CentralWindow(QWidget* parent = Q_NULLPTR);

	bool tryToClose();

	~CentralWindow();

private:
	void connectSignals();
	void connectActiveArea(ActiveArea* area);
	void createActions();
	void createInstruments();
	void changeInstrument(const QString& instName);
	ActiveArea* GetActiveArea(int index = -1);
	void createNewTab(ActiveArea* actArea);
	void getSupportedExtensions();
	bool closeTab(int index);

signals:
	void signalMouseMoved(QPoint mousePosition);
	void signalUndoStatus(bool status);
	void signalRedoStatus(bool status);
	void signalColorChanged(QColor newColor);

public slots:
	void slotMousePressed();
	void slotMouseMoved(QPoint mousePosition);
	void slotMouseReleased();
	void slotCursorEnteredArea();
	//void slotCursorLeavedArea();

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

	QString supportedExtensions;

	QHash<QString, QPixmap> cursors;
	QHash<QString, Instrument*> instruments;
	QHash<QString, int> arguments;

	Instrument* activeInstrument;
	QPixmap activeCursor;
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