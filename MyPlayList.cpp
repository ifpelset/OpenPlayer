#include "MyPlayList.h"

QMediaPlaylist *MyPlayList::m_playlist = NULL;

MyPlayList::MyPlayList()
{

}

QMediaPlaylist *MyPlayList::getInstance()
{
    if (m_playlist == NULL) {
        m_playlist = new QMediaPlaylist;
        m_playlist->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/getAlbumPicture/123.mp3"));
        m_playlist->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/getAlbumPicture/456.mp3"));
        m_playlist->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/getAlbumPicture/789.mp3"));
    }

    return m_playlist;
}

