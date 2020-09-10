#pragma once

#include <QDialog>
#include <QMutex>
#include <QTimer>
#include <QThread>
#include <QList>
#include "../Unility/Unility.h"
#include "ui_VhallIALiveSettingDlg.h"
#include "VhallUI_define.h"
#include "LiveEventInterface.h"


class VhallWaiting;
class VhallIALiveSettingDlg ;
class NoiseTooltip;
//class NoiseTooltip;

enum SettingPageIndex {
	video_page = 0,
	audio_page = 1,
	layout_page = 2,
	aboutUs_page = 3,
};

enum CloseType {
   CloseWithSave = 0,
   CloseNone = 1,
   CloseWithSaveLayoutMode = 2,
};

class DevInfo
{
public:
    DevInfo();
    DevInfo(const QString& strId, const int& iIdex);
	//CameraInfo(const QString& strId, const int& iIdex, std::list<vhall::VideoDevProperty> DevPropertyList);
    DevInfo(const QString& strId, const int& iIdex, std::vector<std::shared_ptr<vhall::VideoFormat>> DevPropertyList);
    DevInfo(const DevInfo& Info);
	~DevInfo();
	void init(std::vector<std::shared_ptr<vhall::VideoFormat>> ver);
	QList<vhall::VideoFormat> FormatList()
	{
		return m_FormatList;
	}
	int index() {
		return m_iIdex;
	}
	QString Id() {
		return m_strId;
	}
private:
	QString m_strId = "";
	int m_iIdex=-1;
	QList<vhall::VideoFormat> m_FormatList;
};
Q_DECLARE_METATYPE(DevInfo)
Q_DECLARE_METATYPE(vhall::VideoDevProperty)
Q_DECLARE_METATYPE(vhall::VideoFormat)

class VhallIALiveSettingDlg : public CBaseWnd
{
    Q_OBJECT

public:
	VhallIALiveSettingDlg(QWidget *parent = nullptr, bool host = false);
	~VhallIALiveSettingDlg();
	CREATE_WND();

   int GetLayOutMode();
   void SetLayOutMode(int mode);
   void SetLastLayoutMode();

   virtual void Notice(const CSTDMapParam &);
   virtual void Notice(const CSTDMapParamPtr &);
   void init();
   void RegisterListener(DeviceChangedListener* listener);
   void Apply();
   void HandelCustomEvent(QEvent*);
   void SetStreamType();
   static bool isEnableDoublePublish();
	bool IsOpenDesktopCapture();
	void SetOpenDesktopCapture(bool bDesktopCapture);
	void SetAllSubScribeAudio(bool bmute);
	void RecoverState();
	int GetCurrentMicIndex();
	DevInfo GetCurrentPlayerIndex();
signals:
	void sigPicPush(const bool&);
	void sigNoMicAndPlayer(const QString&);
public Q_SLOTS:
	virtual int exec();
protected:
	virtual void customEvent(QEvent*);
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent*);
	virtual bool eventFilter(QObject * watched, QEvent * event);
private slots:

   void slot_OnClickedScreenShareCameraTopLayout();
   void slot_OnClickedScreenShareCameraBottomLayout();
   void slot_OnClickedScreenShareOnlyLayout();
	void slot_comboBox_Quality_currentIndexChanged(int index);
    void slot_TestPlay();
    void slot_OnClickedApply();
    void slot_OnClickedClose();
    void slot_CurrentCameraSelectChanged(int);
    void slot_CurrentMicSelectChanged(int);
    void slot_CurrentPlayerSelectChanged(int);
    void slot_MicValueChanged(int);
    void slot_OnClickedAudioTab();
    void slot_OnClickedVideoTab();
	void slot_OnClickedAAboutTab();
    void slot_OnClickedSelectPic();
    void slot_OpenDesktopAudio(bool checked);
    void slot_OpenPushPic(bool checked);
    void slot_RadioCamera(bool checked);
    void slot_CurrentDeskTopCaptureChanged(int);
    void slot_DesktopCaptureValueChanged(int);

    void on_btnMixedLayoutTiled_clicked(bool checked = false);
    void on_btnMixedLayoutGrid_clicked(bool checked = false);
	void slot__Adaptive(bool badaptive);
private:
    void StartPreView();
    void DisableAllControl();
    void ReleaseResource();

    void LoadPrivewPic(const QString picPath);
	void setOpenDesktopCaptureChecked(const bool& bChecked);
private:
   //void setMiedLayout(const int& layout);
	Ui::VhallIALiveSettingDlg ui;

    QString mCurCameraDevID;
    QString mCurMicDevID;
    QString mCurPlayerID;
    bool mbInitSuc = false;

    std::list<vhall::AudioDevProperty> micList;
    std::list<vhall::VideoDevProperty> mCameraList;
    std::list<vhall::AudioDevProperty> mPlayerList;
    DeviceChangedListener* mpDevChangedListener = NULL;
	bool mbPicPush = false;
   bool mbCurrentCameraCanOpen = true;
   NoiseTooltip* m_pNoiseTips = NULL;
   int miMixedLayoutMode = -1;
   bool mbShareScreenLayoutHasCamera = false;
};