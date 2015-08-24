#include "MainWindow.h"
#include "MyPlayer.h"
#include "MyPlayList.h"
#include "PlayListDlg.h"
#include "LrcDisplay.h"
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QDebug>
#include <QVariant>
#include <QMediaMetaData>
#include <QImage>
#include <QTime>
#include <QMouseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent, Qt::FramelessWindowHint), m_lrcDisplay(NULL)
{
    initMainWindow();

    QMediaPlayer *player = MyPlayer::getInstance();
    connect(player, SIGNAL(metaDataChanged()), this, SLOT(setMetaData()));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setSliderValue(qint64)));
}

void MainWindow::initMainWindow()
{
    QVBoxLayout *lytMainVBox = new QVBoxLayout(this);
    QHBoxLayout *lytTopHBox = new QHBoxLayout;
    QHBoxLayout *lytCenterHBox = new QHBoxLayout;
    QVBoxLayout *lytCenterRightVBox = new QVBoxLayout;
    QHBoxLayout *lytCenterRightHBox = new QHBoxLayout;
    QHBoxLayout *lytBottomHBox = new QHBoxLayout;

    m_btnLoginStatus = new QPushButton(this);
    m_labelLoginStatus = new QLabel(this);
    m_btnMinWindow = new QPushButton(this);
    m_btnCloseWindow = new QPushButton(this);
    lytTopHBox->addWidget(m_btnLoginStatus);
    lytTopHBox->addWidget(m_labelLoginStatus);
    lytTopHBox->addStretch(1);
    lytTopHBox->addWidget(m_btnMinWindow);
    lytTopHBox->addWidget(m_btnCloseWindow);

    m_btnLoginStatus->setIcon(QIcon(":/imgs/imgs/user-login.png"));
    m_btnLoginStatus->setIconSize(QSize(40, 40));
    m_btnLoginStatus->setToolTip("登录");
    m_btnLoginStatus->setObjectName("btnLoginStatus");

    m_btnMinWindow->setIcon(QIcon(":/imgs/imgs/minimize_window.png"));
    m_btnMinWindow->setIconSize(QSize(20, 20));
    m_btnMinWindow->setFlat(true);

    m_btnCloseWindow->setIcon(QIcon(":/imgs/imgs/close_window.png"));
    m_btnCloseWindow->setIconSize(QSize(20, 20));
    m_btnCloseWindow->setFlat(true);

    connect(m_btnMinWindow, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));
    connect(m_btnCloseWindow, SIGNAL(clicked(bool)), this, SLOT(close()));

    m_labelAlbumPic = new QLabel(this);
    m_labelSongName = new QLabel(this);
    m_labelSingerName = new QLabel(this);
    m_labelAlbumName = new QLabel(this);
    m_labelLrcArea = new QLabel(this);

    m_labelSongName->setStyleSheet("font-weight: bold; font-size: 20px; color: white;");
    m_labelSongName->adjustSize();
    m_labelSongName->setMaximumWidth(190);
    m_labelSongName->setWordWrap(true); // 373, 190
    m_labelSongName->setAlignment(Qt::AlignTop);

    m_labelSingerName->setStyleSheet("color: white;");
    m_labelAlbumName->setStyleSheet("color: white;");

    m_labelLrcArea->setStyleSheet("color: #16BC5C; font-size: 14px;");
    m_labelLrcArea->adjustSize();
    m_labelLrcArea->setMaximumWidth(190);
    m_labelLrcArea->setWordWrap(true); // 373, 190
    m_labelLrcArea->setAlignment(Qt::AlignTop);

    m_labelAlbumPic->setPixmap(QPixmap("C:/Users/IFPELSET/Desktop/getAlbumPicture/temp.jpeg").scaled(200, 200));
    m_labelAlbumPic->setStyleSheet("border: 7px solid rgba(40,44,48,0.2);");

    lytCenterRightVBox->addWidget(m_labelSongName);
    lytCenterRightHBox->addWidget(m_labelSingerName);
    lytCenterRightHBox->addWidget(m_labelAlbumName);
    lytCenterRightVBox->addLayout(lytCenterRightHBox);
    lytCenterRightVBox->addSpacing(35);
    lytCenterRightVBox->addWidget(m_labelLrcArea);
    lytCenterRightVBox->addSpacing(35);

    lytCenterHBox->addStretch(2);
