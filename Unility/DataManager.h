#pragma once

#include <QObject>
#include <QMutex>

#include "DataManagerInterface.h"

#define PLAY_FILE_LIST  "/playfile.list"
class DataManager : public QObject, public DataManagerInterface
{
   Q_OBJECT

public:
    DataManager(QObject *parent = nullptr);
    virtual ~DataManager();
    virtual bool Init(std::wstring logPath);
    virtual bool Init();
    virtual void WriteLog(char* pszMessage, ...);

	 virtual void SetVersion(QString appid);
	 virtual QString GetVersion();

    virtual void SetAppid(QString appid);
    virtual QString GetAppid();

	//virtual void SetJoinUid(QString JoinUid);
	//virtual QString GetJoinUid();

	 //virtual void SetThridPartyUserId(QString third_party_user_id) {}
	 //virtual QString GetThridPartyUserId() { return ""; }

    virtual void SetInteractionRoomId(QString roomid);
    virtual QString GetInteractionRoomId();

    virtual void SetAccessToken(QString token);
    virtual QString GetAccessToken();

    virtual QString GetStartUrl();
    virtual QString GetInavInfoUrl();

    virtual bool GetIsSpeaking();
    virtual void SetIsSpeaking(const bool& bIsLiving = false);

	virtual QString GetLiveRoomId();
	virtual void SetLiveRoomId(QString roomId);

    //virtual void SetPushStreamPermission(bool host);
    //virtual bool GetPushStreamPermission();

	virtual void SetPublished(bool bIsLiving = false);
	virtual bool IsAlreadyPublished();

    virtual void SetMuteMicByTeacher(bool mute);
    virtual bool GetMuteMicByTeacherState();

    virtual void SetMuteCameraByTeacher(bool mute);
    virtual bool GetMuteCameraByTeacherState();

    virtual void SetCurrentPenUser(const QString& user_id);
    virtual QString GetCurrentPenUser();

    virtual void SetTeacherJoinId(const QString& user_id);
    virtual QString GetTeacherJoinId();

    virtual bool IsMyCameraOpen();
    virtual void SetMyCameraOpen(bool open);

    virtual bool IsMultiClassOtherUserCameraOpen();
    virtual void SetMultiClassOtherUserCameraOpen(bool open);

    //�ļ��岥��ؽӿ�
    /**
    * ��ȡ�����ļ��б�
    */
    virtual QList<QString> GetMediaPlayFileList();
    /*
    * ɾ���岥�ļ�
    */
    virtual void RemoveMediaPlayFile(QString filePath);
    /**
    * ��Ӳ岥�ļ�
    */
    virtual void AddMediaPlayFile(QString filePath);
    /*
    *  �жϵ�ǰ�ļ��Ƿ������
    */
    virtual bool IsFileExist(const QString filePath);
    /*
    *  �жϵ�ǰ�ļ��Ƿ���֧�ֵ��ļ���ʽ��
    *  Ŀǰ֧�ֵ��ļ���ʽ�У�MP4��AVI��MKV��FLV��MOV��MP3��WAV
    */
    virtual bool IsSupportFormat(const QString filePath) ;

private:
    void MediaListFileWrite(QJsonArray value);
private:
    bool bInit = false;
    QString mAppid;
    //QString mThridPartyUserId;
    QString mInteractionRoomId;
    QString mAccessToken;
	QString mStrLiveRoomId;
	//QString mJoinUid;

	QMutex mStreamMutexCS;
    QString mCurrentPenUser;

    //std::atomic_bool mbIsHost = false;
    std::atomic_bool mbIsLiving = false;

    std::atomic_bool mbSpeaking = false;

    std::atomic_bool mbIsMuteMic = false;
    std::atomic_bool mbIsMuteCamera = false;

    std::atomic_bool mbMyCameraOpen = true;
    std::atomic_bool mbMutiClassOtherCameraOpen = true;

    QString mTeacherJoinId;
	 QString mstrCurVersion;
};
