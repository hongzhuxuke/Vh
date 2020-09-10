#ifndef UTILSETTING_H
#define UTILSETTING_H

#include <QVariant>
#include <QString>

class ConfigSetting {
public:
   static bool writeValue(const QString& path, const QString& group, const QString& user_key, const QVariant &value);
   //static bool readInit(QString path, QString group, QString user_key, QString &user_value);
   static bool WriteInt(const QString& path, const QString& group, const QString& user_key, const int& value);
   static int ReadInt(const QString& path, const QString& group, const QString& user_key, const int& defaultValue);
   static QString ReadString(const QString& path, const QString& group, const QString& user_key, const QString& defaultValue);
   static int GetAutoBitRate(int width, int height);
};


#define DEFAULT_PIC_NAME    "default_pic_name"
#define VALUE_PIC_NAME      "default_pushstream_pic.png"
#define USER_DEFAULT          "user"
#define GROUP_DEFAULT         "default"
#define KEY_RECORD_MODE       "recode"
#define KEY_RECORD_PATH       "path"
#define KEY_RECORD_FILENAME   "filename"
//#define KEY_RECORD_FILESNUM   "filesnum" //1 生成一个文件  0 生成多个文件
//分辨率
//#define KEY_PUBLISH_QUALITY   "quality"
#define KEY_PUBLISH_QUALITY_NUMBER   "quality_id"

#define KEY_CHK_VER_URL      "verUrl"
#define KEY_AUDIO_SYNC       "synctime"
#define KEY_CUSTOM_RESOLUTION    "custom_resolution"
#define KEY_CUSTOM_BITRATE       "custom_bitrate"

//默认线路
#define KEY_VIDEO_LINE        "video_line"
#define KEY_BITRATE_360P      "bitrate_360p"
#define KEY_BITRATE_540P      "bitrate_540p"
#define KEY_BITRATE_720P      "bitrate_720p"
#define KEY_BITRATE_1080P     "bitrate_1080p"
#define KEY_BITRATE_AUDO      "bitrate_auto"
#define KEY_AUDO_Enable       "auto_enable"
#define KEY_MENGZHU_APP       "isMengZhu"

#define MEDIA_STREAM_WIDTH       "mediaWidth"
#define MEDIA_STREAM_HEIGHT       "mediaHeight"
#define MEDIA_STREAM_BW      "mediaBW"
#define MEDIA_STREAM_FRAMERATE "mediaFrameRate"

#define DESKTOP_STREAM_WIDTH       "DeskTopWidth"
#define DESKTOP_STREAM_HEIGHT       "DeskTopHeight"
#define DESKTOP_STREAM_BW      "DeskTopBW"
#define DESKTOP_STREAM_FRAMERATE "DeskTopFrameRate"
//////////////////////////////////////////////////////////////////////////
//字幕
#define KEY_VT_FONT_SIZE      "vt_font_size"
#define KEY_VT_LANGUAGE       "vt_language"


#define KEY_VCODEC_FPS       "fps"
#define KEY_VCODEC_GOP       "gop"

#define KEY_VCODEC_HIGH_PROFILE_OPEN   "high_codec_open"//高质量编码
#define KEY_AUTO_SPEED              "autospeed"
#define KEY_SWSCALE_TYPE            "scaletype"

#define KEY_CAMERA_RESOLUTION_X     "ResolutionX"
#define KEY_CAMERA_RESOLUTION_Y     "ResolutionY"
#define KEY_CAMERA_FRAMEINTERVAL    "FrameInterval"
#define KEY_CAMERA_DEINTERLACE      "DeinterLace"

