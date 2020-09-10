#pragma once

#include <QObject>

class CBaseRender
{
public:
    CBaseRender();
    ~CBaseRender();

    virtual void SetUserJoinID(QString id);
    virtual QString GetUserJoinID();

    void SetStreamID(QString id);
    QString GetStreamID();

    void SetStreamHasVideoFormat(bool has);
    bool HasVideoFormat();

    void SetStreamHasAudioFormat(bool has);
    bool HasAudioFormat();


    void SetSmallWndUserJoinID(QString id);
    QString GetSmallWndUserJoinID();

    void SetSmallWndStreamID(QString id);
    QString GetSmallWndStreamID();

    void SetSmallWndStreamHasVideoFormat(bool has);
    bool HasSmallWndVideoFormat();

    void SetSmallWndStreamHasAudioFormat(bool has);
    bool HasSmallWndAudioFormat();

protected:
   QString mUserJoinId; //当前登录用户
private:
    bool mbHasVideoFormat;
    bool mbHasAudioFormat;
    bool mbSmallWndHasVideoFormat;
    bool mbSmallWndHasAudioFormat;


    //////////////////////////////
    QString mSmallWndUserJoinId; //学员用户id
    QString mSmallWndStreamId;//学员流id
    QString mStreamId;  //讲师流ID
};
