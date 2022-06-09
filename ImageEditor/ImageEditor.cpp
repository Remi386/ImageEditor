#include "ImageEditor.h"
#include "CentralWindow.h"

ImageEditor::ImageEditor(QWidget *parent)
    : QMainWindow(parent), mousePositionLabel(new QLabel(this))
{
    QMenu* fileMenu = menuBar()->addMenu("&File");

    bool fileActs = true;

    QAction* newFileAct = new QAction("&New");
    newFileAct->setShortcut(Qt::CTRL + Qt::Key_N);
    fileActs &= (bool)connect(newFileAct, &QAction::triggered, this, &ImageEditor::signalNew);

    QAction* open = new QAction("&Open file");
    open->setShortcut(Qt::CTRL + Qt::Key_A);
    fileActs &= (bool)connect(open, &QAction::triggered, this, &ImageEditor::signalOpen);

    QAction* save = new QAction("&Save");
    save->setShortcut(Qt::CTRL + Qt::Key_S);
    fileActs &= (bool)connect(save, &QAction::triggered, this, &ImageEditor::signalSave);

    QAction* saveAs = new QAction("Save &as");
    saveAs->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    fileActs &= (bool)connect(saveAs, &QAction::triggered, this, &ImageEditor::signalSaveAs);

    QAction* about = new QAction("&About ImageEditor");
    fileActs &= (bool)connect(about, &QAction::triggered, this, &ImageEditor::slotAbout);

    QAction* exit = new QAction("&Exit");
    fileActs &= (bool)connect(exit, &QAction::triggered, this, [this]() { close(); });

    Q_ASSERT(fileActs);

    fileMenu->addAction(newFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(open);
    fileMenu->addSeparator();
    fileMenu->addAction(save);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(about);
    fileMenu->addSeparator();
    fileMenu->addAction(exit);

    QMenu* editMenu = menuBar()->addMenu("&Edit");
    bool editActs = true;

    QAction* undo = new QAction("&Undo");
    undo->setShortcut(Qt::CTRL + Qt::Key_Z);
    editActs &= (bool)connect(undo, &QAction::triggered, this, &ImageEditor::signalUndo);
    undo->setEnabled(false);

    QAction* redo = new QAction("&Redo");
    redo->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);
    editActs &= (bool)connect(redo, &QAction::triggered, this, &ImageEditor::signalRedo);
    redo->setEnabled(false);

    QAction* swapActiveColor = new QAction("&Swap colors");
    swapActiveColor->setShortcut(Qt::Key_X);

    QAction* incPenSize = new QAction("&Increase pen size");
    incPenSize->setShortcut(Qt::CTRL + Qt::Key_Period);
    editActs &= (bool)connect(incPenSize, &QAction::triggered, 
                              this, &ImageEditor::signalIncreasePenSize);

    QAction* decPenSize = new QAction("&Decrease pen size");
    decPenSize->setEnabled(false);
    decPenSize->setShortcut(Qt::CTRL + Qt::Key_Comma);
    editActs &= (bool)connect(decPenSize, &QAction::triggered,
                              this, &ImageEditor::signalDecreasePenSize);

    Q_ASSERT(editActs);

    editMenu->addAction(undo);
    editMenu->addSeparator();
    editMenu->addAction(redo);
    editMenu->addSeparator();
    editMenu->addAction(swapActiveColor);
    editMenu->addSeparator();
    editMenu->addAction(incPenSize);
    editMenu->addSeparator();
    editMenu->addAction(decPenSize);

    CentralWindow* centralWindow = new CentralWindow;

    bool centWind = true;

    centWind &= (bool)connect(this, &ImageEditor::signalOpen,
                              centralWindow, &CentralWindow::slotOpenFile);

    centWind &= (bool)connect(this, &ImageEditor::signalSave,
                              centralWindow, &CentralWindow::slotSave);

    centWind &= (bool)connect(this, &ImageEditor::signalSaveAs,
                              centralWindow, &CentralWindow::slotSaveAs);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalMouseMoved,
                              this, &ImageEditor::slotMouseMoved);

    centWind &= (bool)connect(this, &ImageEditor::signalUndo,
                              centralWindow, &CentralWindow::slotUndo);

    centWind &= (bool)connect(this, &ImageEditor::signalRedo,
                              centralWindow, &CentralWindow::slotRedo);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalRedoStatus,
                              redo, &QAction::setEnabled);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalUndoStatus,
                              undo, &QAction::setEnabled);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalMouseLeaved,
                              this, &ImageEditor::slotMouseLeaved);

    centWind &= (bool)connect(this, &ImageEditor::signalNew,
                              centralWindow, &CentralWindow::slotNewFile);

    centWind &= (bool)connect(swapActiveColor, &QAction::triggered,
                              centralWindow, &CentralWindow::slotSwapColors);

    centWind &= (bool)connect(this, &ImageEditor::signalIncreasePenSize,
                              centralWindow, &CentralWindow::slotIncreasePenSize);

    centWind &= (bool)connect(this, &ImageEditor::signalDecreasePenSize,
                              centralWindow, &CentralWindow::slotDecreasePenSize);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalIncPenStatus,
                              incPenSize, &QAction::setEnabled);

    centWind &= (bool)connect(centralWindow, &CentralWindow::signalDecPenStatus,
                              decPenSize, &QAction::setEnabled);

    Q_ASSERT(centWind);

    setCentralWidget(centralWindow);

    QStatusBar* statBar = statusBar();
    statBar->addWidget(mousePositionLabel);

    resize(500, 400);
}

bool ImageEditor::CloseWindow()
{
    CentralWindow* wind = qobject_cast<CentralWindow*>(centralWidget());
    return wind->tryToClose();
}

void ImageEditor::closeEvent(QCloseEvent* ce)
{
    if (CloseWindow())
        ce->accept();
    else
        ce->ignore();
}

void ImageEditor::slotMouseMoved(QPoint mousePos)
{
    mousePositionLabel->setText("X = " + QString::number(mousePos.x()) +
        " , Y = " + QString::number(mousePos.y()));
}

void ImageEditor::slotMouseLeaved()
{
    mousePositionLabel->setText("");
}

void ImageEditor::slotAbout()
{
    QMessageBox::about(this, tr("About Image Editor"),
        tr("This application was created for educational purposes <br>"
        "You can find the code here: "
        "<a href = \"https://github.com/Remi386/ImageEditor\">Github repository</a> <br>"
        "Tab icons are taken from: <a href = \"https://www.flaticon.com/packs/photo-editing\""
        "title = \" Icon Pack: Photo editing | Lineal color\">Photo editing icons "
        "created by Freepik - Flaticon</a> <br>"
        "Cursor icons taken from: <a href = "
        "\"https://www.flaticon.com/packs/graphic-design-117?word=photo%20editing\">"
        "Icon Pack: Graphic Design | Flat, created by justicon - Flaticon</a>"));
}