//是否开启噪音阀
#define KEY_AUDIO_ISNOISE           "isNoise"
#define KEY_AUDIO_NOISE_VALUE       "noiseValue"
#define KEY_AUDIO_KBPS              "kbps"               //音频码率
#define KEY_VHALL_LIVE              "vhall_live"         //微吼直播
#define KEY_VHALL_HELPER            "vhall_helper"
#define KEY_VHALL_ICON_NAME         "icon_name"
#define KEY_LOCAL_VIDEO_PROFILE     "local_video_profile"
#define KEY_REMOTE_VIDEO_PROFILE    "remote_video_profile"
#define KEY_VHALL_LOGO_HIDE         "vhall_logo_hide"    //微吼logo隐藏字段，获取的value=1 表示隐藏所有vhall_logo字样
#define KEY_VHALL_USER_REGISTER_HIDE         "hide_user_register"    //登陆界面是否隐藏用户注册入口
#define KEY_VHALL_APPUPDATE_ENABLE  "vhall_app_update_enable" //助手应用更新是否允许，获取的value=1 表示允许，其他不允许
#define KEY_VHALL_HIDE_MEMBER_LIST  "vhall_hide_member_list"   //隐藏成员列表：1 隐藏、0显示
#define KEY_VHALL_SHARE_LIVE_PIC    "vhall_share_pic_downloadurl"   //下载二维码生成地址
#define KEY_AUDIO_SAMPLERATE        "samplerate"			//音频采样率
#define KEY_AUDIO_NOISEREDUCTION    "noisereduction"     //麦克风降噪
#define KEY_AUDIO_MICGAIN           "micgain"            //麦克风增益
#define KEY_VHALL_ShareURL         "vhallShareUrl"    //分享链接
#define KEY_VHALL_BU                "bu"    //平台类型
#define KEY_DOMAIN                  "domain"
#define KEY_DEBUG_ADDR              "debug_addr"

#define KEY_LOGREPORT_URL           "logReportUrl"       //日志上报路径
//#define KEY_VIDEO_HIGHQUALITYCOD   "highQualityCod"    //高质量编码

//互动房间
#define KEY_ROOM_ID                 "roomID"
#define KEY_NICK_NAME               "userNickName"
#define KEY_ROOM_PWD                "roomPwd"
#define KEY_ROOM_PHONE                "phone"

//强制单声道
#define KEY_AUDIO_FORCEMONO         "force_mono"

//自动生成回放
#define KEY_SERVER_PLAYBACK         "server_playback"
//#define KEY_AUDIODEVICE_INFO  "audiodeviceinfo"

#define DEFAULT_VCODEC_FPS   25
#define DEFAULT_VCODEC_GOP   2
#define DEFAULT_VCODEC_HIGH_PROFILE_OPEN   0


#define CONFIGPATH L"config.ini"
#define VHALL_TOOL_CONFIG   L"vhall_tool_config.ini"
#define VHALL_CLASS_TOOL_CONFIG   L"vhall_class_config.ini"
#define CONFIGPATH_DEVICE L"audiodevice.ini"

#define KEY_URL_DOMAIN "domainUrl"
#define KEY_VSS_URL_DOMAIN "vssDomainUrl"
#define domainUrl "http://e.vhall.com"
#define vss_domainUrl   "http://d-vss.vhall.com"
#define KEY_PAAS_DOMAIN   "vhall_paas_sdk_domain"
#define VALUE_PAAS_DOMAIN   "http://api.vhallyun.com/sdk/v1"

//调试面板
#define DEBUGFORM "debugform"
#define DEBUGFILE "debugfile"
#define DEBUGJS "debugjs"
#define DEBUGMEMBER "debugmember"
//代理相关
#define PROXY_OPEN "proxy_open"
#define PROXY_HOST "proxy_host"
#define PROXY_PORT "proxy_port"
#define PROXY_USERNAME "proxy_username"
#define PROXY_PASSWORD "proxy_password"
#define PROXY_TYPE   "proxy_type"   //使用的代理设置类型 PROXY_CONFIG_TYPE

#define PUSH_PIC_PATH   "push_pic_path" //互动推流图片地址
#define PUSH_PIC_OPEN   "push_pic_open" //互动推流图片地址

#define PUSH_DESKTOP_CAPTURE_OPEN   "desktop_capture_open" //桌面音频采集
#define PUSH_DESKTOP_CAPTURE_INDEX  "desktop_capture_index" //桌面音频采集
#define PUSH_DESKTOP_CAPTURE_DEVID  "desktop_capture_devId" //桌面音频采集
#define PUSH_DESKTOP_CAPTURE_VOLUME "desktop_capture_volume" //桌面音频采集
#define PUSH_DESKTOP_CAPTURE_MUTE_REMOTE "PUSH_DESKTOP_CAPTURE_MUTE_REMOTE" //桌面音频采集静音
#define PUSH_DESKTOP_SHARE_LAYOUT   "push_desktop_layout"     //公开课桌面共享时旁路布局

