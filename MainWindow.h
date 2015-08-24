#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QLabel;
class QSlider;
class PlayListDlg;
class LrcDisplay;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    void initMainWindow();
    QPushButton *m_btnPlayPause;

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

signals:

public slots:
    void onClickBtnPlayPause();
    void onClickBtnPre();
    void onClickBtnNext();

    void setMetaData();
    void setSliderValue(qint64);
    void showPlayListDlg();

private:
    QPushButton *m_btnLoginStatus;
    QLabel *m_labelLoginStatus;

    QLabel *m_labelAlbumPic;
    QLabel *m_labelSongName;
    QLabel *m_labelSingerName;
    QLabel *m_labelAlbumName;
    QLabel *m_labelLrcArea;

    QPushButton *m_btnPre;

    QPushButton *m_btnNext;

    QLabel *m_labelPlayTime;
    QSlider *m_sliderProPlay;
    QLabel *m_labelDuration;

    QPushButton *m_btnPlayList;

    PlayListDlg *m_playListDlg;

    QPushButton *m_btnMinWindow;
    QPushButton *m_btnCloseWindow;

    QPoint dragPosition;

    LrcDisplay *m_lrcDisplay;
};

#endif // MAINWINDOW_H
