#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow wnd;

    QFile qssFile(QObject::tr(":/qss/myqss.qss"));
    qssFile.open(QFile::ReadOnly);
    QByteArray byteArray = qssFile.readAll() ;
    qDebug() << byteArray;
    app.setStyleSheet(byteArray);
    qssFile.close();

//    //将字体文件名传给addApplicationFont,得到字体的Id
//    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/Monaco.ttf");
//    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
//    QString msyh = QFontDatabase::applicationFontFamilies ( fontId ).at(0);
//    QFont font(msyh,10);
//    //将此字体设为QApplication的默认字体
//    QApplication::setFont(font);

    wnd.show();

    return app.exec();
}
