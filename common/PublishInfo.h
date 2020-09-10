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
   string mStrAlias;//��ʾ����
   string mStrDomain;//����
   string mPingInfo;
   //��������Ϊ��TCP���ݣ�����Ϊ0����ΪRTMP������Ϊ��TCP
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

   //��������Ϊ��TCP���ݣ�����Ϊ0����ΪRTMP������Ϊ��TCP
   int  mIMultiConnNum = 0;
   int  mIMultiConnBufSize = 0;
   QString mRoomToken; //����΢�𻥶���Ҫ��token
   QString mPluginUrl; //�ĵ����url
   QString mMsgToken;
   QString mUserId;//�������û�ID�����   
   QString mWebinarType;//flash���ݲ������Ƿ�Ϊ��α�� ismix 
   QString mAccesstoken;//flash���ݲ���
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
    int isPrivilege = 0;         //0��ʾ��ǰ����Ƕ�α����1��ʾ�Ƕ�α����
    int isGuestChange = 0;      // //0��������ʹ�û������ܣ�1����ʹ�û������ܡ�
    int live_time = 0;          //ֱ��ʱ��
    int isVoice = 0;            //0��ʾ�û�û��ʹ��ʵʱ��Ļ���ܣ�1��ʾ֧��ʵʱ��Ļ����
    int display_plugin = 0;
    int display_cut_record = 0;      //�Ƿ���ʾ���¼��  eDispalyCutRecord
    int host_type = 0;           ///�˻����� eHostType 0 ��׼��(�޼���) 1 רҵ�� 2 �콢��  ����ע�汾�û����ṩ�������ܡ�
    int interact_type = 0;           //0��ֱ�� 1����Ѷ���� 2��΢�𻥶�
    int show_checkbox = 0;          //�Ƿ���ʾ����1��ʾ0����ʾ
    int handsUp = 0;
    int bizRole = 0;
    int device_audio = 0;
    int device_video = 0;
    int webinar_type = 0;   // 1 ֱ����������ֱ��
    int player = 1;     // 1 flash 2 h5
    int selectLiveType = 0; // eLiveType_Live ֱ���� eLiveType_VhallActive ������ ����Ӧ��ѡ��Ļ���͡�

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
    QString vod_create_url; //"http:\/\/vod.vhallservice.com"    //���¼������
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
    QString role_name; // �û���ɫ��1:��ʦ 2 : ѧԱ 3 : ���� 4 : �α� 5 : ��Σ�
    QString curr_presenter;
    QString speaker_list;
    QString invite_list;
    QString vss_token;
    QString vss_room_id;
    QString assistant_token;

    QString web_chat_url;//  ����Ƕ���ַ
    QString web_doc_url;//h5 �ĵ�Ƕ���ַ
    QString web_question_url;// h5 �ĵ�Ƕ���ַ
    std::list<PushStreamAddress> pushStreamList;
    std::list<QString> permission_data;
};

class VSSGetRoomInfo {
public:
    QString room_id;     //"lss_xxxxx",
    QString subject;     //"���Է���002"
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
    int status; //״̬��0ԤԼ1ֱ��2����

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

