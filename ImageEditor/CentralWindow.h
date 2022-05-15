#pragma once

#include <QtWidgets>
#include "ActiveArea.h"

class CentralWindow : public QWidget
{
	Q_OBJECT
public:
	CentralWindow(QWidget* parent = Q_NULLPTR);

public slots:
	void slotSaveAs  (const QString& fileName, const QString& fileExtension);
	void slotOpenFile(const QString& fileName);

private:
	ActiveArea* area;
};