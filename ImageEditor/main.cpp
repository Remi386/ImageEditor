#include "ImageEditor.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
//#include "ActiveArea.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageEditor w;
    w.show();
    /*
    ActiveArea* area = new ActiveArea;

    QScrollArea* scr = new QScrollArea;

    //scr->setWidgetResizable(false);
    scr->setWidget(area);
    scr->show();
    */
    return a.exec();
}
