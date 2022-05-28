#include "CentralWindow.h"
#include "ActiveArea.h"
#include "Pencil.h"
#include "Brush.h"
#include "Eraser.h"
#include "Spray.h"
#include "Line.h"
#include "Dropper.h"

CentralWindow::CentralWindow(QWidget* parent /* = Q_NULLPTR*/) 
	:QWidget(parent), activeArea(new QScrollArea),
	currentInstrument(new Pencil)//Pencil by default
{
	createActions();

	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->setContentsMargins(1, 1, 1, 1);

	//tool bar
	QToolBar* rigthBar = new QToolBar;
	rigthBar->setOrientation(Qt::Vertical);

	rigthBar->addAction(choosePen);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseBrush);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseEraser);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseDropper);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseHand);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseRotate);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseSpray);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseLine);
	rigthBar->addSeparator();

	rigthBar->addAction(chooseColor);

	QSlider* penWidthSlider = new QSlider(Qt::Horizontal);
	penWidthSlider->setRange(1, 20);
	penWidthSlider->setValue(1);
	
	QLabel* penSizeDisplay = new QLabel(QString::number(penSize));
	penSizeDisplay->setAlignment(Qt::AlignCenter);

	bool penOk = true;
	penOk &= (bool)connect(penWidthSlider, &QSlider::valueChanged, 
						   this, &CentralWindow::slotPenSizeChanged);

	penOk &= (bool)connect(penWidthSlider, &QSlider::valueChanged, 
						   penSizeDisplay, qOverload<int>(&QLabel::setNum));

	Q_ASSERT(penOk);
	rigthBar->addWidget(penWidthSlider);
	rigthBar->addWidget(penSizeDisplay);

	hLayout->addWidget(rigthBar);

	ActiveArea* area = new ActiveArea(arguments);

	activeArea->setWidget(area);
	activeArea->setAlignment(Qt::AlignCenter);

	hLayout->addWidget(activeArea);

	arguments["penSize"] = penSize;
	activeColor.getRgb(&arguments["red"], &arguments["green"], &arguments["blue"]);

	connectSignals();

	setLayout(hLayout);
}

CentralWindow::~CentralWindow()
{
	delete currentInstrument;
}

void CentralWindow::createActions()
{
	QIcon PencilIcon("icons/icon-pencil.png");
	choosePen = new QAction(PencilIcon, tr("Pencil"));

	QIcon BrushIcon("icons/icon-brush.png");
	chooseBrush = new QAction(BrushIcon, tr("Brush"));

	QIcon EraserIcon("icons/icon-eraser.png");
	chooseEraser = new QAction(EraserIcon, tr("Eraser"));

	QIcon DropperIcon("icons/icon-dropper.png");
	chooseDropper = new QAction(DropperIcon, tr("Dropper"));

	QIcon HandIcon("icons/icon-hand.png");
	chooseHand = new QAction(HandIcon, tr("Hand"));

	QIcon RotateIcon("icons/icon-rotate.png");
	chooseRotate = new QAction(RotateIcon, tr("Rotate"));

	QIcon SprayIcon("icons/icon-spray.png");
	chooseSpray = new QAction(SprayIcon, tr("Spray"));

	QIcon LineIcon("icons/icon-line.png");
	chooseLine = new QAction(LineIcon, tr("Line"));

	QIcon PaletteIcon("icons/icon-color-palette.png");
	chooseColor = new QAction(PaletteIcon, tr("Choose color"));
}