#define PLAY_OUT_DEV_ID             "play_out_dev_id"

#define PLAY_OUT_DEV_INDEX          "play_out_dev_index"

#define CAPTURE_CAMERA_ID           "capture_camera_id"
#define CAPTURE_CAMERA_INDEX        "capture_camera_index"

#define DOUBLE_CAMERA_ID            "double_camera_id"   //记录上次双推摄像头id

#define CAPTURE_MIC_ID              "capture_mic_id"
#define CAPTURE_MIC_INDEX           "capture_mic_index"

#define SCREEN_ADAPTIVE  "screen_adaptive"   //是否自适应屏幕
//线路扩展
#define PROXY_NOLINE "proxy_noLine"

enum ShareScreenLayout {
   ShareScreenLayout_camera_top = 1,
   ShareScreenLayout_camera_bottom = 2,
   ShareScreenLayout_on_camera = 3,
};

enum eLOG {
	eLOG_Show = 0, //显示log
	eLOG_Hide = 1 //隐藏log
};

typedef enum PROXY_CONFIG_TYPE {
   NoProxyConfig = 0,
   ProxyConfig_Http = 1,        //手动配置http代理
   ProxyConfig_Browser = 2      //从浏览器读取代理配置
}_proxy_config_type;

//实时字幕设置
#define SUBTITLE_FONT_SIZE       "subtitle_font_size"       //字体大小
#define SUBTITLE_FONT_COLOR      "subtitle_font_color"      //字体颜色
#define SUBTITLE_FONT_STYLE      "subtitle_font_style"      //字体样式

//media_core日志
#define MEDIA_CORE_LOG_LEVEL "media_core_log_level"

//侦听
#define DEBUG_AUDIO_LISTEN "audio_listen"

//std::wstring	GetAppPath2();

#define X264_EX_PARAM
#define X264_EX_PARAM_VIDEO_PROCESS_FILTERS  "video_process_filters"
#define X264_EX_PARAM_IS_ADJUST_BITRATE      "is_adjust_bitrate"
#define X264_EX_PARAM_IS_QUALITY_LIMITED     "is_quality_limited"
#define X264_EX_PARAM_IS_ENCODER_DEBUG       "is_encoder_debug"
#define X264_EX_PARAM_IS_SAVING_DATA_DEBUG   "is_saving_data_debug"


//Audio CaptureLoop Config
// 0-31
#define AUDIO_PRIORITY "audio_priority"
// 1-5
#define AUDIO_INTERVAL "audio_interval"

#define AUDIO_DEFAULT_DEVICE "audio_default_device"
#define PLAYER_DEFAULT_DEVICE "player_default_device"


#define defaultLogReportURL QString("http://pingback.e.vhall.com/zs.gif")
//#define AUDIO_BITRATE "audio_bitrate"

#define GET_VERIFY_CODE_TIME "get_verify_code"

#define USER_AUTH_TOKEN "user_auth_token"
#define USER_PWD_LOGIN_CLIENT_UNIQUE   "client_unique"
#define LOG_ID_BASE "logIdBase"
#define DUMP_SERVER_URL "dumpURL"  //测试dump上报链接 http://helpdump.dev.vhall.com:5587/dumpupload  线上dump上报链接	http://helpdump.t.vhall.com/dumpupload
#define LAST_DUMP_UPLOAD_TIME "dump_upload_time"

#define TENCENT_APPID "appid"
#define TENCENT_SDKAPPID "sdk_appid"
#define TENCENT_BIZID "bizid"
#define TENCENT_ACCOUNT_TYPE "account_type"
//#define RECORD_FILES_NUM "files_num"
//文本提示
#define  VHALL_LIVE_TEXT      QString::fromWCharArray(L"微吼直播")
#define  VHALL_LIVE_HELPER    QString::fromWCharArray(L"微吼助手") 
#define defaultLogReportURL QString("http://pingback.e.vhall.com/zs.gif")
#define defaultDomainURL QString("http://t-api-class.e.vhall.com")


#endif // UTILSETTING_H
