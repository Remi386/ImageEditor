#pragma once

#include <QtWidgets>
//#include "CentralWindow.h"

class ImageEditor : public QMainWindow
{
    Q_OBJECT
private:
    QString supportedExtensions;
public:
    ImageEditor(QWidget *parent = Q_NULLPTR);

signals:
    void signalOpen(const QString& fileName);
    void signalSave(const QString& fileName, const QString& fileExtension);

public slots:
    void slotSave();
    void slotOpen();

private:
    void setUpMenus();
};
