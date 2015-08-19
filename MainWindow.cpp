#include "MainWindow.h"
#include "MyPlayer.h"
#include "MyPlayList.h"
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QDebug>
#include <QVariant>
#include <QMediaMetaData>
#include <QImage>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    initMainWindow();

    QMediaPlayer *player = MyPlayer::getInstance();
    connect(player, SIGNAL(metaDataChanged()), this, SLOT(setMetaData()));
    m_timer = new QTimer;
    m_timer->setInterval(100);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(setProgressValue()));
}

void MainWindow::initMainWindow()
{
    QVBoxLayout *lytMainVBox = new QVBoxLayout(this);
    QHBoxLayout *lytTopHBox = new QHBoxLayout;
    QHBoxLayout *lytCenterHBox = new QHBoxLayout;
    QVBoxLayout *lytCenterRightVBox = new QVBoxLayout;
    QHBoxLayout *lytCenterRightHBox = new QHBoxLayout;
    QHBoxLayout *lytBottomHBox = new QHBoxLayout;

    m_btnLoginStatus = new QPushButton;
    m_labelLoginStatus = new QLabel("点击登录");
    lytTopHBox->addWidget(m_btnLoginStatus);
    lytTopHBox->addWidget(m_labelLoginStatus);
    lytTopHBox->addStretch(1);

    m_labelAlbumPic = new QLabel("--专辑图片");
    m_labelAlbumPic->setFrameShape(QFrame::Panel);
    m_labelSongName = new QLabel("歌曲名:xxxx");
    m_labelSingerName = new QLabel("歌手:xxx");
    m_labelAlbumName = new QLabel("专辑:xxx");
    m_labelLrcArea = new QLabel("暂无歌词");

    lytCenterRightVBox->addWidget(m_labelSongName);
    lytCenterRightHBox->addWidget(m_labelSingerName);
    lytCenterRightHBox->addWidget(m_labelAlbumName);
    lytCenterRightVBox->addLayout(lytCenterRightHBox);
    lytCenterRightVBox->addWidget(m_labelLrcArea);

    lytCenterHBox->addStretch(2);
    lytCenterHBox->addWidget(m_labelAlbumPic);
    lytCenterHBox->addStretch(1);
    lytCenterHBox->addLayout(lytCenterRightVBox);
    lytCenterHBox->addStretch(2);

    m_btnPre = new QPushButton("上一曲");
    m_btnPlayPause = new QPushButton("播放");
    m_btnNext = new QPushButton("下一曲");
    m_proPlay = new QProgressBar;
    m_btnPlayList = new QPushButton("播放列表");

    connect(m_btnPre, SIGNAL(clicked(bool)), this, SLOT(onClickBtnPre()));
    connect(m_btnPlayPause, SIGNAL(clicked(bool)), this, SLOT(onClickBtnPlayPause()));
    connect(m_btnNext, SIGNAL(clicked(bool)), this, SLOT(onClickBtnNext()));
    m_proPlay->setMaximum(1);
    m_proPlay->setMinimum(0);
    //connect(m_proPlay, SIGNAL())


    lytBottomHBox->addWidget(m_btnPre);
    lytBottomHBox->addWidget(m_btnPlayPause);
    lytBottomHBox->addWidget(m_btnNext);
    lytBottomHBox->addWidget(m_proPlay);
    lytBottomHBox->addWidget(m_btnPlayList);

    lytMainVBox->addLayout(lytTopHBox);
    lytMainVBox->addStretch(1);
    lytMainVBox->addLayout(lytCenterHBox);
    lytMainVBox->addStretch(1);
    lytMainVBox->addLayout(lytBottomHBox);
}

void MainWindow::onClickBtnPlayPause()
{
    QMediaPlayer *player = MyPlayer::getInstance();

    if (m_btnPlayPause->text() == "播放") {
        m_btnPlayPause->setText("暂停");
        player->play();
        m_timer->start();
    } else {
        m_btnPlayPause->setText("播放");
        player->pause();
        m_timer->stop();
    }
}

void MainWindow::onClickBtnPre()
{
    QMediaPlaylist *playlist = MyPlayList::getInstance();
    if (playlist->currentIndex() <= 0) {
        return ;
    }
    else
        playlist->previous();
}

void MainWindow::onClickBtnNext()
{
    QMediaPlaylist *playlist = MyPlayList::getInstance();
    if (playlist->nextIndex() >= playlist->mediaCount() - 1) {
        return ;
    } else {
        playlist->next();
    }  
}

void MainWindow::setMetaData()
{
    QMediaPlayer *player = MyPlayer::getInstance();

    m_labelSongName->setText("歌曲名:" + player->metaData(QMediaMetaData::Title).toString());
    m_labelSingerName->setText("歌手:" + player->metaData(QMediaMetaData::Author).toString());
    m_labelAlbumName->setText("专辑:" + player->metaData(QMediaMetaData::AlbumTitle).toString());
   // m_labelAlbumPic->setPixmap(pixmap);//setText(player->metaData(QMediaMetaData::AlbumTitle).toString());
    qDebug() << " ....  " << player->metaData(QMediaMetaData::ContributingArtist).toStringList();
}

void MainWindow::setProgressValue()
{
//    static int value = 1000;
//    QMediaPlayer *player = MyPlayer::getInstance();
//    m_proPlay->setValue(value / player->metaData(QMediaMetaData::Duration).toInt());
//    value += 1000;

    //qDebug () << "value:" << player->metaData(QMediaMetaData::Duration).toInt() / value++;
}

