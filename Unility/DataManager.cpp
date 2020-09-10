#include "DataManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QApplication>
#include "vhall_log.h"
#include "ConfigSetting.h"
#include <QDir>


DataManager::DataManager(QObject *parent)
    : QObject(parent)
{

}

DataManager::~DataManager()
{
}

bool DataManager::Init(std::wstring logPath) {
    InitLog(logPath);
    return true;
}

bool DataManager::Init()
{
   return false;
}

void DataManager::WriteLog(char* pszMessage, ...) {
	if (NULL == pszMessage) return;
    va_list vargs;
    QString strTmp, strRet;
    va_start(vargs, pszMessage);
    strRet += strTmp.vsprintf(pszMessage, vargs);
    va_end(vargs);
    LOGD(strRet.toLatin1().data());
}

void DataManager::SetVersion(QString appid)
{
	mstrCurVersion = appid;
}

QString DataManager::GetVersion()
{
	return mstrCurVersion;
}

void DataManager::SetAppid(QString appid) {
    this->mAppid = appid;
}

QString DataManager::GetAppid() {
    return this->mAppid;
}


//void DataManager::SetJoinUid(QString appid) {
//	this->mJoinUid = appid;
//}
//
//QString DataManager::GetJoinUid() {
//	return this->mJoinUid;
//}

//void DataManager::SetThridPartyUserId(QString third_party_user_id) {
//    this->mThridPartyUserId = third_party_user_id;
//}
//QString DataManager::GetThridPartyUserId() {
//    return this->mThridPartyUserId;
//}

void DataManager::SetInteractionRoomId(QString roomid) {
    this->mInteractionRoomId = roomid;
}
QString DataManager::GetInteractionRoomId() {
    return this->mInteractionRoomId;
}

void DataManager::SetAccessToken(QString token) {
    this->mAccessToken = token;
}
QString DataManager::GetAccessToken() {
    return  this->mAccessToken;
}

QString DataManager::GetStartUrl() {
    return  "";//UrlGenerator::GetInvaStartUrl();
}

QString DataManager::GetInavInfoUrl() {
    return  "";//UrlGenerator::GetInvaStartUrl();
}

bool DataManager::GetIsSpeaking()
{
   return mbSpeaking;
}

void DataManager::SetIsSpeaking(const bool& bIsLiving /*= false*/)
{
   mbSpeaking = bIsLiving;
}

QString DataManager::GetLiveRoomId()
{
	return mStrLiveRoomId;
}

void DataManager::SetLiveRoomId(QString roomId)
{
	mStrLiveRoomId = roomId;
}

//void DataManager::SetPushStreamPermission(bool host) {
//    mbIsHost = host;
//}
//
//bool DataManager::GetPushStreamPermission() {
//    return mbIsHost;
//}

void DataManager::SetPublished(bool bIsLiving /*= false*/) {
	mbIsLiving = bIsLiving;
}

bool DataManager::IsAlreadyPublished(){
	return mbIsLiving;
}

void DataManager::SetMuteMicByTeacher(bool mute) {
    mbIsMuteMic = mute;
}

bool DataManager::GetMuteMicByTeacherState() {
    LOGD("mbIsMuteMic:%d", (int)mbIsMuteMic);
    return mbIsMuteMic;
}

void DataManager::SetMuteCameraByTeacher(bool mute) {
    mbIsMuteCamera = mute;
}

void DataManager::SetCurrentPenUser(const QString& user_id) {
    mCurrentPenUser = user_id;
}

QString DataManager::GetCurrentPenUser() {
    return mCurrentPenUser;
}

bool DataManager::GetMuteCameraByTeacherState() {  
    LOGD("mbIsMuteCamera:%d", (int)mbIsMuteCamera);
    return mbIsMuteCamera;
}

QList<QString> DataManager::GetMediaPlayFileList() {
	QList<QString> fileList;
    QString playfile = QDir::tempPath() + PLAY_FILE_LIST;
    QJsonArray array = QJsonArray();
    QFile f;
    f.setFileName(playfile);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray ba = f.readAll();
        f.close();
        QJsonDocument doc = QJsonDocument::fromJson(ba);
        array = doc.array();
        for (int i = 0; i < array.count(); i++) {
            QString filename = array[i].toString();
            fileList.push_back(filename);
        }
    }
    return fileList;
}
/*
* 删除插播文件
*/
void  DataManager::RemoveMediaPlayFile(QString removeFile) {
    QList<QString> fileList = GetMediaPlayFileList();
    QJsonArray array;
	QList<QString>::iterator iter = fileList.begin();
    while (iter != fileList.end()) {
        QString filename = *iter;
        //如果删除的文件已经存在，那么不再存储该文件。
        if (!removeFile.isEmpty() && removeFile.compare(filename) != 0) {
            array.append(filename);
        }
        iter++;
    }
    MediaListFileWrite(array);
}
/**
* 添加插播文件
*/
void DataManager::AddMediaPlayFile(QString addFile) {
    bool bSameFile = false;
    QList<QString> fileList = GetMediaPlayFileList();
    QJsonArray array;
	QList<QString>::iterator iter = fileList.begin();
    while (iter != fileList.end()) {
        QString filename = *iter;
        array.append(filename);
        //如果添加的文件没有找到，则添加新文件到列表中。否则不存储
        if (!addFile.isEmpty() && addFile.compare(filename) == 0) {
            bSameFile = true;
            break;
        }
        iter++;
    }
    if (!bSameFile) {
        array.append(addFile);
        MediaListFileWrite(array);
    }
}

void DataManager::MediaListFileWrite(QJsonArray value) {
    QString filename = QDir::tempPath() + PLAY_FILE_LIST;
    QFile f;
    f.setFileName(filename);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc;
        doc.setArray(value);
        QByteArray ba = doc.toJson();
        f.write(ba);
        f.close();
    }
}

bool DataManager::IsFileExist(const QString filePath) {
    QFileInfo fileDir(filePath);
    if (fileDir.exists()) {
        return true;
    }
    return false;
}
/*
*  判断当前文件是否是支持的文件格式。
*  目前支持的文件格式有：MP4、AVI、MKV、FLV、MOV、MP3、WAV
*/
bool DataManager::IsSupportFormat(const QString filePath) {
    int lastPos = filePath.lastIndexOf(".");
    QString format = filePath.mid(lastPos, filePath.length());
    format = format.toLower();
    if (format.contains("mp4") || format.contains("avi") || format.contains("mkv") || format.contains("flv") || format.contains("mov") || format.contains("mp3") || format.contains("wav")) {
        return true;
    }
    return false;
}

void DataManager::SetTeacherJoinId(const QString& user_id) {
    mTeacherJoinId = user_id;
}

QString DataManager::GetTeacherJoinId() {
    return mTeacherJoinId;
}


bool DataManager::IsMyCameraOpen() {
   return mbMyCameraOpen;
}
                         
void DataManager::SetMyCameraOpen(bool open) {
   mbMyCameraOpen = open; //自己摄像头开关状态
}

bool DataManager::IsMultiClassOtherUserCameraOpen() {
   return mbMutiClassOtherCameraOpen;
}

void DataManager::SetMultiClassOtherUserCameraOpen(bool open) {
   mbMutiClassOtherCameraOpen = open;  //对方摄像头开关状态
}