#include "CentralWindow.h"
#include "ActiveArea.h"

CentralWindow::CentralWindow(QWidget* parent /* = Q_NULLPTR*/) 
	:QWidget(parent), area(new ActiveArea)
{
	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->setContentsMargins(1, 1, 1, 1);

	//tool bar
	QToolBar* toolBar = new QToolBar;
	toolBar->setOrientation(Qt::Vertical);

	QIcon PencilIcon("icons/icon-pencil.png");
	QAction* choosePen = new QAction(PencilIcon, tr("Pencil"));
	toolBar->addAction(choosePen);
	toolBar->addSeparator();

	QIcon BrushIcon("icons/icon-brush.png");
	QAction* chooseBrush = new QAction(BrushIcon, tr("Brush"));
	toolBar->addAction(chooseBrush);
	toolBar->addSeparator();

	QIcon EraserIcon("icons/icon-eraser.png");
	QAction* chooseEraser = new QAction(EraserIcon, tr("Eraser"));
	toolBar->addAction(chooseEraser);
	toolBar->addSeparator();

	QIcon DropperIcon("icons/icon-dropper.png");
	QAction* chooseDropper = new QAction(DropperIcon, tr("Dropper"));
	toolBar->addAction(chooseDropper);
	toolBar->addSeparator();

	QIcon HandIcon("icons/icon-hand.png");
	QAction* chooseHand = new QAction(HandIcon, tr("Hand"));
	toolBar->addAction(chooseHand);
	toolBar->addSeparator();

	QIcon RotateIcon("icons/icon-rotate.png");
	QAction* chooseRotate = new QAction(RotateIcon, tr("Rotate"));
	toolBar->addAction(chooseRotate);
	toolBar->addSeparator();

	QIcon SprayIcon("icons/icon-spray.png");
	QAction* chooseSpray = new QAction(SprayIcon, tr("Spray"));
	toolBar->addAction(chooseSpray);
	toolBar->addSeparator();

	QIcon LineIcon("icons/icon-line.png");
	QAction* chooseLine = new QAction(LineIcon, tr("Line"));
	toolBar->addAction(chooseLine);

	toolBar->setMovable(true);
	
	hLayout->addWidget(toolBar);

	QScrollArea* scrArea = new QScrollArea;
	scrArea->setWidget(area);
	scrArea->setAlignment(Qt::AlignCenter);

	hLayout->addWidget(scrArea);

	setLayout(hLayout);
	
}

void connectSignals()
{

}

void CentralWindow::slotSaveAs(const QString& fileName, const QString& extension)
{
	if (!area->SaveAs(fileName, extension)) {
		QMessageBox::warning(this, 
			"Warning", "Unable to save a file: " + fileName + "." + extension
		);
	}
}

void CentralWindow::slotOpenFile(const QString& fileName)
{
	if (!area->Open(fileName)) {
		QMessageBox::warning(this, "Warning", "Unable to open a file: " + fileName);
	}
}