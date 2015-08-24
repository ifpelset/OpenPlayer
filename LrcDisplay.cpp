#include "LrcDisplay.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

LrcDisplay::LrcDisplay(const QString &fileName, QWidget *parent) : QLabel(parent)
{
    setText("正在查找歌词...");
    initLrc(fileName);

    setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    resize(800, 60);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::OpenHandCursor);

    //歌词的线性渐变填充！
    linearGradient.setStart(0,10);
    linearGradient.setFinalStop(0,40);
    linearGradient.setColorAt(0.1,QColor(14,179,255));
    linearGradient.setColorAt(0.5,QColor(114,32,255));
    linearGradient.setColorAt(0.9,QColor(14,179,255));

    //遮罩的线性渐变填充！
    maskLinearGradient.setStart(0,10);
    maskLinearGradient.setFinalStop(0,40);
    maskLinearGradient.setColorAt(0.1,QColor(222,54,4));
    maskLinearGradient.setColorAt(0.5,QColor(255,72,16));
    maskLinearGradient.setColorAt(0.9,QColor(222,54,4));

    //设置字体！
    //font.setFamily("Times New Roman");
    font.setBold(true);
    font.setPointSize(30);

    //设置定时器！
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;
}

void LrcDisplay::timeout()
{
    lrcMaskWidth += lrcMaskWidthInterval;
    update();
}

void LrcDisplay::initLrc(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return ;

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString s = stream.readLine();

        if (s.length() < 4)
            continue;

        if (s.startsWith("[ti:")) {
            getHeaderInfo("[ti:", s);
            m_mapLrc.insert(0, s);
            continue;
        } else if (s.startsWith("[ar:")) {
            getHeaderInfo("[ar:", s);
            m_mapLrc.insert(1000, s);
            continue;
        } else if (s.startsWith("[al:")) {
            getHeaderInfo("[al:", s);
            m_mapLrc.insert(2000, s);
            continue;
        } else if (s.startsWith("[by:")) {
            getHeaderInfo("[by:", s);
            m_mapLrc.insert(3000, s);
            continue;
        } else {
            getContentByTime(s);
        }
    }

    file.close();
}

void LrcDisplay::getHeaderInfo(const char *str, QString &des)
{
    des = des.remove(str);
    des = des.remove("]");
}

void LrcDisplay::getContentByTime(const QString &line)
{
    if (line.startsWith("[")) {
        int idx = line.indexOf("]");
        QString strTmp = line.mid(1, idx - 1);
        QString strMin = strTmp.mid(0, 2);
        QString strSec = strTmp.mid(3, 2);

        int iMin = strMin.toInt();
        int iSec = strSec.toInt();

        int iTime = iMin * 60 /* 1000*/ + iSec /* 1000*/;
        strTmp = line.mid(idx + 1);
        idx = strTmp.lastIndexOf("]");

        if (idx < 0) {
            m_mapLrc.insert(iTime, strTmp);
        } else {
            strTmp = strTmp.mid(idx + 1);
            m_mapLrc.insert(iTime, strTmp);
            getContentByTime(strTmp);
        }
    }
}

QMap<int, QString> LrcDisplay::getMapLrc()
{
    return m_mapLrc;
}

void LrcDisplay::startLrcMask(qint64 intervalTime)
{
    //此处设置没30毫秒刷新下遮罩宽度，更新太快会增加CPU负担，太慢则动画效果不流畅！
    qreal count = intervalTime / 30;
    //获取每次遮罩需要增加的宽度，此处的800为部件的固定宽度！
    lrcMaskWidthInterval = 800 / count;
    lrcMaskWidth = 0;
    timer->start(30);
}

void LrcDisplay::stopLrcMask()
{
    timer->stop();
    lrcMaskWidth = 0;
    update();
}

void LrcDisplay::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        m_dragPosition = ev->globalPos() - frameGeometry().topLeft();
        ev->accept();
    }
    QLabel::mousePressEvent(ev);
}

void LrcDisplay::mouseMoveEvent(QMouseEvent *ev)
{
//    if (ev->button() & Qt::LeftButton) {
        move(ev->globalPos() - m_dragPosition);
        ev->accept();
//    }

    QLabel::mouseMoveEvent(ev);
}

void LrcDisplay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(font);

    //先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感！
    painter.setPen(QColor(0,0,0,200));
    painter.drawText(1,1,800,60,Qt::AlignLeft,text());

    //再在上面绘制渐变文字~！
    painter.setPen(QPen(linearGradient,0));
    painter.drawText(0,0,800,60,Qt::AlignLeft,text());

    //设置歌词遮罩~！
    painter.setPen(QPen(maskLinearGradient,0));
    painter.drawText(0,0,lrcMaskWidth,60,Qt::AlignLeft,text());
}

void LrcDisplay::showLrc(const int time)
{
    if (!m_mapLrc.value(time).isNull())
        setText(m_mapLrc.value(time));
}

void LrcDisplay::setLrcFile(const QString &fileName)
{
    m_mapLrc.clear();
    setText("正在查找歌词...");
    initLrc(fileName);
}

