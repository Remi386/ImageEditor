#pragma once

#include <QtWidgets/QWidget>
#include "ui_ImageEditor.h"

class ImageEditor : public QWidget
{
    Q_OBJECT

public:
    ImageEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::ImageEditorClass ui;
};
