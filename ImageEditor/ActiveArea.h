#pragma once
#include <QtWidgets>
#include "InstrumentBase.h"
#include "HistoryBuffer.h"
#include <tuple>

class ActiveArea : public QWidget {
	Q_OBJECT
public:

	enum Option {
		Nothing = 0x00,
		CreateNewImage = 0x01,
		ResetCounter = 0x02
	};

	ActiveArea(QHash<QString, int>& arguments, int options = CreateNewImage,
		       QWidget* parent = Q_NULLPTR);

	bool Open(const QString& fileName);

	bool Save(const QString& supportedExtensions);

	bool SaveAs(const QString& supportedExtensions);

	bool CloseArea(const QString& supportedExtensions);

	bool Draw(Instrument* instr, OperationType operType);

	bool Undo();

	bool Redo();

	QString GetImageName() { return fileName; };

	QString GetPath() { return path; };

	std::tuple<bool, bool> GetRedoUndoStatus() 
		{ return std::make_tuple(isPossibleToRedo, isPossibleToUndo); }

private:
	QImage image;
	QString fileName;
	QString path;
	QHash<QString, int>& arguments;
	HistoryBuffer historyBuffer;
	size_t currentHistoryIndex = 0;
	bool isPossibleToRedo = false;
	bool isPossibleToUndo = false;
	bool isPossibleToClose = true;

	QString simplifyPath(QString path);

signals:
	void signalMousePressed();
	void signalMouseMoved(QPoint mousePosition);
	void signalMouseReleased();
	void signalRedoStatusChanged(bool status);
	void signalUndoStatusChanged(bool status);
	void signalColorChanged(const QColor& newColor);
	void signalCursorEntered();
	void signalCursorLeaved();

protected:
	void mousePressEvent(QMouseEvent* me) override;
	void mouseMoveEvent(QMouseEvent* me) override;
	void mouseReleaseEvent(QMouseEvent* me) override;
	void enterEvent(QEvent* e) override;
	void leaveEvent(QEvent* e) override;
	void paintEvent(QPaintEvent* pe) override;
};