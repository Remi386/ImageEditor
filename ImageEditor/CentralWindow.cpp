#include "CentralWindow.h"
#include "ActiveArea.h"
#include "ColorWidget.h"
#include "PenWidthSlider.h"
#include "PenWidthLineEdit.h"
#include "Pencil.h"
#include "Brush.h"
#include "Eraser.h"
#include "Spray.h"
#include "Line.h"
#include "Dropper.h"
#include "Flood.h"

constexpr int maxPenWidth = 20;

CentralWindow::CentralWindow(QWidget* parent /* = Q_NULLPTR*/) 
	:QWidget(parent), tabWidget(new QTabWidget)
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

	toolBar->setMinimumWidth(70);
	toolBar->setMaximumWidth(100);

	PenWidthSlider* pWidthSlider = new PenWidthSlider(Qt::Horizontal);
	pWidthSlider->setRange(1, maxPenWidth);
	pWidthSlider->setValue(1);

	pLineEdit = new PenWidthLineEdit(QString::number(penSize));
	pLineEdit->setAlignment(Qt::AlignCenter);
	pLineEdit->setValidator(new QIntValidator(1, maxPenWidth, this));
	
	bool penSizeDisp = true;
	penSizeDisp &= (bool)connect(pWidthSlider, &PenWidthSlider::valueChanged,
								 this, &CentralWindow::slotPenSizeChanged);

	penSizeDisp &= (bool)connect(pWidthSlider, &PenWidthSlider::valueChanged,
								 pLineEdit, &PenWidthLineEdit::setTextFromValue);

	penSizeDisp &= (bool)connect(pLineEdit, &PenWidthLineEdit::textChanged,
								 pWidthSlider, &PenWidthSlider::setValueFromString);


	Q_ASSERT(penSizeDisp);

	toolBar->addWidget(pWidthSlider);
	toolBar->addWidget(pLineEdit);

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

	createInstruments();
	connectSignals();
	getSupportedExtensions();

	changeInstrument("Pencil");

	setLayout(hLayout);
}

CentralWindow::~CentralWindow()
{
	for (auto iter = instruments.begin(); iter != instruments.end(); ++iter)
		delete* iter;	
}

bool CentralWindow::tryToClose()
{
	int tabCount = tabWidget->count();

	for (int i = 0; i < tabCount; ++i)
		if (closeTab(0)) {
			tabWidget->removeTab(0);
		} else {
			return false;
		}

	return true;
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

void CentralWindow::createInstruments()
{
	instruments["Pencil"] = new Pencil;
	instruments["Brush"] = new Brush;
	instruments["Eraser"] = new Eraser;
	instruments["Dropper"] = new Dropper;
	instruments["Spray"] = new Spray;
	instruments["Flood"] = new Flood;
	instruments["Line"] = new Line;

	cursors["Pencil"] = QPixmap("icons/cursor-pen.png");
	cursors["Brush"] = cursors["Pencil"];
	cursors["Eraser"] = QPixmap("icons/cursor-eraser.png");
	cursors["Spray"] = QPixmap("icons/cursor-spray.png");
	cursors["Dropper"] = QPixmap("icons/cursor-dropper.png");
	cursors["Flood"] = QPixmap("icons/cursor-flood.png");
	cursors["Line"] = cursors["Pencil"];
}

void CentralWindow::setCursor(ActiveArea* area, bool onMouseMove, bool forceSet/* = false*/)
{
	static QColor prevColor = Qt::black;
	static int cursorSize = 0;

	if (activeInstrument->isScalable())
	{
		//Variables for preventing unnecessary cursor copy
		//If forceSet = true, cursor will change in any way
		bool colorsSimular = forceSet;
		bool sizeDiffers = forceSet;

		if (colorDifference(area->getPixelColor(), prevColor) < ColorDifferenceValue * 10) {
			if (prevColor == Qt::black)
				prevColor = Qt::white;
			else
				prevColor = Qt::black;
		
			colorsSimular = true;
		}

		int newCursorSize = qMax(3, penSize);

		if (cursorSize != newCursorSize || colorsSimular) {
			cursorSize = newCursorSize;
			sizeDiffers = true;
		}

		if (colorsSimular || sizeDiffers) {
			QPixmap cursorCopy = activeCursor;

			if (colorsSimular) {
				cursorCopy.fill(prevColor);
				cursorCopy.setMask(activeCursor.mask());
			}

			if (sizeDiffers)
				cursorCopy = cursorCopy.scaled(cursorSize, cursorSize);

			area->setCursor(cursorCopy);
		}
	}
	else if (!onMouseMove) { 
		// cursor image is static, no need change it in mouse move event
		area->setCursor(activeCursor);
		cursorSize = 0;
	}
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

	areaCon &= (bool)connect(area, &ActiveArea::signalMouseLeaved,
							 this, &CentralWindow::signalMouseLeaved);

	Q_ASSERT(areaCon);
}

void CentralWindow::connectSignals()
{
	bool actB = true;

	actB &= (bool)connect(choosePen, &QAction::triggered, 
						  this, [this]() {changeInstrument("Pencil"); });

	actB &= (bool)connect(chooseBrush, &QAction::triggered, 
						  this, [this]() {changeInstrument("Brush"); });
	
	actB &= (bool)connect(chooseEraser, &QAction::triggered, 
						  this, [this]() {changeInstrument("Eraser"); });
	
	actB &= (bool)connect(chooseSpray, &QAction::triggered, 
						  this, [this]() {changeInstrument("Spray"); });
	
	actB &= (bool)connect(chooseLine, &QAction::triggered, 
						  this, [this]() {changeInstrument("Line"); });
	
	actB &= (bool)connect(chooseDropper, &QAction::triggered, 
						  this, [this]() {changeInstrument("Dropper"); });

	actB &= (bool)connect(chooseFlood, &QAction::triggered,
						  this, [this]() {changeInstrument("Flood"); });

	actB &= (bool)connect(chooseColor, &QAction::triggered, 
						  this, [this]() { slotColorChanged(QColorDialog::getColor(), true); });

	actB &= (bool)connect(tabWidget, &QTabWidget::currentChanged,
						  this, &CentralWindow::slotCurrentWidgetChanged);

	actB &= (bool)connect(tabWidget, &QTabWidget::tabCloseRequested,
						  this, &CentralWindow::slotCloseRequest);

	Q_ASSERT(actB);
}

void CentralWindow::getSupportedExtensions()
{
	supportedExtensionsOpen += "All image files (";
	foreach(const QByteArray & format, QImageWriter::supportedImageFormats()) {
		QString strFormat = QString::fromLatin1(format);
		supportedExtensionsSave += strFormat.toUpper() + QString(" (*.") + strFormat + QString(");;");
		supportedExtensionsOpen += "*." + strFormat + " ";
	};
	supportedExtensionsOpen += ");; All files (*)";

	//deleting two last characters for correct display
	int lastIndex = supportedExtensionsSave.size() - 1;
	supportedExtensionsSave[lastIndex] = supportedExtensionsSave[lastIndex - 1] = ' ';
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
	if (area->Save(supportedExtensionsSave)) {
		tabWidget->setTabText(tabWidget->currentIndex(), area->GetImageName());
	}
}

void CentralWindow::slotSaveAs()
{
	ActiveArea* area = GetActiveArea();
	if (area->SaveAs(supportedExtensionsSave)) {
		tabWidget->setTabText(tabWidget->currentIndex(), area->GetImageName());
	}
}

void CentralWindow::slotOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
													QDir::currentPath(), 
													supportedExtensionsOpen);

	if (fileName.isEmpty())
		return;

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
	GetActiveArea()->Draw(activeInstrument, OperationType::Press);
}

