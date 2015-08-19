#ifndef MYPLAYER_H
#define MYPLAYER_H

class QMediaPlayer;

class MyPlayer
{
public:
    static QMediaPlayer *getInstance();

private:
    static QMediaPlayer *m_player;
    MyPlayer();
};

#endif // MYPLAYER_H
