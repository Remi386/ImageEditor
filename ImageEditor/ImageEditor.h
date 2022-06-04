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
    void signalNew();
    void signalOpen(const QString& fileName);
    void signalSave();
    void signalSaveAs();
    void signalUndo();
    void signalRedo();

public slots:
    void slotOpen();
    void slotAbout();
    void slotMouseMoved(QPoint mousePosition);

    void slotStatusUndoChanged(bool status);
    void slotStatusRedoChanged(bool status);
};
