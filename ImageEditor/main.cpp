#include "ImageEditor.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("appstyle.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    a.setStyleSheet(styleSheet);

    ImageEditor w;
    w.show();
    
    return a.exec();
}