void CentralWindow::connectSignals()
{
	ActiveArea* area = GetActiveArea();

	bool areaCon = true;

	areaCon &= (bool)connect(area, &ActiveArea::signalMousePressed,
							 this, &CentralWindow::slotMousePressed);

	areaCon &= (bool)connect(area, &ActiveArea::signalMouseMoved,
							 this, &CentralWindow::slotMouseMoved);

	areaCon &= (bool)connect(area, &ActiveArea::signalMouseReleased,
							 this, &CentralWindow::slotMouseReleased);

	areaCon &= (bool)connect(area, &ActiveArea::signalRedoStatusChanged,
							 this, &CentralWindow::signalRedoStatus);

	areaCon &= (bool)connect(area, &ActiveArea::signalUndoStatusChanged,
							 this, &CentralWindow::signalUndoStatus);

	Q_ASSERT(areaCon);

	bool actB = true;
	actB &= (bool)connect(choosePen, &QAction::triggered, 
						  this, [this]() {changeInstrument(new Pencil); });

	actB &= (bool)connect(chooseBrush, &QAction::triggered, 
						  this, [this]() {changeInstrument(new Brush); });
	
	actB &= (bool)connect(chooseEraser, &QAction::triggered, 
						  this, [this]() {changeInstrument(new Eraser); });
	
	actB &= (bool)connect(chooseSpray, &QAction::triggered, 
						  this, [this]() {changeInstrument(new Spray); });
	
	actB &= (bool)connect(chooseLine, &QAction::triggered, 
						  this, [this]() {changeInstrument(new Line); });
	
	actB &= (bool)connect(chooseDropper, &QAction::triggered, 
						  this, [this]() {changeInstrument(new Dropper); });

	actB &= (bool)connect(chooseColor, &QAction::triggered, 
						  this, [this]() { slotColorChanged(QColorDialog::getColor()); });

	Q_ASSERT(actB);
}

void CentralWindow::slotSaveAs(const QString& fileName, const QString& extension)
{
	if (!GetActiveArea()->SaveAs(fileName, extension)) {
		QMessageBox::warning(this,
			tr("Warning"), tr("Unable to save a file: ") + fileName + "." + extension
		);
	}
}

void CentralWindow::slotOpenFile(const QString& fileName)
{
	if (!GetActiveArea()->Open(fileName)) {
		QMessageBox::warning(this, tr("Warning"), tr("Unable to open a file: ") + fileName);
	}
}

void CentralWindow::slotMousePressed()
{
	isMousePressed = true;
	GetActiveArea()->Draw(currentInstrument, OperationType::Press);
}

void CentralWindow::slotMouseMoved(QPoint pos)
{
	emit signalMouseMoved(pos);

	if (isMousePressed)
		GetActiveArea()->Draw(currentInstrument, OperationType::Move);
}

void CentralWindow::slotMouseReleased()
{
	isMousePressed = false;
	GetActiveArea()->Draw(currentInstrument, OperationType::Release);
}

//void CentralWindow::slotScaleFactorChanged(double scaleFactor)
//{
//	ActiveArea* area = GetActiveArea();
//	area->resize(area->size() * scaleFactor);
//	scaleScrollBar(activeArea->horizontalScrollBar(), scaleFactor);
//	scaleScrollBar(activeArea->verticalScrollBar(), scaleFactor);
//}

//void CentralWindow::scaleScrollBar(QScrollBar* scrollBar, double scaleFactor)
//{
//	scrollBar->setValue(int(scaleFactor * scrollBar->value()
//		+ ((scaleFactor - 1) * scrollBar->pageStep() / 2)));
//}

void CentralWindow::slotColorChanged(const QColor& newColor)
{
	activeColor = newColor;
	activeColor.getRgb(&arguments["red"], &arguments["green"], &arguments["blue"]);
}

void CentralWindow::slotPenSizeChanged(int newSize)
{
	penSize = newSize;
	arguments["penSize"] = penSize;
}

void CentralWindow::changeInstrument(Instrument* newInst)
{
	if (newInst) {
		delete currentInstrument;
		currentInstrument = newInst;
	}
}

ActiveArea* CentralWindow::GetActiveArea()
{
	return qobject_cast<ActiveArea*>(activeArea->widget());
}

void CentralWindow::slotUndo()
{
	if (!GetActiveArea()->Undo()) {
		emit signalUndoStatus(false);
	}
	emit signalRedoStatus(true);
}

void CentralWindow::slotRedo()
{
	if (!GetActiveArea()->Redo()) {
		emit signalRedoStatus(false);
	}
	emit signalUndoStatus(true);
}