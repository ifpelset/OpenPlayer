#include "LoginDlg.h"
#include "MainWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonArray>
#include <QMessageBox>

LoginDlg::LoginDlg(QWidget *parent) : QDialog(parent)
{
    initLoginDlg();
}

void LoginDlg::initLoginDlg()
{
    QHBoxLayout *lytTopHBox = new QHBoxLayout;
    QHBoxLayout *lytCenterHBox = new QHBoxLayout;
    QHBoxLayout *lytBottomHBox = new QHBoxLayout;
    QVBoxLayout *lytMainVBox = new QVBoxLayout(this);

    m_labelUserName = new QLabel("用户名：", this);
    m_labelPassWord = new QLabel("密  码：", this);

    m_lineEditUserName = new QLineEdit(this);
    m_lineEditPassWord = new QLineEdit(this);

    m_btnLogin = new QPushButton("登录", this);

    m_lineEditPassWord->setEchoMode(QLineEdit::Password);

    connect(m_btnLogin, SIGNAL(clicked(bool)), this, SLOT(sendLoginRequest()));

    lytTopHBox->addWidget(m_labelUserName);
    lytTopHBox->addWidget(m_lineEditUserName);

    lytCenterHBox->addWidget(m_labelPassWord);
    lytCenterHBox->addWidget(m_lineEditPassWord);

    lytBottomHBox->addSpacerItem(new QSpacerItem(100, 20));
    lytBottomHBox->addWidget(m_btnLogin);

    lytMainVBox->addLayout(lytTopHBox);
    lytMainVBox->addLayout(lytCenterHBox);
    lytMainVBox->addLayout(lytBottomHBox);

    setWindowTitle("Login Dou Ban FM");
}

void LoginDlg::setAudioInfo()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req;

    // 获取歌曲列表
    QByteArray songArray;
    songArray.append("app_name=radio_desktop_win&");
    songArray.append("version=100&");
    songArray.append(QString("user_id=%1&").arg(m_userID));
    songArray.append(QString("token=%1&").arg(m_token));
    songArray.append(QString("expire=%1&").arg(m_expire));
    songArray.append("type=n&");
    songArray.append("channel=1&");
    QUrl url = "http://www.douban.com/j/app/radio/people?" + QString(songArray);

    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    manager->get(req);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(audioInfoRequestFinished(QNetworkReply*)));
}

void LoginDlg::sendLoginRequest()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req;

    req.setUrl(QUrl("http://www.douban.com/j/app/login"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postArg;
    postArg.append("app_name=radio_desktop_win&");
    postArg.append("version=100&");
    postArg.append(QString("email=%1&").arg(m_lineEditUserName->text()));
    postArg.append(QString("password=%1").arg(m_lineEditPassWord->text()));

    manager->post(req, postArg);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(loginRequestFinished(QNetworkReply*)));
}

void LoginDlg::loginRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        return ;
    }

    QByteArray arr = reply->readAll();
    QJsonObject obj = QJsonDocument::fromJson(arr).object();
    if (obj.value("r") != 0) {
        QMessageBox::critical(this, "登录错误", obj.value("err").toString());
        return ;
    }

    QMessageBox::information(this, "登录成功", "登录成功");
    m_userID = obj.value("user_id").toString();
    m_token = obj.value("token").toString();
    m_expire = obj.value("expire").toString();

    setAudioInfo();
}

void LoginDlg::audioInfoRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
       qDebug() << "Error:" << reply->errorString();
       return ;
   }
   ((MainWindow *)parent())->setArrAudioInfo(reply->readAll());
   ((MainWindow *)parent())->setIsOnline(true);
    close();
}

