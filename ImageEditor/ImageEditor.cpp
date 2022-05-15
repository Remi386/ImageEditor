#include "ImageEditor.h"
#include "stdafx.h"
//#include "ActiveArea.h"
#include "CentralWindow.h"

ImageEditor::ImageEditor(QWidget *parent)
    : QMainWindow(parent)
{
    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* open = new QAction("&Open file");
    open->setShortcut(Qt::CTRL + Qt::Key_A);
    connect(open, SIGNAL(triggered()), this, SLOT(slotOpen()));
    
    QAction* save = new QAction("&Save");
    save->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(save, SIGNAL(triggered()), this, SLOT(slotSave()));

    fileMenu->addAction(open);
    fileMenu->addSeparator();
    fileMenu->addAction(save);
    fileMenu->addSeparator();
    fileMenu->addAction("&Exit", qApp, SLOT(quit()));
    
    CentralWindow* centralWindow = new CentralWindow;
    
    bool bOk = true;
    //bOk &= (bool)connect(open, SIGNAL(triggered()), centralWindow, SLOT(slotOpenFile()));
    bOk &= (bool)connect(this, SIGNAL(signalOpen(const QString&)), centralWindow, SLOT(slotOpenFile(const QString&)));

    bOk &= (bool)connect(this, SIGNAL(signalSave(const QString&, const QString&)), 
                         centralWindow, SLOT(slotSaveAs(const QString&, const QString&)));
    Q_ASSERT(bOk);

    setCentralWidget(centralWindow);
    
    //Getting supported extensions

    bool begin = true;
    foreach(const QByteArray& format, QImageWriter::supportedImageFormats()) {
        if (begin) 
            begin = false;
        else
            supportedExtensions += ";;";

        supportedExtensions += QString::fromLatin1(format);
    };
   
    
    resize(500, 400);
}

void ImageEditor::setUpMenus()
{

}

void ImageEditor::slotOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), 
                                                    "", tr("Image Files (*.png *.jpg *.bmp)"));

    emit signalOpen(fileName);
}

void ImageEditor::slotSave()
{
    QString chosenExtension;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
        "", supportedExtensions,
        &chosenExtension);

    //QMessageBox::information(this, "ext", "chosen extension: " + chosenExtension);

    emit signalSave(fileName, chosenExtension);
}

