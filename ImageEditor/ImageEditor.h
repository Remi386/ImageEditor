#pragma once
#include <QtWidgets>

class ImageEditor : public QMainWindow
{
    Q_OBJECT
private:
    QString supportedExtensions;
    QLabel* mousePosition;

public:
    ImageEditor(QWidget *parent = Q_NULLPTR);

signals:
    void signalOpen(const QString& fileName);
    void signalSave(const QString& fileName, const QString& fileExtension);

public slots:
    void slotSave();
    void slotOpen();
    void slotAbout();
    void slotMouseMoved(QPoint mousePosition);

};
