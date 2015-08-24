#ifndef PLAYLISTDLG_H
#define PLAYLISTDLG_H

#include <QDialog>
#include <QModelIndex>

class QListView;
class QStandardItemModel;
class QMenu;
class QAction;

class PlayListDlg : public QDialog
{
    Q_OBJECT
public:
    explicit PlayListDlg(QWidget *parent = 0);
    void createActions();

protected:
    void contextMenuEvent(QContextMenuEvent *);

signals:

public slots:
    void playSelectAudio(QModelIndex);

    void triggerAddAudio();
    void triggerRemoveAll();


private:
    QListView *m_listView;
    QStandardItemModel *m_stdItemModel;
    int m_mediaCount;

    QMenu *m_menuPop;
    QAction *m_actionAddAudio;
    QAction *m_actionRemoveAll;
};

#endif // PLAYLISTDLG_H
