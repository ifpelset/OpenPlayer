#include "MyPlayList.h"

QMediaPlaylist *MyPlayList::m_playlist = NULL;

MyPlayList::MyPlayList()
{

}

QMediaPlaylist *MyPlayList::getInstance()
{
    if (m_playlist == NULL) {
        m_playlist = new QMediaPlaylist;
    }

    return m_playlist;
}
