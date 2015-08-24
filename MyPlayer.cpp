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

        m_player->setPlaylist(myPlayList);
    }

    return m_player;
}

