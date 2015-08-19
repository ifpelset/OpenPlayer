#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QLabel;
class QProgressBar;
class QTimer;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    void initMainWindow();

signals:

public slots:
    void onClickBtnPlayPause();
    void onClickBtnPre();
    void onClickBtnNext();

    void setMetaData();
    void setProgressValue();

private:
    QPushButton *m_btnLoginStatus;
    QLabel *m_labelLoginStatus;

    QLabel *m_labelAlbumPic;
    QLabel *m_labelSongName;
    QLabel *m_labelSingerName;
    QLabel *m_labelAlbumName;
    QLabel *m_labelLrcArea;

    QPushButton *m_btnPre;
    QPushButton *m_btnPlayPause;
    QPushButton *m_btnNext;

    QLabel *m_labelPlayTime;
    QProgressBar *m_proPlay;

    QPushButton *m_btnPlayList;

    QTimer *m_timer;
};

#endif // MAINWINDOW_H
