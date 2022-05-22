#include "ImageEditor.h"
#include "CentralWindow.h"

ImageEditor::ImageEditor(QWidget *parent)
    : QMainWindow(parent), mousePosition(new QLabel(this))
{
    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* open = new QAction("&Open file");
    open->setShortcut(Qt::CTRL + Qt::Key_A);
    connect(open, SIGNAL(triggered()), this, SLOT(slotOpen()));
    
    QAction* save = new QAction("&Save");
    save->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(save, SIGNAL(triggered()), this, SLOT(slotSave()));

    QAction* about = new QAction("&About ImageEditor");
    connect(about, SIGNAL(triggered()), this, SLOT(slotAbout()));

    fileMenu->addAction(open);
    fileMenu->addSeparator();
    fileMenu->addAction(save);
    fileMenu->addSeparator();
    fileMenu->addAction(about);
    fileMenu->addSeparator();
    fileMenu->addAction("&Exit", qApp, SLOT(quit()));
    
    CentralWindow* centralWindow = new CentralWindow;
    
    bool bOk = true;

    bOk &= (bool)connect(this, &ImageEditor::signalOpen,
                         centralWindow, &CentralWindow::slotOpenFile);

    bOk &= (bool)connect(this, &ImageEditor::signalSave,
                         centralWindow, &CentralWindow::slotSaveAs);

    bOk &= (bool)connect(centralWindow, &CentralWindow::signalMouseMoved,
                         this, &ImageEditor::slotMouseMoved);


    Q_ASSERT(bOk);

    setCentralWidget(centralWindow);
    
    //Status bar
    QStatusBar* statBar = statusBar();
    statBar->addWidget(mousePosition);

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

void ImageEditor::slotMouseMoved(QPoint mousePos)
{
    mousePosition->setText("X = " + QString::number(mousePos.x()) +
        " , Y = " + QString::number(mousePos.y()));
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

    emit signalSave(fileName, chosenExtension);
}

void ImageEditor::slotAbout()
{
    QMessageBox::about(this, "About Image Editor",
        "This application was created for educational purposes <br>"
        "You can find the code here: "
        "<a href = \"https://github.com/Remi386/ImageEditor\">Github repository</a> <br>"
        "All icons are taken from: <a href = \"https://www.flaticon.com/packs/photo-editing\""
        "title = \" Icon Pack: Photo editing | Lineal color\">Photo editing icons "
        "created by Freepik - Flaticon</a>"
    );
}