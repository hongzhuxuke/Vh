#pragma once
#include <QString>

////清晰度配置
#define Fluent_definition     QString::fromWCharArray(L"流畅")
//#define High_definition         QString::fromWCharArray(L"高清")   
//#define Super_definition        QString::fromWCharArray(L"超清")   
//#define SuperHigh_definition    QString::fromWCharArray(L"超高清")   

class DataManagerInterface 
{
public:
    DataManagerInterface() {};
    virtual ~DataManagerInterface() {};
    virtual bool Init(std::wstring logPath) = 0;

    virtual void WriteLog(char* pszMessage, ...) = 0;

	virtual void SetVersion(QString appid) = 0;
	virtual QString GetVersion() = 0;

    virtual void SetAppid(QString appid) = 0;
    virtual QString GetAppid() = 0;

	//virtual void SetJoinUid(QString appid) = 0;
	//virtual QString GetJoinUid() = 0;

    //virtual void SetThridPartyUserId(QString third_party_user_id) = 0;
    //virtual QString GetThridPartyUserId() = 0;

    virtual void SetInteractionRoomId(QString roomid) = 0;
    virtual QString GetInteractionRoomId() = 0;

    virtual void SetAccessToken(QString token) = 0;
    virtual QString GetAccessToken() = 0;

    virtual QString GetStartUrl() = 0;
    virtual QString GetInavInfoUrl() = 0;

    virtual bool GetIsSpeaking() = 0;
    virtual void SetIsSpeaking(const bool& bIsLiving = false) = 0;

    //virtual void SetPushStreamPermission(bool host) = 0;
    //virtual bool GetPushStreamPermission() = 0;

	virtual void SetPublished(bool bIsLiving = false) = 0;
	virtual bool IsAlreadyPublished() = 0;

	virtual QString GetLiveRoomId() = 0;
	virtual void SetLiveRoomId(QString roomId) = 0;

    virtual void SetMuteMicByTeacher(bool mute) = 0;
    virtual bool GetMuteMicByTeacherState() = 0;

    virtual void SetMuteCameraByTeacher(bool mute) = 0;
    virtual bool GetMuteCameraByTeacherState() = 0;

    virtual void SetCurrentPenUser(const QString& user_id) = 0;
    virtual QString GetCurrentPenUser() = 0;

    virtual void SetTeacherJoinId(const QString& user_id) = 0;
    virtual QString GetTeacherJoinId() = 0;

    virtual bool IsMyCameraOpen() = 0;
    virtual void SetMyCameraOpen(bool open) = 0;

    virtual bool IsMultiClassOtherUserCameraOpen() = 0;
    virtual void SetMultiClassOtherUserCameraOpen(bool open) = 0;

    //文件插播相关接口
    /**
    * 获取播放文件列表
    */
    virtual QList<QString> GetMediaPlayFileList() = 0;
    /*
    * 删除插播文件
    */
    virtual void RemoveMediaPlayFile(QString filePath) = 0;
    /**
    * 添加插播文件
    */
    virtual void AddMediaPlayFile(QString filePath) = 0;
    /*
    *  判断当前文件是否存在在
    */
    virtual bool IsFileExist(const QString filePath) = 0;
    /*
    *  判断当前文件是否是支持的文件格式。
    *  目前支持的文件格式有：MP4、AVI、MKV、FLV、MOV、MP3、WAV
    */
    virtual bool IsSupportFormat(const QString filePath) = 0;

};