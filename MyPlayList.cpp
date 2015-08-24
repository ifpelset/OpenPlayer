#include "MyPlayList.h"

QMediaPlaylist *MyPlayList::m_playlist = NULL;

MyPlayList::MyPlayList()
{

}

QMediaPlaylist *MyPlayList::getInstance()
{
    if (m_playlist == NULL) {
        m_playlist = new QMediaPlaylist;
        //m_playlist->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/MyMusic/吴克群 - 寂寞来了怎么办_.mp3"));
        m_playlist->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/MyMusic/薛之谦 - 绅士.mp3"));
        m_playlist->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/MyMusic/薛之谦 - 下雨了.mp3"));
        m_playlist->addMedia(QUrl::fromLocalFile("C:/Users/IFPELSET/Desktop/MyMusic/薛之谦 - 演员.mp3"));
    }

    return m_playlist;
}
