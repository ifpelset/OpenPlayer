#ifndef LRCDISPLAY_H
#define LRCDISPLAY_H

#include <QLabel>
#include <QMap>

class LrcDisplay : public QLabel
{
    Q_OBJECT
public:
    LrcDisplay(const QString &fileName, QWidget *parent = 0);

    void setLrcFile(const QString &fileName);
    void initLrc(const QString &fileName);
    void getHeaderInfo(const char *str, QString &des);
    void getContentByTime(const QString &line);
    QMap<int, QString> getMapLrc();

    void startLrcMask(qint64 intervalTime);
    void pauseAndRecoverLrcMask(qint64 intervalTime);
    void stopLrcMask();

signals:

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
//    void contextMenuEvent(QContextMenuEvent *ev);

    void paintEvent(QPaintEvent *);

public slots:
    void showLrc(const int time);
    void timeout();

private:
    QMap<int, QString> m_mapLrc;
    QPoint m_dragPosition;

    QTimer *timer;
    QLinearGradient linearGradient;
    QLinearGradient maskLinearGradient;
    QFont font;
    qreal lrcMaskWidth;

    qreal lrcMaskWidthInterval;
    QPoint offset;

};

#endif // LRCDISPLAY_H
