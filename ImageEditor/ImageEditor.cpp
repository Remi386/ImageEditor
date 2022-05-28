#include "ImageEditor.h"
#include "CentralWindow.h"

ImageEditor::ImageEditor(QWidget *parent)
    : QMainWindow(parent), mousePosition(new QLabel(this))
{
    QMenu* fileMenu = menuBar()->addMenu("&File");

    bool openActs = true;
    QAction* open = new QAction("&Open file");
    open->setShortcut(Qt::CTRL + Qt::Key_A);
    openActs &= (bool)connect(open, &QAction::triggered, this, &ImageEditor::slotOpen);
    
    QAction* save = new QAction("&Save");
    save->setShortcut(Qt::CTRL + Qt::Key_S);
    openActs &= (bool)connect(save, &QAction::triggered, this, &ImageEditor::slotSave);

    QAction* about = new QAction("&About ImageEditor");
    openActs &= (bool)connect(about, &QAction::triggered, this, &ImageEditor::slotAbout);

    Q_ASSERT(openActs);

    fileMenu->addAction(open);
    fileMenu->addSeparator();
    fileMenu->addAction(save);
    fileMenu->addSeparator();
    fileMenu->addAction(about);
    fileMenu->addSeparator();
    fileMenu->addAction("&Exit", qApp, SLOT(quit()));
    
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    bool editActs = true;

    undo = new QAction("&Undo");
    undo->setShortcut(Qt::CTRL + Qt::Key_Z);
    editActs &= (bool)connect(undo, &QAction::triggered, this, &ImageEditor::signalUndo);
    undo->setEnabled(false);

    redo = new QAction("&Redo");
    redo->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);
    editActs &= (bool)connect(redo, &QAction::triggered, this, &ImageEditor::signalRedo);
    redo->setEnabled(false);

    Q_ASSERT(editActs);

    editMenu->addAction(undo);
    fileMenu->addSeparator();
    editMenu->addAction(redo);
    
    CentralWindow* centralWindow = new CentralWindow;
    
    bool centWind = true;

    centWind &= (bool)connect(this, &ImageEditor::signalOpen,
                              centralWindow, &CentralWindow::slotOpenFile);

    centWind &= (bool)connect(this, &ImageEditor::signalSave,
                              centralWindow, &CentralWindow::slotSaveAs);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalMouseMoved,
                              this, &ImageEditor::slotMouseMoved);

    centWind &= (bool)connect(this, &ImageEditor::signalUndo,
                              centralWindow, &CentralWindow::slotUndo);

    centWind &= (bool)connect(this, &ImageEditor::signalRedo,
                              centralWindow, &CentralWindow::slotRedo);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalRedoStatus, 
                              this, &ImageEditor::slotStatusRedoChanged);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalUndoStatus,
                              this, &ImageEditor::slotStatusUndoChanged);

    Q_ASSERT(centWind);

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

void ImageEditor::slotStatusUndoChanged(bool status)
{
    undo->setEnabled(status);
}

void ImageEditor::slotStatusRedoChanged(bool status)
{
    redo->setEnabled(status);
}