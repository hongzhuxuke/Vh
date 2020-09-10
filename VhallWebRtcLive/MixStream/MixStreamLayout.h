#pragma once

#include <QObject>
#include "../Unility/Unility.h"
#include <QTimer>

#define RESET_LAYOUT_TIMEOUT    3000 
class WebRtcLiveWdg;

class MixStreamLayout : public QObject
{
    Q_OBJECT

public:
    MixStreamLayout();
    ~MixStreamLayout();

   // changeMixedProfile表示桌面共享时，旁路混流改成1080P
    void SetMixStreamLayout(bool startClass = false, int layout = -1, bool hasDesktopStream = false);
    void SetMainViewStream(QString streamId);

    void RestartSetLayout(int layout, bool hasDesktop);
    void RestartSetMainView();

    void RePushLocalStream();
    
    void RePushDesktopSream(int type);
    void RePushMediaFileStream();
    void SetAlreadyMixed(bool mixed);

    void StopAllRepublish();

    int GetCurrentLayOut();
    void SetMainViewStreamID(const QString& stream_id);

    BROAD_CAST_DPI GetBradCastIndex(int broadIndex);

    //enum eUiSetLayoutMode
    //{
    //   eUiSetLayoutMode_TILED = 0, //主次平铺
    //   eUiSetLayoutMode_GRID,          //均匀分布
    //};

    void SetRetLiveWdg(WebRtcLiveWdg* pWebRtcLiveWdg);
private slots:
    void slot_OnSetBroadCastLayoutTimeOut();
    void slot_OnSetMainViewTimeOut();
    void slot_OnRepushMediaFileStream();
    void slot_OnRepushLocalStream();
    void slot_OnRepushDeskTopStream();
    void slot_OnRepushSourceStream();
private:
   int iMixLayoutMode();

    bool  mbAlreadySetMixed = false;
    LayoutMode mCurrentLayoutMode;


    QTimer mLayoutTimer;
    QTimer mMainViewTimer;
    QString mStreamId;

    int mReSetLayoutType;
    bool bResetHasDesktop = false;

    QTimer mRePushLocalStreamTimer;
    QTimer mRePushDeskStopStreamTimer;
    QTimer mRePushSourceStreamTimer;
    QTimer mRePushMediaFileStreamTimer;

    WebRtcLiveWdg* mpWebRtcLiveWdg = nullptr;
};
