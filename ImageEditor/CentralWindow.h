#pragma once
#include <QtWidgets>
#include "ActiveArea.h"

class ColorWidget;
class PenWidthLineEdit;

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
	void setCursor(ActiveArea* area, bool onMouseMove, bool forceSet = false);

signals:
	void signalMouseMoved(QPoint mousePosition);
	void signalUndoStatus(bool status);
	void signalRedoStatus(bool status);
	void signalColorChanged(QColor newColor);
	void signalMouseLeaved();
	void signalIncPenStatus(bool status);
	void signalDecPenStatus(bool status);

public slots:
	void slotMousePressed();
	void slotMouseMoved(QPoint mousePosition);
	void slotMouseReleased();

	void slotNewFile();
	void slotSaveAs();
	void slotSave();
	void slotOpenFile();
	void slotCloseRequest(int index);

	void slotColorChanged(const QColor& newColor, bool shouldChangeColorWidget);
	void slotPenSizeChanged(int newSize);
	void slotIncreasePenSize();
	void slotDecreasePenSize();

	void slotUndo();
	void slotRedo();
	void slotSwapColors();

	void slotCurrentWidgetChanged();
private:
	QTabWidget* tabWidget;

	QString supportedExtensionsSave;
	QString supportedExtensionsOpen;

	QHash<QString, QPixmap> cursors;
	QHash<QString, Instrument*> instruments;
	QHash<QString, int> arguments;

	Instrument* activeInstrument;
	QPixmap activeCursor;
	QColor activeColor = Qt::black;

	int penSize = 1;
	bool isMousePressed = false;
	bool isPosToIncPen = true;
	bool isPosToDecPen = false;

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
	PenWidthLineEdit* pLineEdit;
};