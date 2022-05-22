#include "CentralWindow.h"
#include "ActiveArea.h"

CentralWindow::CentralWindow(QWidget* parent /* = Q_NULLPTR*/) 
	:QWidget(parent), area(new ActiveArea(arguments)), 
	currentInstrument(new Pencil)//Pencil by default
{
	createActions();
	connectSignals();

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

	/*
	firstColor = new QPushButton("Color 1");
	//firstColor->setColo
	QPalette pal;

	secondColor = new QPushButton("Color 2");

	rigthBar->addWidget(firstColor);
	rigthBar->addWidget(secondColor);
	*/
	hLayout->addWidget(rigthBar);

	QScrollArea* scrArea = new QScrollArea;
	scrArea->setWidget(area);
	scrArea->setAlignment(Qt::AlignCenter);

	hLayout->addWidget(scrArea);

	arguments["penSize"] = penSize;
	activeColor.getRgb(&arguments["red"], &arguments["green"], &arguments["blue"]);

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
	bool con = true;
	con &= (bool)connect(area, &ActiveArea::signalMousePressed, this, &CentralWindow::slotMousePressed);
	con &= (bool)connect(area, &ActiveArea::signalMouseMoved, this, &CentralWindow::slotMouseMoved);
	con &= (bool)connect(area, &ActiveArea::signalMouseReleased, this, &CentralWindow::slotMouseReleased);
	Q_ASSERT(con);

	bool actB = true;
	actB &= (bool)connect(choosePen, &QAction::triggered, this, []() {});
	actB &= (bool)connect(chooseColor, &QAction::triggered, this, [this]() {
		slotColorChanged(QColorDialog::getColor());
		});

	Q_ASSERT(actB);
}

void CentralWindow::slotSaveAs(const QString& fileName, const QString& extension)
{
	if (!area->SaveAs(fileName, extension)) {
		QMessageBox::warning(this, 
			tr("Warning"), tr("Unable to save a file: ") + fileName + "." + extension
		);
	}
}

void CentralWindow::slotOpenFile(const QString& fileName)
{
	if (!area->Open(fileName)) {
		QMessageBox::warning(this, tr("Warning"), tr("Unable to open a file: ") + fileName);
	}
}

void CentralWindow::slotMousePressed()
{
	isMousePressed = true;
	area->Draw(currentInstrument, OperationType::Press);
}

void CentralWindow::slotMouseMoved(QPoint pos)
{
	emit signalMouseMoved(pos);

	if (isMousePressed)
		area->Draw(currentInstrument, OperationType::Move);
}

void CentralWindow::slotMouseReleased()
{
	isMousePressed = false;
	area->Draw(currentInstrument, OperationType::Release);
}

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