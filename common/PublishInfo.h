#pragma once
#include <string>  
using namespace std;
#include <QList>
#include <QJsonArray>
#include <QMutex>
#include <QJsonParseError>
#include <QVariant>
#include "json.h"

struct PubLineInfo
{
   int mID;
   string mStrAlias;//显示名称
   string mStrDomain;//域名
   string mPingInfo;
   //以下内容为多TCP内容，若均为0，则为RTMP，否则为多TCP
   //int  mIMultiConnNum = 0;
   //int  mIMultiConnBufSize = 0;
}; 

class PublishInfo
{
public:
   PublishInfo();
   ~PublishInfo();
public:
   void init(string gUserName, string gToken, string gStreamName, string gChanelId, string gRtmpUrl, bool bHideLogo);
   void parsePubLineStr(string gPubLineList);
   void InitMultitcpPubLine(QString customUrlFile);
public:
   string mUserName;
   string mToken;
   string mStreamName;
   string mChanelId;
   string mRtmpUrlsStr;
   string mCurRtmpUrl;
   int mCurRtmpUrlIndex;
   int nProtocolType;
   bool m_bHideLogo = false;
   QList<PubLineInfo> mPubLineInfo;

   //以下内容为多TCP内容，若均为0，则为RTMP，否则为多TCP
   int  mIMultiConnNum = 0;
   int  mIMultiConnBufSize = 0;
   QString mRoomToken; //加入微吼互动需要的token
   QString mPluginUrl; //文档插件url
   QString mMsgToken;
   QString mUserId;//流名与用户ID的组合   
   QString mWebinarType;//flash传递参数，是否为多嘉宾活动 ismix 
   QString mAccesstoken;//flash传递参数
   QString mRole;
   QString mScheduler;
   QString mWebinarName;
};

class StartLiveUrl {
public:
    StartLiveUrl() {};
    ~StartLiveUrl() {};
    void clear() {
        stream_token.clear();
        stream_name.clear();
        msg_token.clear();
        hide_logo = false;
    }
public:
    QString stream_token;
    QString stream_name;
    QString msg_token;
    bool hide_logo = false;
};

class UserOnLineParam {
public:
    QString type;
    QString userId;
    QString userName;
    QString userRole;
    bool is_banned;
};


class PushStreamAddress {
public:
    QString name;
    QString srv;
};

#define FLASH_LIVE_CREATE_TYPE  1
#define H5_LIVE_CREATE_TYPE 2

class ClientApiInitResp {
public:
    ClientApiInitResp() {};
    ~ClientApiInitResp() {};
public:
    int ismix = 0;              // 1
    int forbidchat = 0;         // 0
    int is_webinar_plug = 0;    // 1
    int protocol_type = 0;      // 0
    int isPrivilege = 0;         //0表示当前活动不是多嘉宾活动，1表示是多嘉宾活动。
    int isGuestChange = 0;      // //0代表不可以使用互动功能，1可以使用互动功能。
    int live_time = 0;          //直播时长
    int isVoice = 0;            //0表示用户没有使用实时字幕功能，1表示支持实时字幕功能
    int display_plugin = 0;
    int display_cut_record = 0;      //是否显示打点录制  eDispalyCutRecord
    int host_type = 0;           ///账户类型 eHostType 0 标准版(无极版) 1 专业版 2 旗舰版  ，标注版本用户不提供互动功能。
    int interact_type = 0;           //0：直播 1：腾讯互动 2：微吼互动
    int show_checkbox = 0;          //是否显示弹窗1显示0不显示
    int handsUp = 0;
    int bizRole = 0;
    int device_audio = 0;
    int device_video = 0;
    int webinar_type = 0;   // 1 直播，其他非直播
    int player = 1;     // 1 flash 2 h5
    int selectLiveType = 0; // eLiveType_Live 直播， eLiveType_VhallActive 互动。 本地应用选择的活动类型。

    QString current_app_version;
    QString avatar;     //"\/\/t-alistatic01.e.vhall.com\/upload\/user\/avatar\/b5\/fa\/b5fa73691edecde3fa41be2a017a76c8.jpg",
    QString room_token;
    QString watchLayout;
    QString definition;
    QString against_url;
    QString thirdStreamToken;
    QString reportUrl;
    QString mainShow;
    QString accesstoken;    //"vhall"
    QString switchDomain;   //"//t-mss.e.vhall.com/",
    QString vod_create_url; //"http:\/\/vod.vhallservice.com"    //打点录制连接
    QString filterurl;      //"http:\/\/t.e.vhall.com\/webinar\/chatfilter\/250685450?vc=eyJpdiI6Ilh5dTZBRzJRU2ROazVzdWMydU5CY2c9PSIsInZhbHVlIjoiRHBZdVMwZ1I0ZDArb0xSemd2QVlyelwvV2V3SlMzK2JSbEhTVjI1SnFCVE09IiwibWFjIjoiNWYxYzVjMTU1YTVkMTM0NGE3OGFhNjljZTY2ZWE0NDE3OWYzNjU3MmIyODI3MTBkYWE4MWQ3MDdlZmQ3NjE3MSJ9",
    QString msg_srv;        //"t-msg01.e.vhall.com",
    QString msg_port;       //"80"
    QString chat_srv;       //"t-chat01.e.vhall.com"
    QString chat_port;      //"80"
    QString chat_url;       //"//t-api.e.vhall.com\/pub\/index"
    QString webinar_name;   
    QString thirdstream_url;  //t-relay.e.vhall.com:5000\/api\/3rdstream\/
    QString interact_plugins_url;
    QString plugins_url;
    QString join_uid;
    QString user_id;
    QString saas_join_uid;
    QString params_verify_token;
    QString nick_name;
    QString role_name; // 用户角色（1:老师 2 : 学员 3 : 助教 4 : 嘉宾 5 : 监课）
    QString curr_presenter;
    QString speaker_list;
    QString invite_list;
    QString vss_token;
    QString vss_room_id;
    QString assistant_token;

    QString web_chat_url;//  聊天嵌入地址
    QString web_doc_url;//h5 文档嵌入地址
    QString web_question_url;// h5 文档嵌入地址
    std::list<PushStreamAddress> pushStreamList;
    std::list<QString> permission_data;
};

class VSSGetRoomInfo {
public:
    QString room_id;     //"lss_xxxxx",
    QString subject;     //"测试房间002"
    QString account_id;
    QString inav_id;
    QString channel_id;
    QString record_id;
    QString start_time;
    QString app_id;
    QString paas_access_token;
    QString role_name;
    QString third_party_user_id;

    int category;
    int status; //状态，0预约1直播2结束

};


class VSSRoomAttributes {
public:
    int is_handsup;
    QString main_screen;
    QString speaker_list;

    QString layout;
    QString definition;
};

class VSSRoomPushUrl {
public:
    QString stream_number;
    QString push_address;
};

