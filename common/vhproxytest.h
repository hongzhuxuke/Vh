#ifndef _VH_PROXY_TEST_H_
#define _VH_PROXY_TEST_H_
#include <QString>
#include <QObject>
#include <QEvent>

class QNetworkReply;
class QAuthenticator;
class QNetworkProxy;

const QEvent::Type CustomEvent_LoginByAccount = (QEvent::Type)10000;    //用户登录
const QEvent::Type CustomEvent_LoginByPhone = (QEvent::Type)10001;      //手机号登录
const QEvent::Type CustomEvent_VerifyCode = (QEvent::Type)10002;        //获取登录验证码
const QEvent::Type CustomEvent_AuthCode = (QEvent::Type)10003;          //bu 获取
const QEvent::Type CustomEvent_StartLive = (QEvent::Type)10004;          //开播处理
const QEvent::Type CustomEvent_JoinActivityDirect = (QEvent::Type)10005;    //活动列表已嘉宾身份加入互动
const QEvent::Type CustomEvent_JoinActivityInit = (QEvent::Type)10006;    //获取初始化互动房间信息
const QEvent::Type CustomEvent_JoinActivityByPwd = (QEvent::Type)10007;    //口令登陆互动
const QEvent::Type CustomEvent_UploadLiveType = (QEvent::Type)10008;    //上报活动类型
const QEvent::Type CustomEvent_StopWebniar = (QEvent::Type)10009;       //结束直播
const QEvent::Type CustomEvent_StartWebniar = (QEvent::Type)10010;       //开始直播
const QEvent::Type CustomEvent_VSSGetRoomAttributes = (QEvent::Type)10011;    
const QEvent::Type CustomEvent_VSSGetPushInfo = (QEvent::Type)10012;


class QHttpResponeEvent : public QEvent {
public:
    QHttpResponeEvent(QEvent::Type type, const int code, const QString& data) : QEvent(type) {
        mCode = code;
        mData = data;
    }
    int mCode;
    QString mData;
    bool mIsClose;
    bool mIsScheduling;
};

class UploadLiveType : public QEvent {
public:
    UploadLiveType(QEvent::Type type, const int code, const QString& data, const int liveType) : QEvent(type) {
        mCode = code;
        mData = data;
        mLiveTypeType = liveType;
    }
    int mCode;
    int mLiveTypeType;
    QString mData;
};


class QHttpJoinActiveResponeEvent : public QEvent {
public:
    QHttpJoinActiveResponeEvent(QEvent::Type type, const int code, const QString& data, const QString& room_id, const QString& nick, const QString& password, const bool isPwdLogin) : QEvent(type) {
        mCode = code;
        mData = data;
        mIsPwdLogin = isPwdLogin;
        mRoomId = room_id;
        mJoinNickName = nick;
        mRoomPwd = password;
    }
    bool mIsPwdLogin;
    int mCode;
    QString mData;
    QString mRoomId;
    QString mJoinNickName;
    QString mRoomPwd;
};


class HttpRequest :public QObject{
   Q_OBJECT
public:
   HttpRequest(QObject* obj = NULL);
   ~HttpRequest();
   bool VHProxyTest(QString ip, unsigned short port, QString userName, QString password, QString url);
   bool VHGetRet(QString url, int, QByteArray &ba);
   bool VHPostRet(QString url, int, QByteArray &ba, QString &json);

private slots:
   void slot_authenticationRequired(QNetworkReply *, QAuthenticator *);
   void slot_proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *);
private:
   QString m_ip;
   int m_port;
   QString m_user;
   QString m_pwd;
};

#endif
