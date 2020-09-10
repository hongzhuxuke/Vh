#ifndef _VH_PROXY_TEST_H_
#define _VH_PROXY_TEST_H_
#include <QString>
#include <QObject>
#include <QEvent>

class QNetworkReply;
class QAuthenticator;
class QNetworkProxy;

const QEvent::Type CustomEvent_LoginByAccount = (QEvent::Type)10000;    //�û���¼
const QEvent::Type CustomEvent_LoginByPhone = (QEvent::Type)10001;      //�ֻ��ŵ�¼
const QEvent::Type CustomEvent_VerifyCode = (QEvent::Type)10002;        //��ȡ��¼��֤��
const QEvent::Type CustomEvent_AuthCode = (QEvent::Type)10003;          //bu ��ȡ
const QEvent::Type CustomEvent_StartLive = (QEvent::Type)10004;          //��������
const QEvent::Type CustomEvent_JoinActivityDirect = (QEvent::Type)10005;    //��б��Ѽα���ݼ��뻥��
const QEvent::Type CustomEvent_JoinActivityInit = (QEvent::Type)10006;    //��ȡ��ʼ������������Ϣ
const QEvent::Type CustomEvent_JoinActivityByPwd = (QEvent::Type)10007;    //�����½����
const QEvent::Type CustomEvent_UploadLiveType = (QEvent::Type)10008;    //�ϱ������
const QEvent::Type CustomEvent_StopWebniar = (QEvent::Type)10009;       //����ֱ��
const QEvent::Type CustomEvent_StartWebniar = (QEvent::Type)10010;       //��ʼֱ��
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
