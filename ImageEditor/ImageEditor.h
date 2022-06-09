#pragma once
#include <QtWidgets/QMainWindow>

class QLabel;

class ImageEditor : public QMainWindow
{
    Q_OBJECT
private:
    QString supportedExtensions;
    QLabel* mousePositionLabel;

    bool CloseWindow();

public:
    ImageEditor(QWidget *parent = Q_NULLPTR);

signals:
    void signalNew();
    void signalOpen();
    void signalSave();
    void signalSaveAs();
    void signalUndo();
    void signalRedo();
    void signalIncreasePenSize();
    void signalDecreasePenSize();

public slots:
    void slotAbout();
    void slotMouseMoved(QPoint mousePosition);
    void slotMouseLeaved();

protected:
    void closeEvent(QCloseEvent* ce) override;
};