void CentralWindow::slotMouseMoved(QPoint pos)
{
	emit signalMouseMoved(pos);

	ActiveArea* area = GetActiveArea();
	
	setCursor(area, true);

	if (isMousePressed)
		area->Draw(activeInstrument, OperationType::Move);
}

void CentralWindow::slotMouseReleased()
{
	isMousePressed = false;
	GetActiveArea()->Draw(activeInstrument, OperationType::Release);
}

void CentralWindow::slotColorChanged(const QColor& newColor, bool shouldChangeColorWidget)
{
	activeColor = newColor;
	activeColor.getRgb(&arguments["red"], &arguments["green"], &arguments["blue"]);

	//updating color widget in case color changed by dropper
	if (shouldChangeColorWidget) {
		if (color1->isActive())
			color1->setColor(activeColor);
		else
			color2->setColor(activeColor);
	}
}

void CentralWindow::slotPenSizeChanged(int newSize)
{
	penSize = newSize;
	arguments["penSize"] = penSize;
	
	if (penSize == maxPenWidth) {
		isPosToIncPen = false;
		emit signalIncPenStatus(false);
	}
	else if(penSize == 1) {
		isPosToDecPen = false;
		emit signalDecPenStatus(false);
	}
	else if (!isPosToIncPen) {
		isPosToIncPen = true;
		emit signalIncPenStatus(true);
	}
	else if (!isPosToDecPen) {
		isPosToDecPen = true;
		emit signalDecPenStatus(true);
	}

	setCursor(GetActiveArea(), false, true);
}

void CentralWindow::slotIncreasePenSize()
{
	if (isPosToIncPen) {
		int increasedSize = penSize + 1;
		slotPenSizeChanged(increasedSize);
		pLineEdit->setTextFromValue(increasedSize);
	}
}

void CentralWindow::slotDecreasePenSize()
{
	if (isPosToDecPen) {
		int decreasedSize = penSize - 1;
		slotPenSizeChanged(decreasedSize);
		pLineEdit->setTextFromValue(decreasedSize);
	}
}

void CentralWindow::changeInstrument(const QString& instName)
{
	if (instruments.contains(instName) && cursors.contains(instName)) {
		activeCursor = cursors[instName];
		activeInstrument = instruments[instName];
		setCursor(GetActiveArea(), false, true);
	}
	else
		QMessageBox::critical(this, "ImageEditor", 
							  "Error! Unknown instrument name: " + instName);
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

bool CentralWindow::closeTab(int index)
{
	return GetActiveArea(index)->CloseArea(supportedExtensionsSave);
}

void CentralWindow::slotCloseRequest(int index)
{
	if (closeTab(index)) {
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
	setCursor(area, false, true);
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

void CentralWindow::slotSwapColors()
{
	if (color1->isActive()) {
		color1->setActive(false);
		color2->setActive(true);
		slotColorChanged(color2->getColor(), false);
	}
	else {
		color1->setActive(true);
		color2->setActive(false);
		slotColorChanged(color1->getColor(), false);
	}
}