//    lytCenterHBox->addSpacing(40);
    lytCenterHBox->addWidget(m_labelAlbumPic);
    lytCenterHBox->addStretch(1);
//    lytCenterHBox->addSpacing(20);
    lytCenterHBox->addLayout(lytCenterRightVBox);
    lytCenterHBox->addStretch(2);
//    lytCenterHBox->addSpacing(40);

    m_btnPre = new QPushButton(this);
    m_btnPlayPause = new QPushButton(this);
    m_btnNext = new QPushButton(this);
    m_sliderProPlay = new QSlider(this);
    m_labelDuration = new QLabel("00:00 / 00:00");
    m_btnPlayList = new QPushButton(this);

    connect(m_btnPre, SIGNAL(clicked(bool)), this, SLOT(onClickBtnPre()));
    connect(m_btnPlayPause, SIGNAL(clicked(bool)), this, SLOT(onClickBtnPlayPause()));
    connect(m_btnNext, SIGNAL(clicked(bool)), this, SLOT(onClickBtnNext()));
    connect(m_btnPlayList, SIGNAL(clicked(bool)), this, SLOT(showPlayListDlg()));

    m_sliderProPlay->setMinimum(0);
    m_sliderProPlay->setOrientation(Qt::Horizontal);
    m_sliderProPlay->setPageStep(1000); // 设置每次快进1s

    m_btnPre->setMinimumSize(30, 30);
    m_btnPre->setIcon(QIcon(":/imgs/imgs/gtk-media-next-rtl.png"));
    m_btnPre->setIconSize(QSize(25, 25));
    m_btnPre->setToolTip("上一曲");
    m_btnPre->setObjectName("btnPre");

    m_btnPlayPause->setMinimumSize(40, 40);
    m_btnPlayPause->setIcon(QIcon(":/imgs/imgs/gtk-media-play-ltr.png"));
    m_btnPlayPause->setIconSize(QSize(35, 35));
    m_btnPlayPause->setToolTip("播放");
    m_btnPlayPause->setObjectName("btnPlayPause");

    m_btnNext->setMinimumSize(30, 30);
    m_btnNext->setIcon(QIcon(":/imgs/imgs/gtk-media-next-ltr.png"));
    m_btnNext->setIconSize(QSize(25, 25));
    m_btnNext->setToolTip("下一曲");
    m_btnNext->setObjectName("btnNext");

    m_btnPlayList->resize(30, 30);
    m_btnPlayList->setIcon(QIcon(":/imgs/imgs/icon_menu-circle_alt2.png"));
    m_btnPlayList->setIconSize(QSize(32, 32));
    m_btnPlayList->setToolTip("播放列表");
    m_btnPlayList->setObjectName("btnPlayList");

    lytBottomHBox->addWidget(m_btnPre);
    lytBottomHBox->addWidget(m_btnPlayPause);
    lytBottomHBox->addWidget(m_btnNext);
    lytBottomHBox->addWidget(m_sliderProPlay);
    lytBottomHBox->addWidget(m_labelDuration);
    lytBottomHBox->addWidget(m_btnPlayList);

    lytMainVBox->addLayout(lytTopHBox);
    lytMainVBox->addStretch(1);
    lytMainVBox->addLayout(lytCenterHBox);
    lytMainVBox->addStretch(1);
    lytMainVBox->addLayout(lytBottomHBox);

    setMinimumSize(600, 360);
    setMaximumSize(600, 360);
    m_playListDlg = new PlayListDlg(this);
    m_playListDlg->hide();
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        dragPosition = ev->globalPos() - frameGeometry().topLeft();
        ev->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    move(ev->globalPos() - dragPosition);
    ev->accept();
}

