#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QNetworkReply;

class LoginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDlg(QWidget *parent = 0);

    void initLoginDlg();
    void setAudioInfo();

signals:

public slots:
    void sendLoginRequest();
    void loginRequestFinished(QNetworkReply*);
    void audioInfoRequestFinished(QNetworkReply*);

private:
    QLabel *m_labelUserName;
    QLabel *m_labelPassWord;
    QLineEdit *m_lineEditUserName;
    QLineEdit *m_lineEditPassWord;
    QPushButton *m_btnLogin;

    QString m_userID;
    QString m_token;
    QString m_expire;
};

#endif // LOGINDLG_H
