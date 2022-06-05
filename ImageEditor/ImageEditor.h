#pragma once
#include <QtWidgets>

class ImageEditor : public QMainWindow
{
    Q_OBJECT
private:
    QString supportedExtensions;
    QLabel* mousePositionLabel;

    QAction* undo;
    QAction* redo;

    bool CloseWindow();

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

protected:
    void closeEvent(QCloseEvent* ce) override;
};