void MainWindow::onClickBtnPlayPause()
{
    QMediaPlaylist *playlist = MyPlayList::getInstance();
    QMediaPlayer *player = MyPlayer::getInstance();

    if (player->state() == QMediaPlayer::StoppedState ||
            player->state() == QMediaPlayer::PausedState) {

        if (playlist->mediaCount() == 0) {
            QMessageBox::warning(this, "播放列表为空", "请添加歌曲到播放列表");
            return ;
        }

        m_btnPlayPause->setIcon(QIcon(":/imgs/imgs/gtk-media-pause.png"));
        m_btnPlayPause->setToolTip("暂停");
        player->play();
    } else {
        m_btnPlayPause->setIcon(QIcon(":/imgs/imgs/gtk-media-play-ltr.png"));
        m_btnPlayPause->setToolTip("播放");
        player->pause();
    }
}

void MainWindow::onClickBtnPre()
{
    QMediaPlaylist *playlist = MyPlayList::getInstance();
    if (playlist->currentIndex() == 0) {
        return ;
    }
    else
        playlist->previous();
}

void MainWindow::onClickBtnNext()
{
    QMediaPlaylist *playlist = MyPlayList::getInstance();
    if (playlist->nextIndex() == playlist->mediaCount()
            || playlist->nextIndex() == -1) {
        return ;
    } else {
        playlist->next();
    }  
}

void MainWindow::setMetaData()
{
    QMediaPlayer *player = MyPlayer::getInstance();

    m_labelSongName->setText(player->metaData(QMediaMetaData::Title).toString());
    m_labelSingerName->setText("<b>歌手:</b>" + player->metaData(QMediaMetaData::Author).toString());
    m_labelAlbumName->setText("<b>专辑:</b>" + player->metaData(QMediaMetaData::AlbumTitle).toString());

    // 获取了该音频文件的时间信息之后，立马设置slider的最大值
    m_sliderProPlay->setMaximum(player->metaData(QMediaMetaData::Duration).toInt());

    QMediaPlaylist *playlist = MyPlayList::getInstance();
    QMediaContent curMedia = playlist->currentMedia();
    QString audioFileName = curMedia.canonicalUrl().toLocalFile();
    QString lrcFileName = audioFileName.left(audioFileName.indexOf(".")) + ".lrc";

    if (m_lrcDisplay == NULL) {
        m_lrcDisplay = new LrcDisplay(lrcFileName);
        m_lrcDisplay->show();
    }
    else
        m_lrcDisplay->setLrcFile(lrcFileName);
}

void MainWindow::setSliderValue(qint64 currentInfo)
{
    QMediaPlayer *player = MyPlayer::getInstance();
    qint64 duration = player->metaData(QMediaMetaData::Duration).toInt();
    QString tStr;

    m_sliderProPlay->setValue(currentInfo);
    currentInfo /= 1000;
    duration /= 1000;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }

    m_labelDuration->setText(tStr);
    m_lrcDisplay->showLrc(currentInfo);

    QMap<int, QString> mapLrc = m_lrcDisplay->getMapLrc();
    if (!mapLrc.isEmpty()) {
        qint64 previous = 0;
        qint64 later = 0;

        foreach (qint64 value, mapLrc.keys()) {
            if (currentInfo >= value) {
                previous = value;
            } else {
                later = value;
                break;
            }
        }

        if (later == 0)
            later = duration;

        QString currentLrc = mapLrc.value(previous);
        if (currentLrc != m_labelLrcArea->text()) {
            qint64 intervalTime = later - previous;
            m_lrcDisplay->startLrcMask(intervalTime * 1000 + 2000);

            tStr = mapLrc.value(currentInfo);
            if (!tStr.isNull()) {
                m_labelLrcArea->setText(tStr);
            }
        }
    }
}

void MainWindow::showPlayListDlg()
{
    if (m_playListDlg->isHidden())
        m_playListDlg->show();
    else
        m_playListDlg->hide();
}

