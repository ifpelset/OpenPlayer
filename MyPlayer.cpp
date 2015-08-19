#include "MyPlayer.h"
#include "MyPlayList.h"
#include <QMediaPlayer>

#include <QDebug>

QMediaPlayer *MyPlayer::m_player = NULL;


MyPlayer::MyPlayer()
{

}

QMediaPlayer *MyPlayer::getInstance()
{
    if (m_player == NULL) {
        m_player = new QMediaPlayer;
        QMediaPlaylist *myPlayList = MyPlayList::getInstance();

        myPlayList->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/getAlbumPicture/123.mp3"));
        myPlayList->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/getAlbumPicture/456.mp3"));
        myPlayList->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/getAlbumPicture/789.mp3"));

        m_player->setPlaylist(myPlayList);
    }

    return m_player;
}

