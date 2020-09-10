#ifndef HTTP_PROXY_GET_TEXT_H
#define HTTP_PROXY_GET_TEXT_H
#include <QObject>

class HttpProxyGetInfo
{
public:
    HttpProxyGetInfo();
   ~HttpProxyGetInfo();

   static int GetProxyInfoFromBrowser(QString &ip,int &port,QString &userName, QString &userPwd);
   static bool IsEnableInternetProxy();
   static int GetProxyIP(QString &ip, int &port, QString &userName, QString &userPwd);
   static void SplitProxyIPInfo(const QString httpProxy, QString &ip,int &port);
   static void GetProxyUserInfo(const QString httpProxy, QString &userName, QString &userPwd);

};

#endif // HTTP_PROXY_GET_TEXT_H
