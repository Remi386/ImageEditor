#include "CentralWindow.h"
#include "ActiveArea.h"
#include "Pencil.h"
#include "Brush.h"
#include "Eraser.h"
#include "Spray.h"
#include "Line.h"
#include "Dropper.h"
#include "Flood.h"

CentralWindow::CentralWindow(QWidget* parent /* = Q_NULLPTR*/) 
	:QWidget(parent), tabWidget(new QTabWidget),
	currentInstrument(new Pencil)//Pencil by default
{
	createActions();

	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->setContentsMargins(1, 1, 1, 1);

	//tool bar
	QToolBar* toolBar = new QToolBar;
	toolBar->setOrientation(Qt::Vertical);

	toolBar->addAction(choosePen);
	toolBar->addSeparator();

	toolBar->addAction(chooseBrush);
	toolBar->addSeparator();

	toolBar->addAction(chooseEraser);
	toolBar->addSeparator();

	toolBar->addAction(chooseDropper);
	toolBar->addSeparator();

	toolBar->addAction(chooseFlood);
	toolBar->addSeparator();

	toolBar->addAction(chooseSpray);
	toolBar->addSeparator();

	toolBar->addAction(chooseLine);
	toolBar->addSeparator();

	toolBar->addAction(chooseColor);

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
	toolBar->addWidget(penWidthSlider);
	toolBar->addWidget(penSizeDisplay);

	color1 = new ColorWidget("Color 1", Qt::black);
	color2 = new ColorWidget("Color 2", Qt::white);

	color1->setActive(true);

	bool colorOk = true;

	colorOk &= (bool)connect(color1, &ColorWidget::signalActiveColorChanged,
						     color2, &ColorWidget::slotActiveColorChanged);

	colorOk &= (bool)connect(color2, &ColorWidget::signalActiveColorChanged,
							 color1, &ColorWidget::slotActiveColorChanged);

	colorOk &= (bool)connect(color1, &ColorWidget::signalActiveColorChanged,
							 this, &CentralWindow::slotColorChanged);

	colorOk &= (bool)connect(color2, &ColorWidget::signalActiveColorChanged,
						     this, &CentralWindow::slotColorChanged);
	
	Q_ASSERT(colorOk);

	toolBar->addWidget(color1);
	toolBar->addWidget(color2);

	hLayout->addWidget(toolBar);

	ActiveArea* area = new ActiveArea(arguments);
	createNewTab(area);
	
	tabWidget->setTabsClosable(true);
	hLayout->addWidget(tabWidget);

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

	QIcon FloodIcon("icons/icon-flood.png");
	chooseFlood = new QAction(FloodIcon, tr("Fill with color"));

	QIcon SprayIcon("icons/icon-spray.png");
	chooseSpray = new QAction(SprayIcon, tr("Spray"));

	QIcon LineIcon("icons/icon-line.png");
	chooseLine = new QAction(LineIcon, tr("Line"));

	QIcon PaletteIcon("icons/icon-color-palette.png");
	chooseColor = new QAction(PaletteIcon, tr("Choose color"));
}

void CentralWindow::connectActiveArea(ActiveArea* area)
{
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

	areaCon &= (bool)connect(area, &ActiveArea::signalColorChanged,
							 this, &CentralWindow::slotColorChanged);

	Q_ASSERT(areaCon);
}

void CentralWindow::connectSignals()
{
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

	actB &= (bool)connect(chooseFlood, &QAction::triggered,
						  this, [this]() {changeInstrument(new Flood); });

	actB &= (bool)connect(chooseColor, &QAction::triggered, 
						  this, [this]() { slotColorChanged(QColorDialog::getColor()); });

	actB &= (bool)connect(tabWidget, &QTabWidget::currentChanged,
						  this, &CentralWindow::slotCurrentWidgetChanged);

	actB &= (bool)connect(tabWidget, &QTabWidget::tabCloseRequested,
						  this, &CentralWindow::slotCloseRequest);

	Q_ASSERT(actB);
}

void CentralWindow::createNewTab(ActiveArea* actArea)
{
	connectActiveArea(actArea);
	QScrollArea* scrArea = new QScrollArea;
	scrArea->setWidget(actArea);
	scrArea->setAlignment(Qt::AlignCenter);

	int index = tabWidget->addTab(scrArea, actArea->GetImageName());
	tabWidget->setCurrentIndex(index);
}

void CentralWindow::slotNewFile()
{
	ActiveArea* actArea = new ActiveArea(arguments);
	createNewTab(actArea);
}

void CentralWindow::slotSave()
{
	ActiveArea* area = GetActiveArea();
	if (!area->Save()) {
		QMessageBox::warning(this,
			tr("Warning"), tr("Unable to save a file: ") + area->GetPath()
		);
	}
	tabWidget->setTabText(tabWidget->currentIndex(), area->GetImageName());
}

void CentralWindow::slotSaveAs()
{
	ActiveArea* area = GetActiveArea();
	if (!area->SaveAs()) {
		QMessageBox::warning(this,
			tr("Warning"), tr("Unable to save a file: ") + area->GetPath()
		);
	}
	tabWidget->setTabText(tabWidget->currentIndex(), area->GetImageName());
}

void CentralWindow::slotOpenFile(const QString& fileName)
{
	ActiveArea* actArea = new ActiveArea(arguments, ActiveArea::Nothing);

	if (!actArea->Open(fileName)) {
		QMessageBox::warning(this, tr("Warning"), tr("Unable to open a file: ") + fileName);
		delete actArea;
		return;
	}

	createNewTab(actArea);
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

void CentralWindow::slotColorChanged(const QColor& newColor)
{
	activeColor = newColor;
	activeColor.getRgb(&arguments["red"], &arguments["green"], &arguments["blue"]);

	//updating color widget in case color changed by dropper
	if (color1->isActive())
		color1->setColor(activeColor);
	else
		color2->setColor(activeColor);
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

ActiveArea* CentralWindow::GetActiveArea(int index /*= -1*/)
{
	QScrollArea* scrArea;
	if(index == -1)
		scrArea = qobject_cast<QScrollArea*>(tabWidget->currentWidget());
	else
		scrArea = qobject_cast<QScrollArea*>(tabWidget->widget(index));
	
	return qobject_cast<ActiveArea*>(scrArea->widget());
}

void CentralWindow::slotCloseRequest(int index)
{
	ActiveArea* area = GetActiveArea(index);

	if (area->CloseArea()) {
		if (tabWidget->count() == 1) {
			ActiveArea* newArea = new ActiveArea(arguments, 
												 ActiveArea::CreateNewImage | 
												 ActiveArea::ResetCounter);
			createNewTab(newArea);
		}

		tabWidget->removeTab(index);
	}
}

void CentralWindow::slotCurrentWidgetChanged()
{
	ActiveArea* area = GetActiveArea();
	auto [isPossibleToRedo, isPossibleToUndo] = area->GetRedoUndoStatus();
	emit signalRedoStatus(isPossibleToRedo);
	emit signalUndoStatus(isPossibleToUndo);
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