#include "PlayListDlg.h"
#include "MainWindow.h"
#include "MyPlayList.h"
#include "MyPlayer.h"
#include <QDebug>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QPushButton>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QDesktopServices>

PlayListDlg::PlayListDlg(QWidget *parent)/* :
    //QDialog(parent)*/
{
    resize(140, 320);
    setParent(parent);
    setGeometry(600-160, 20, 160, 290);
    //setStyleSheet("background-color: rgba(160,160,161,0.3);");

    QHBoxLayout *bx = new QHBoxLayout(this);
    QMediaPlaylist *playList = MyPlayList::getInstance();
    QString fileName;
    QString audioName;
    m_mediaCount = playList->mediaCount();

    m_listView = new QListView(this);
    m_stdItemModel = new QStandardItemModel(1, m_mediaCount, this);

    m_stdItemModel->setHeaderData(0, Qt::Horizontal, "播放列表");
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i=0; i<m_mediaCount; i++) {
        fileName = QFileInfo(playList->media(i).canonicalUrl().toLocalFile()).fileName();
        audioName = fileName.left(fileName.indexOf("."));
        m_stdItemModel->setItem(i, new QStandardItem(audioName));
    }

    connect(m_listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(playSelectAudio(QModelIndex)));
    m_listView->setModel(m_stdItemModel);
    bx->addWidget(m_listView);

    createActions();
}

void PlayListDlg::createActions()
{
    m_menuPop = new QMenu;
    m_actionAddAudio = m_menuPop->addAction("增加音乐");
    m_actionRemoveAll = m_menuPop->addAction("清空播放列表");

    connect(m_actionAddAudio, SIGNAL(triggered(bool)), this, SLOT(triggerAddAudio()));
    connect(m_actionRemoveAll, SIGNAL(triggered(bool)), this, SLOT(triggerRemoveAll()));
}

void PlayListDlg::contextMenuEvent(QContextMenuEvent *ev)
{
   m_menuPop->exec(QCursor::pos());
   ev->accept();
}

void PlayListDlg::playSelectAudio(QModelIndex idx)
{
    QMediaPlaylist *playList = MyPlayList::getInstance();
    playList->setCurrentIndex(idx.row());
    MyPlayer::getInstance()->play();
    ((MainWindow *)parent())->m_btnPlayPause->setIcon(QIcon(":/imgs/imgs/gtk-media-pause.png"));
    ((MainWindow *)parent())->setIsOnline(false);
}

void PlayListDlg::triggerAddAudio()
{
    QString fileName, audioName, formatName;
    QMediaPlaylist *playList = MyPlayList::getInstance();
    QStringList list = QFileDialog::getOpenFileNames(
                this, "选择需要添加的音乐文件");

    if (list.isEmpty())
        return ;

    foreach(QString absFilesPath, list) {
        fileName = QFileInfo(absFilesPath).fileName();
        formatName = fileName.mid(fileName.indexOf("."));

        if (formatName != ".mp3")
            continue ;

        audioName = fileName.left(fileName.indexOf("."));
        m_stdItemModel->setItem(m_mediaCount ++, new QStandardItem(audioName));
        playList->addMedia(QUrl::fromLocalFile(absFilesPath));
    }
}

void PlayListDlg::triggerRemoveAll()
{
    QMediaPlaylist *playList = MyPlayList::getInstance();

    while (m_mediaCount) {
        m_stdItemModel->removeRow(m_mediaCount - 1);
        playList->removeMedia(m_mediaCount - 1);
        m_mediaCount --;
    }
}
