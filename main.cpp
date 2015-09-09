#include "MainWindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow wnd;

    QFile qssFile(QObject::tr(":/qss/myqss.qss"));
    qssFile.open(QFile::ReadOnly);
    QByteArray byteArray = qssFile.readAll() ;
    app.setStyleSheet(byteArray);
    qssFile.close();
    wnd.show();

    return app.exec();
}
