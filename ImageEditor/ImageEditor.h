#pragma once
#include <QtWidgets>

class ImageEditor : public QMainWindow
{
    Q_OBJECT
private:
    QString supportedExtensions;
    QLabel* mousePosition;

    QAction* undo;
    QAction* redo;

public:
    ImageEditor(QWidget *parent = Q_NULLPTR);

signals:
    void signalOpen(const QString& fileName);
    void signalSave(const QString& fileName, const QString& fileExtension);
    void signalUndo();
    void signalRedo();

public slots:
    void slotSave();
    void slotOpen();
    void slotAbout();
    void slotMouseMoved(QPoint mousePosition);

    void slotStatusUndoChanged(bool status);
    void slotStatusRedoChanged(bool status);
};
