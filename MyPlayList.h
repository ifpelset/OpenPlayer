#ifndef MYPLAYLIST_H
#define MYPLAYLIST_H

#include <QMediaPlaylist>

class MyPlayList
{
public:
    static QMediaPlaylist *getInstance();

private:
    static QMediaPlaylist *m_playlist;
    MyPlayList();
};

#endif // MYPLAYLIST_H
