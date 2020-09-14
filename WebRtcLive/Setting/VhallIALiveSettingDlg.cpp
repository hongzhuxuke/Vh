#include "VhallIALiveSettingDlg.h"
#include "DebugTrace.h"
#include "Pathmanager.h"
#include "ConfigSetting.h"     
#include "../Unility/CustomEventType.h"
#include "../Unility/Unility.h"
#include <QVariant>
#include <QCloseEvent>
#include <QPainter>
#include <QAbstractItemView>
#include <QComboBox>
#include "NoiseTooltip.h"

#define AUDIO_FILE  "yisell_sound_48k.pcm"
#define CameraOpenSuccess   QStringLiteral("border-image: url(:/ctrl/img/ctrl/media_setting_01.jpg);")
#define CameraOpenFailed    QStringLiteral("border-image: url(:/ctrl/img/ctrl/media_setting_02.jpg);")

#define SCRREN_LAYOUT_TOP_SELECT QStringLiteral("border:1px solid  rgb(82, 204, 144);image: url(:/button/img/button/camera_top_layout.png);padding:2px;border-radius:4px;")
#define SCRREN_LAYOUT_TOP_NO_SELECT QStringLiteral("border:1px solid  rgb(226, 226, 226);image: url(:/button/img/button/camera_top_layout.png);padding:2px;border-radius:4px;")
//#define SCRREN_LAYOUT_BOTTOM_SELECT QStringLiteral("border:1px solid  rgb(82, 204, 144);image: url(:/button/img/button/camera_right_bottom.png);padding:2px;border-radius:4px;")


#define SCRREN_LAYOUT_BOTTOM_SELECT QStringLiteral("border:1px solid  rgb(82, 204, 144);image: url(:/button/img/button/camera_right_bottom.png);padding:2px;border-radius:4px;")
#define SCRREN_LAYOUT_BOTTOM_NO_SELECT QStringLiteral("border:1px solid  rgb(226, 226, 226);image: url(:/button/img/button/camera_right_bottom.png);padding:2px;border-radius:4px;")

#define SCRREN_LAYOUT_NORMAL_SELECT QStringLiteral("border:1px solid  rgb(82, 204, 144);image: url(:/button/img/button/camera_no_layout.png);padding:2px;border-radius:4px;")
#define SCRREN_LAYOUT_NORMAL_NO_SELECT QStringLiteral("border:1px solid  rgb(226, 226, 226);image: url(:/button/img/button/camera_no_layout.png);padding:2px;border-radius:4px;")
#define MIXED_LAYOUT_TILED_SELECT QStringLiteral("border:1px solid  rgb(82, 204, 144);\
    image: url(:/button/img/button/layoutTiled.png);\
    padding:2px;\
       border-radius:4px;")

#define MIXED_LAYOUT_TILED_NO_SELECT QStringLiteral("border:1px solid  rgb(226, 226, 226);\
    image: url(:/button/img/button/layoutTiled.png);\
    padding:2px;\
       border-radius:4px;")

#define MIXED_LAYOUT_GRID_SELECT QStringLiteral("border:1px solid  rgb(82, 204, 144);\
    image: url(:/button/img/button/layoutGrid.png);\
    padding:2px;\
       border-radius:4px;")

#define MIXED_LAYOUT_GRID_NO_SELECT QStringLiteral("border:1px solid  rgb(226, 226, 226);\
    image: url(:/button/img/button/layoutGrid.png);\
    padding:2px;\
       border-radius:4px;")

extern ToolManager *globalToolManager;

CREATE_WND_FUNCTION(VhallIALiveSettingDlg);

DevInfo::DevInfo()
{

}
DevInfo::~DevInfo()
{
    m_strId = "";
    m_iIdex = -1;
    m_FormatList.clear();
}
DevInfo::DevInfo(const QString& strId, const int& iIdex)
{
	m_strId = strId;
	m_iIdex = iIdex;
}
DevInfo::DevInfo(const QString& strId, const int& iIdex, std::vector<std::shared_ptr<vhall::VideoFormat>> FormatList)
{
	m_strId = strId;
	m_iIdex = iIdex;
	init(FormatList);
}

DevInfo::DevInfo(const DevInfo& Info)
{
	m_strId = Info.m_strId;
	m_iIdex = Info.m_iIdex;
	vhall::VideoFormat dev;
	foreach(dev, Info.m_FormatList)
	{
		m_FormatList.append(dev);
	}
}

void DevInfo::init(std::vector<std::shared_ptr<vhall::VideoFormat>> ver)
{
	std::vector<std::shared_ptr<vhall::VideoFormat>>::iterator it = ver.begin();
	while (it != ver.end())
	{
		vhall::VideoFormat format;
		format.width = (*it)->width;
		format.height = (*it)->height;
		format.maxFPS = (*it)->maxFPS;
		format.videoType = (*it)->videoType;
		format.interlaced = (*it)->interlaced;
		m_FormatList.append(format);
		it++;
	}
}

VhallIALiveSettingDlg::VhallIALiveSettingDlg(QWidget *parent,bool host)
	: CBaseWnd(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Dialog /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);

    globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
    this->setWindowTitle(QStringLiteral("媒体设置"));
        
    ui.widget_micVolume->SetVolumeType(VolumeType_PreviewMic);
    ui.widget_playFile->SetVolumeType(VolumeType_Play_File); 
	 //ui.label_message->setVisible(false);
	
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(slot_OnClickedApply()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_OnClickedClose()));
	connect(ui.comboBox_camera, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentCameraSelectChanged(int)));
	connect(ui.comboBox_micDev, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentMicSelectChanged(int)));
	connect(ui.comboBox_playerDev, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentPlayerSelectChanged(int)));
	connect(ui.comboBox_deskTopCapture, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentDeskTopCaptureChanged(int)));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_TestPlay()));
	connect(ui.horizontalSlider_mic, SIGNAL(valueChanged(int)), this, SLOT(slot_MicValueChanged(int)));
	connect(ui.pushButton_video_tab, SIGNAL(clicked()), this, SLOT(slot_OnClickedVideoTab()));
	connect(ui.pushButton_audio_tab, SIGNAL(clicked()), this, SLOT(slot_OnClickedAudioTab())); 
	connect(ui.pushButton_about_tab, SIGNAL(clicked()), this, SLOT(slot_OnClickedAAboutTab()));
	connect(ui.pushButton_selectPic, SIGNAL(clicked()), this, SLOT(slot_OnClickedSelectPic()));
	connect(ui.radioButton_pic, SIGNAL(clicked(bool)), this, SLOT(slot_OpenPushPic(bool)));
	connect(ui.radioButton_camera, SIGNAL(clicked(bool)), this, SLOT(slot_RadioCamera(bool)));
	connect(ui.checkBox_openDesktopCapture, SIGNAL(clicked(bool)), this, SLOT(slot_OpenDesktopAudio(bool)));
	connect(ui.horizontalSlider_deskTopCaptureVol, SIGNAL(valueChanged(int)), this, SLOT(slot_DesktopCaptureValueChanged(int))); 
	connect(ui.checkBox_Adaptive, SIGNAL(clicked(bool)), this, SLOT(slot__Adaptive(bool)));

   connect(ui.pushButton_top_layout, SIGNAL(clicked()), this, SLOT(slot_OnClickedScreenShareCameraTopLayout()));
//   connect(ui.pushButton_bottom_layout, SIGNAL(clicked()), this, SLOT(slot_OnClickedScreenShareCameraBottomLayout()));
   connect(ui.pushButton_normal_layout, SIGNAL(clicked()), this, SLOT(slot_OnClickedScreenShareOnlyLayout()));


	ui.pushButton_audio_tab->setFocusPolicy(Qt::NoFocus);
	ui.pushButton_video_tab->setFocusPolicy(Qt::NoFocus);
	ui.pushButton_ok->setFocusPolicy(Qt::NoFocus);
	ui.radioButton_camera->setFocusPolicy(Qt::NoFocus);
	ui.radioButton_pic->setFocusPolicy(Qt::NoFocus);
	ui.pushButton_close->setFocusPolicy(Qt::NoFocus);
	ui.pushButton_selectPic->setFocusPolicy(Qt::NoFocus);
	ui.checkBox_openDesktopCapture->setFocusPolicy(Qt::NoFocus);
	ui.checkBox_muteRemote->setFocusPolicy(Qt::NoFocus);
	ui.horizontalSlider_deskTopCaptureVol->setFocusPolicy(Qt::NoFocus);
	ui.horizontalSlider_mic->setFocusPolicy(Qt::NoFocus);

   ui.comboBox_camera->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
   ui.comboBox_camera->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
	ui.comboBox_camera->installEventFilter(this);
   ui.comboBox_Quality->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
   ui.comboBox_Quality->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

   ui.comboBox_micDev->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
   ui.comboBox_micDev->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

   ui.comboBox_playerDev->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
   ui.comboBox_playerDev->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
	init();
   ui.labLayout->installEventFilter(this); //音频码率

   if (m_pNoiseTips == NULL) {
      m_pNoiseTips = new NoiseTooltip();
   }

   ui.lineEdit_picPath->setEnabled(false);
}

VhallIALiveSettingDlg::~VhallIALiveSettingDlg()
{
   if (m_pNoiseTips != NULL) {
      delete m_pNoiseTips;
      m_pNoiseTips = NULL;
   }
}

void VhallIALiveSettingDlg::init(){
    //从互动页面进入。如果角色不是主持人也不显示设置清晰度。
    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
    //QString configCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, "");
    //QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");
    //QString configPlayId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, "");
    QString confPicPaht = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PUSH_PIC_PATH, "");
    //QString confDesktopCaptureId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, "");
		//自适应屏幕
	QString confadaptive = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, SCREEN_ADAPTIVE, "");
	if (confadaptive == "1") {
		ui.checkBox_Adaptive->setChecked(true);
	}
	else {
		ui.checkBox_Adaptive->setChecked(false);
	}
    int openDesktopValue = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_VOLUME, 100);

	//版本号
	QString version = globalToolManager->GetDataManager()->GetVersion();
	if (!version.isEmpty()) {
		int iPos = version.lastIndexOf(".");
		QString midVersion = version.mid(0, iPos);//取版本前两位 1.0
		//QString class_conf = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
		QString class_conf = globalToolManager->GetToolConfigPath();
		int hideLogo = ConfigSetting::ReadInt(class_conf, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
		if (hideLogo) {
			ui.label_versions->setText(QStringLiteral("当前版本：微吼课堂V%1").arg(midVersion));
		}
		else {
			ui.label_versions->setText(QStringLiteral("当前版本：微吼课堂V%1").arg(midVersion));
		}
	}

    if (confPicPaht.isEmpty()) {
        QString defaultPic = QApplication::applicationDirPath() + "/" + QString::fromStdString("default_pushPic.png");
        LoadPrivewPic(defaultPic);
        ui.lineEdit_picPath->setText(defaultPic);
    }
    else {
        LoadPrivewPic(confPicPaht);
        ui.lineEdit_picPath->setText(confPicPaht);
    }

    int openPic = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 0);
    if (openPic) {
        ui.stackedWidget_preview->setCurrentIndex(2);
        ui.radioButton_pic->setChecked(true);
        ui.radioButton_camera->setChecked(false);
		//ui.lineEdit_picPath->setEnabled(true);
		ui.pushButton_selectPic->setEnabled(true);
		mbPicPush = true;
    }
    else {
        ui.stackedWidget_preview->setCurrentIndex(0);
        ui.radioButton_pic->setChecked(false);
        ui.radioButton_camera->setChecked(true);
		ui.lineEdit_picPath->setEnabled(false);
		ui.pushButton_selectPic->setEnabled(false);
    }
    slot_OnClickedVideoTab();
	int openDesktopCapture = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, 0);
	int muteRemote = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);

    if (1==openDesktopCapture && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
        //ui.checkBox_openDesktopCapture->setChecked(true);
		setOpenDesktopCaptureChecked(true);
        ui.horizontalSlider_deskTopCaptureVol->setValue(openDesktopValue);
        if (muteRemote == 1) {
            ui.checkBox_muteRemote->setChecked(true);
        }
        else {
            ui.checkBox_muteRemote->setChecked(false);
        }
    }
    else {
		setOpenDesktopCaptureChecked(false);
        //ui.checkBox_openDesktopCapture->setChecked(false);
        //ui.checkBox_muteRemote->setChecked(false);
		
    }

	if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher)
	{
		ui.widget_desktopCapture->setVisible(false);
		ui.widget_8->setVisible(false);
	}
}

void VhallIALiveSettingDlg::setOpenDesktopCaptureChecked(const bool& bChecked )
{
	ui.checkBox_openDesktopCapture->setChecked(bChecked);
	if (bChecked)
	{
		ui.comboBox_deskTopCapture->setEnabled(true);
		ui.horizontalSlider_deskTopCaptureVol->setEnabled(true);
		ui.checkBox_muteRemote->setEnabled(true);
	}
	else
	{
		ui.comboBox_deskTopCapture->setEnabled(false);
		ui.horizontalSlider_deskTopCaptureVol->setEnabled(false);
		ui.checkBox_muteRemote->setEnabled(false);
		ui.checkBox_muteRemote->setChecked(false);
	}
}

void VhallIALiveSettingDlg::SetStreamType()
{
	if (mbPicPush)
	{
		emit sigPicPush(mbPicPush);
	}
}

bool VhallIALiveSettingDlg::isEnableDoublePublish()
{
   bool bRef = false;
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
   QString msConfPath = globalToolManager->GetConfigPath();
   QString strOpePic = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, "").trimmed();

   std::list<vhall::VideoDevProperty> CameraList;
   VHClassRoomInterface* pInterActionRoom = globalToolManager->GetClassSDK();
   if (pInterActionRoom) {
      pInterActionRoom->GetCameraDevices(CameraList);
      globalToolManager->GetDataManager()->WriteLog("%s GetCameraDevices %d", __FUNCTION__, CameraList.size());
   }

   
   if ((strOpePic.compare("1") == 0 && CameraList.size() > 0) || CameraList.size() > 1){//正在图片采集 一个以上摄像头可以双推/ 两个个以上摄像头可以双推
      bRef = true;
   }

   return bRef;
}

int VhallIALiveSettingDlg::exec()
{
   
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
	QString configCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, "");
	QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");
	QString configPlayId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, "");
	QString confDesktopCaptureId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_DEVID, "");

   //int layout = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_SHARE_LAYOUT, ShareScreenLayout_on_camera);
   if (baseInfo.mWebinarInfo.desktop_share_layout_mode == ShareScreenLayout_camera_top) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {//公开课
         ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_TOP_SELECT);
      }
      else
      {
         ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_BOTTOM_SELECT);
      }

      ui.pushButton_normal_layout->setStyleSheet(SCRREN_LAYOUT_NORMAL_NO_SELECT);
      mbShareScreenLayoutHasCamera = true;
   }
   else {
      
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {//公开课
         ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_TOP_NO_SELECT);
      }
      else
      {
         ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_BOTTOM_NO_SELECT);
      }


      ui.pushButton_normal_layout->setStyleSheet(SCRREN_LAYOUT_NORMAL_SELECT);
      mbShareScreenLayoutHasCamera = false;
   }

	globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
	bool bHasDevice = false;
	VHClassRoomInterface* pInterActionRoom = globalToolManager->GetClassSDK();
	if (pInterActionRoom) {
		disconnect(ui.comboBox_camera, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentCameraSelectChanged(int)));
		ui.comboBox_playerDev->clear();
		ui.comboBox_deskTopCapture->clear();
		mCameraList.clear();
		globalToolManager->GetDataManager()->WriteLog("%s GetCameraDevices", __FUNCTION__);
		pInterActionRoom->GetCameraDevices(mCameraList);
		//摄像设备显示
		ui.comboBox_camera->clear();
		if (mCameraList.size() == 0) {
			ui.comboBox_camera->addItem(QStringLiteral("无摄像头设备"));
		}
		else {
			int nCount = 0;
			int currentIndex = 0;
			std::list<vhall::VideoDevProperty>::iterator it = mCameraList.begin();
			for (; it != mCameraList.end(); it++) {
				ui.comboBox_camera->addItem(QString::fromStdString(it->mDevName), QVariant::fromValue(*it));
				if ((*it).mDevId == configCameraId.toStdString()) {
					currentIndex = nCount;
				}
				nCount++;
			}
			ui.comboBox_camera->setCurrentIndex(currentIndex);
			ui.comboBox_camera->setEnabled(true);
			bHasDevice = true;
		}

		connect(ui.comboBox_camera, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentCameraSelectChanged(int)));
		//麦克风显示
		int currentMicIndex = 0;
		std::string micDevId;
		micList.clear();
		globalToolManager->GetDataManager()->WriteLog("%s GetMicDevices", __FUNCTION__);
		pInterActionRoom->GetMicDevices(micList);
		ui.comboBox_micDev->clear();
		if (micList.size() == 0) {
			ui.comboBox_micDev->addItem(QStringLiteral("无音频输入设备"));
			ui.horizontalSlider_mic->setEnabled(false);
		}
		else {
			ui.horizontalSlider_mic->setEnabled(true);
			int nCount = 0;
			std::list<vhall::AudioDevProperty>::iterator itMic = micList.begin();
			while (itMic != micList.end()) {
				if (itMic == micList.begin()) {
					micDevId = QString::fromStdWString(itMic->mDevGuid).toStdString();
				}
				DevInfo info(QString::fromStdWString(itMic->mDevGuid), itMic->mIndex);
				ui.comboBox_micDev->addItem(QString::fromStdWString(itMic->mDevName), QVariant::fromValue(info));
				if (QString::fromStdWString(itMic->mDevGuid).toStdString() == configMicId.toStdString()) {
					currentMicIndex = nCount;
					micDevId = QString::fromStdWString(itMic->mDevGuid).toStdString();
				}
				itMic++;
				nCount++;
			}
			ui.comboBox_micDev->setCurrentIndex(currentMicIndex);
			ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, QString::fromStdString(micDevId));
			bHasDevice = true;
		}

		//扬声器
		mPlayerList.clear();
		globalToolManager->GetDataManager()->WriteLog("%s GetPlayerDevices", __FUNCTION__);
		pInterActionRoom->GetPlayerDevices(mPlayerList);
		ui.comboBox_playerDev->clear();

		if (mPlayerList.size() == 0) {
			ui.comboBox_playerDev->addItem(QStringLiteral("无音频输入设备"));
		}
		else {
			int nCount = 0;
			int currentIndex = 0;
			int desktopCaptureIndex = 0;
			std::wstring desktopCaptureDevId;
			std::list<vhall::AudioDevProperty>::iterator player = mPlayerList.begin();
			while (player != mPlayerList.end()) {
				if (player == mPlayerList.begin()) {
					desktopCaptureDevId = player->mDevGuid;
				}
				DevInfo info(QString::fromStdWString(player->mDevGuid), player->mIndex);
				ui.comboBox_playerDev->addItem(QString::fromStdWString(player->mDevName), QVariant::fromValue(info));
				ui.comboBox_deskTopCapture->addItem(QString::fromStdWString(player->mDevName), QVariant::fromValue(info));
				if (QString::fromStdWString(player->mDevGuid).toStdString() == configPlayId.toStdString()) {
					currentIndex = nCount;
				}
				if (QString::fromStdWString(player->mDevGuid).toStdString() == confDesktopCaptureId.toStdString()) {
					desktopCaptureIndex = nCount;
					desktopCaptureDevId = player->mDevGuid;
				}
				player++;
				nCount++;
			}
			ui.comboBox_playerDev->setCurrentIndex(currentIndex);
			ui.comboBox_deskTopCapture->setCurrentIndex(desktopCaptureIndex);
		}

		if (ui.checkBox_openDesktopCapture->isChecked() && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
			DevInfo playerInfo;
			playerInfo = ui.comboBox_deskTopCapture->currentData().value<DevInfo>();
			int nRet = globalToolManager->GetClassSDK()->StartLocalCapturePlayer(currentMicIndex, playerInfo.Id().toStdWString(), ui.horizontalSlider_deskTopCaptureVol->value());
			//if (nRet == 0) {
			//	bool mute = ui.checkBox_muteRemote->isChecked();
			//	if (mute) {
			//		globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(true);
			//	}
			//}
		}
		else {
			globalToolManager->GetClassSDK()->SetUsedMic(currentMicIndex, micDevId, L"");
			SetOpenDesktopCapture(false);
			//globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(false);
		}

		StartPreView();
      bool bEnableDoublePub = VhallIALiveSettingDlg::isEnableDoublePublish();
		globalToolManager->GetClassSDK()->UpdatePublishAbility(bHasDevice, bEnableDoublePub);
	}

  // return CBaseWnd::exec();
	int value = globalToolManager->GetClassSDK()->GetCurrentMicVol();
	ui.horizontalSlider_mic->setValue(value);
	value = globalToolManager->GetClassSDK()->GetCurrentPlayVol();
	//如果麦克风或扬声器为0 不可共享桌面音频
	if (micList.size() == 0 || mPlayerList.size() == 0) {
		ui.checkBox_muteRemote->setChecked(false);
		ui.checkBox_muteRemote->setEnabled(false);
		ui.checkBox_openDesktopCapture->setChecked(false);
		ui.checkBox_openDesktopCapture->setEnabled(false);
		ui.comboBox_deskTopCapture->setEnabled(false);
		ui.horizontalSlider_deskTopCaptureVol->setEnabled(false);		
	}
	else {
		ui.checkBox_openDesktopCapture->setEnabled(true);
		if (ui.checkBox_openDesktopCapture->isChecked()) {
			ui.comboBox_deskTopCapture->setEnabled(true);
			ui.horizontalSlider_deskTopCaptureVol->setEnabled(true);
			ui.checkBox_muteRemote->setEnabled(true);
		}
	}

	return CBaseWnd::exec();
}
void VhallIALiveSettingDlg::StartPreView() {
   //return;
    globalToolManager->GetDataManager()->WriteLog("%s ", __FUNCTION__);
    if (mCameraList.size() > 0) {
        vhall::VideoDevProperty dev;
        dev = ui.comboBox_camera->currentData().value<vhall::VideoDevProperty>();
        std::string devId = dev.mDevId;
		QString msConfPath = globalToolManager->GetConfigPath();

        HWND id = (HWND)(ui.widget_render->winId());
        if (micList.size() > 0) {
            DevInfo info;
            info = ui.comboBox_micDev->currentData().value<DevInfo>();
            if (globalToolManager->GetClassSDK()->PreviewCamera((void*)id, devId, RTC_VIDEO_PROFILE_480P_4x3_M, info.index()) == 0) {
                ui.comboBox_camera->setEnabled(false);
            }
        }
        else {
            if (globalToolManager->GetClassSDK()->PreviewCamera((void*)id, devId, RTC_VIDEO_PROFILE_480P_4x3_M) == 0) {
                ui.comboBox_camera->setEnabled(false);
            }
        }
    }
    globalToolManager->GetDataManager()->WriteLog("%s end", __FUNCTION__);
}

void VhallIALiveSettingDlg::RegisterListener(DeviceChangedListener* listener) {
	mpDevChangedListener = listener;
}

void VhallIALiveSettingDlg::Notice(const CSTDMapParam &mapParam) {

   void (QComboBox:: *IndexChanged)(int index) = &QComboBox::currentIndexChanged;

   disconnect(ui.comboBox_Quality, IndexChanged, this,  &VhallIALiveSettingDlg::slot_comboBox_Quality_currentIndexChanged);
    std::map<QString, QString>::const_iterator iter = mapParam.find(Str_Function);
	 init();
	 ui.comboBox_Quality->clear();
    if (iter != mapParam.end() && iter->second == Str_Function_FromLogin) {
        //从登陆界面不显示设置清晰度，因为此时还不知道是不是主持人。
        ui.widget_mix_def->hide();
        ui.widget_desktopCapture->hide();
        ui.widget_screen_layout->hide();
        ui.widget_screen_layout_tips->hide();
        ui.mixedLayoutWdg->hide();
		//init();
    }
	else
	{
		std::map<QString, QString>::const_iterator iterQuality = mapParam.find(Str_Quality_Enabeld);
		if (iterQuality != mapParam.end() /*&& iter->second == Str_Function_FromLogin*/)
		{
			if (iterQuality->second == Str_Quality_Enabeld)
			{
				ui.comboBox_Quality->setEnabled(true);
			}
			else
			{
				ui.comboBox_Quality->setEnabled(false);
			}
		}
		else {
			//init();
			ui.widget_mix_def->show();
			//ui.widget_desktopCapture->show();
			//从互动页面进入。如果角色不是主持人也不显示设置清晰度。
			ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
			if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
				ui.widget_mix_def->hide();
            ui.widget_screen_layout->hide();
            ui.widget_screen_layout_tips->hide();       
            ui.mixedLayoutWdg->hide();
			}
         else if(baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher){
            ui.widget_screen_layout->show();
            ui.widget_screen_layout_tips->show();
            

            if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) 
            {
               ui.mixedLayoutWdg->hide();
            }
            else {//小课堂
               //ui.widget_screen_layout->hide();
               //ui.widget_screen_layout_tips->hide();
               ui.mixedLayoutWdg->show();
               
               ui.btnMixedLayoutTiled->SetCursorType(CDisEnablePushButton::eCursorType_PointingHandCursor);
               ui.btnMixedLayoutGrid->SetCursorType(CDisEnablePushButton::eCursorType_PointingHandCursor);

               if (eMixedLayoutMode_Tiled == baseInfo.mWebinarInfo.mixedLayout) {
                  ui.btnMixedLayoutTiled->setStyleSheet(MIXED_LAYOUT_TILED_SELECT);
                  ui.btnMixedLayoutGrid->setStyleSheet(MIXED_LAYOUT_GRID_NO_SELECT);
                  miMixedLayoutMode = eMixedLayoutMode_Tiled;
                  ui.btnMixedLayoutTiled->SetDisEnable(false);
                  
                  ui.btnMixedLayoutGrid->SetDisEnable(baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS);
               }
               else {
                  ui.btnMixedLayoutTiled->setStyleSheet(MIXED_LAYOUT_TILED_NO_SELECT);
                  ui.btnMixedLayoutGrid->setStyleSheet(MIXED_LAYOUT_GRID_SELECT);
                  miMixedLayoutMode = eMixedLayoutMode_Grid;
                  ui.btnMixedLayoutTiled->SetDisEnable(baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS);
                  ui.btnMixedLayoutGrid->SetDisEnable(false);
               }

               //if (CLASS_STATUS_IN_CLASS == baseInfo.mWebinarInfo.type){

               //}

               if (CLASS_TYPE_SMALL == baseInfo.mWebinarInfo.course_type
                  && CLASS_JOIN_MEMBER_1V12 == baseInfo.mWebinarInfo.linkmic_apply_type)
               {
                  ui.btnMixedLayoutGrid->setEnabled(false);
                  if (eMixedLayoutMode_Tiled != baseInfo.mWebinarInfo.mixedLayout) {
                     baseInfo.mWebinarInfo.mixedLayout = eMixedLayoutMode_Tiled;
                  }
               }
            }
         }

			if (0 == ui.comboBox_Quality->count())
			{
				//根据下发的分辨率和上次选择的分辨率初始化
				//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
				QString msConfPath = globalToolManager->GetConfigPath();
				//QString configQuality = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, Fluent_definition);
            int configQualityNumber = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, "1").toInt();
            globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, configQualityNumber);
				std::list<UserSettings>::iterator iter = baseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.begin();

            int iRow = 0;
				while (iter != baseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.end() && iter->mbEnable) {
					//ui.comboBox_Quality->addItem(QString::fromStdString(iter->desc));
               ui.comboBox_Quality->insertItem(iRow, QString::fromStdString(iter->desc));
               ui.comboBox_Quality->setItemData(iRow, iter->label_number, Qt::UserRole);
               iRow++;
					iter++;
				}

				bool findConfigQulity = false;
				for (int i = 0; i < ui.comboBox_Quality->count(); i++) {
					//QString def = ui.comboBox_Quality->itemText(i);
               int iDef = ui.comboBox_Quality->itemData(i, Qt::UserRole).toInt();
					if (iDef == configQualityNumber) {
						findConfigQulity = true;
						ui.comboBox_Quality->setCurrentIndex(i);
						break;
					}
				}

				if (baseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.size() == 0) {
					ui.comboBox_Quality->insertItem(iRow, Fluent_definition);
               ui.comboBox_Quality->setItemData(iRow, 1, Qt::UserRole);
					//ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, Fluent_definition);
               ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, 1);
               globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:1\n", __FUNCTION__);
				}
				else if (findConfigQulity == false) {
					//QString configQuality = ui.comboBox_Quality->currentText();
               configQualityNumber  = ui.comboBox_Quality->currentData(Qt::UserRole).toInt();
					//ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, configQuality);
               globalToolManager->GetDataManager()->WriteLog("%s pre iCurLabelNumber:%d\n", __FUNCTION__, configQualityNumber);
               if (configQualityNumber <= 0) {
                  configQualityNumber = 1;
               }
               ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, configQualityNumber);
               globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, configQualityNumber);
				}
			}

			//上课时，清晰度选择框置灰不可点击
			if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
				ui.comboBox_Quality->setEnabled(false);
			}
			else {
				ui.comboBox_Quality->setEnabled(true);
			}
		}
	}
    connect(ui.comboBox_Quality, IndexChanged, this, &VhallIALiveSettingDlg::slot_comboBox_Quality_currentIndexChanged);

}

void VhallIALiveSettingDlg::Notice(const CSTDMapParamPtr &mapParam) {
}


void VhallIALiveSettingDlg::ReleaseResource() {
    globalToolManager->GetClassSDK()->StopPlayAudioFile();
    globalToolManager->GetClassSDK()->StopPreviewCamera();
    //DisableAllControl();
}

void VhallIALiveSettingDlg::slot_OnClickedApply() {
    ReleaseResource();
	Apply();
	accept();

    globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_WEBRTC_SETTING));
}

void VhallIALiveSettingDlg::SetOpenDesktopCapture(bool bDesktopCapture) {

	if (bDesktopCapture) {
		bool bready = true;
			//麦克风
			int currentMicIndex = GetCurrentMicIndex();
			if (micList.size() == 0) {
				emit sigNoMicAndPlayer(QStringLiteral("音频分享失败，未检测到麦克风，是否继续共享？"));
				bready = false;
			}

			//扬声器
			DevInfo playerInfo = GetCurrentPlayerIndex();
			if (mPlayerList.size() == 0 && bready) {
				emit sigNoMicAndPlayer(QStringLiteral("音频分享失败，未检测到扬声器，是否继续共享？"));
				bready = false;
			}

			if(bready){
		    	if (ui.horizontalSlider_deskTopCaptureVol->value() <= 0) {
			  	    ui.horizontalSlider_deskTopCaptureVol->setValue(30);
		    	}				
				int nRet = globalToolManager->GetClassSDK()->StartLocalCapturePlayer(currentMicIndex, playerInfo.Id().toStdWString(), ui.horizontalSlider_deskTopCaptureVol->value());
				emit sigNoMicAndPlayer(QStringLiteral("1"));
				//取消所有學員音頻
				if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher)
				{
					globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(true);
					ui.checkBox_muteRemote->setChecked(true);
				}
				else {
					globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(false);
					ui.checkBox_muteRemote->setChecked(false);
				}
			}
	   }
	else {
		setOpenDesktopCaptureChecked(false);
		globalToolManager->GetClassSDK()->StopLocalCapturePlayer();
		emit sigNoMicAndPlayer(QStringLiteral("1"));
	}
}

DevInfo VhallIALiveSettingDlg::GetCurrentPlayerIndex() {
	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
	QString configPlayId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, "");

	mPlayerList.clear();
	globalToolManager->GetDataManager()->WriteLog("%s GetPlayerDevices", __FUNCTION__);
	VHClassRoomInterface* pInterActionRoom = globalToolManager->GetClassSDK();
	DevInfo playerInfo;
	if (pInterActionRoom) {
		pInterActionRoom->GetPlayerDevices(mPlayerList);
		if (mPlayerList.size() == 0) {
		}
		else {
			int nCount = 0;
			bool bfind = false;
			int currentIndex = 0;
			int desktopCaptureIndex = 0;
			std::wstring desktopCaptureDevId;
			std::list<vhall::AudioDevProperty>::iterator player = mPlayerList.begin();
			while (player != mPlayerList.end()) {
				DevInfo info(QString::fromStdWString(player->mDevGuid), player->mIndex);
				if (QString::fromStdWString(player->mDevGuid).toStdString() == configPlayId.toStdString()) {
					playerInfo = info;
					bfind = true;
				}
				player++;
				nCount++;
			}
			if (!bfind) {
				DevInfo info(QString::fromStdWString(mPlayerList.front().mDevGuid), mPlayerList.front().mIndex);
				playerInfo = info ;
				ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, QString::fromStdWString(mPlayerList.front().mDevGuid));
			}
		}
	}
	return playerInfo;
}


int VhallIALiveSettingDlg::GetCurrentMicIndex() {
	//麦克风显示
	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
	QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");

	VHClassRoomInterface* pInterActionRoom = globalToolManager->GetClassSDK();
	int currentMicIndex = -1;
	if (pInterActionRoom) {
		std::string micDevId;
		micList.clear();
		globalToolManager->GetDataManager()->WriteLog("%s GetMicDevices", __FUNCTION__);
		pInterActionRoom->GetMicDevices(micList);
		if (micList.size() == 0) {
		}
		else {
			int nCount = 0;
			bool bfind = false;
			std::list<vhall::AudioDevProperty>::iterator itMic = micList.begin();
			while (itMic != micList.end()) {
				if (QString::fromStdWString(itMic->mDevGuid).toStdString() == configMicId.toStdString()) {
					currentMicIndex = nCount;
					bfind = true;
				}
				itMic++;
				nCount++;
			}
			if (!bfind) {
				currentMicIndex = 0;
				ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, QString::fromStdWString(micList.front().mDevGuid));
			}
		}
	}
	return currentMicIndex;
}

void VhallIALiveSettingDlg::RecoverState() {
	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
	int openDesktopCapture = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, 0);

	if (openDesktopCapture == 1) {
		//麦克风
		int currentMicIndex = GetCurrentMicIndex();
		if (micList.size() == 0) {
			return;
		}
		//扬声器
		DevInfo playerInfo = GetCurrentPlayerIndex();
		if (mPlayerList.size() == 0) {
			return;
		}
		if (ui.horizontalSlider_deskTopCaptureVol->value() <= 0) {
			ui.horizontalSlider_deskTopCaptureVol->setValue(30);
		}
		int nRet = globalToolManager->GetClassSDK()->StartLocalCapturePlayer(currentMicIndex, playerInfo.Id().toStdWString(), ui.horizontalSlider_deskTopCaptureVol->value());
	}
	else {
		setOpenDesktopCaptureChecked(false);
		globalToolManager->GetClassSDK()->StopLocalCapturePlayer();
	}
}

void VhallIALiveSettingDlg::slot_OnClickedClose() {
   //emit sigRealse();
    ReleaseResource();
    accept();
    globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_WEBRTC_SETTING));
}

void VhallIALiveSettingDlg::showEvent(QShowEvent *event) {
   this->setAttribute(Qt::WA_Mapped);
   QWidget::showEvent(event);

   QString strConfPath = globalToolManager->GetConfigPath();
   //int iLayout = ConfigSetting::ReadInt(strConfPath, GROUP_DEFAULT, PUSH_DESKTOP_SHARE_LAYOUT, ShareScreenLayout_camera_top);   
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   
   if (ShareScreenLayout_camera_top == baseInfo.mWebinarInfo.desktop_share_layout_mode){//pushButton_top_layout  被选中，打开画中画
      ui.pushButton_top_layout->SetDisEnable(false);
      ui.pushButton_normal_layout->SetDisEnable(true && baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS);
      mbShareScreenLayoutHasCamera = true;
   }
   else {//pushButton_normal_layout 被选中，关闭画中画
      ui.pushButton_top_layout->SetDisEnable(true && baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS);
      ui.pushButton_normal_layout->SetDisEnable(false);
      mbShareScreenLayoutHasCamera = false;
   }  
}



void VhallIALiveSettingDlg::SetAllSubScribeAudio(bool bmute) {

	globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(bmute);
}

void VhallIALiveSettingDlg::HandelCustomEvent(QEvent* event)
{
	customEvent(event);
}

void VhallIALiveSettingDlg::customEvent(QEvent* event) {
    if (event) {
        switch (event->type())
        {
        case CustomEvent_PreOnCaptureCallback: {
            QEventStream *customEvent = dynamic_cast<QEventStream*>(event);
            if (ui.radioButton_pic->isChecked()) {
               ui.comboBox_camera->setEnabled(false);
               ui.comboBox_micDev->setEnabled(true);
            }
            else if (ui.radioButton_camera->isChecked()) {
               ui.comboBox_camera->setEnabled(true);
               //ui.comboBox_micDev->setEnabled(false);
            }

            if (customEvent && customEvent->mStreamType == vlive::VHStreamType::VHStreamType_Preview_Video) {

                if (ui.comboBox_micDev->count() > 0) {
                    DevInfo info;
                    info = ui.comboBox_micDev->currentData().value<DevInfo>();
                    globalToolManager->GetClassSDK()->ChangePreViewMic(info.index());
                }
                if (customEvent->mIVHCapture == VHCapture_OK) {
                   // ui.label_11->setStyleSheet(CameraOpenSuccess);
                    mbCurrentCameraCanOpen = true;
					globalToolManager->GetDataManager()->WriteLog("%s mbCurrentCameraCanOpen %d", __FUNCTION__, 1);
					if (!ui.radioButton_pic->isChecked()) {
						ui.stackedWidget_preview->setCurrentIndex(0);
					}			
                }
                else {
                   // ui.label_11->setStyleSheet(CameraOpenFailed);
					     //ui.label_message->setVisible(true);
					     ui.widget_render->setStyleSheet("background-color:rgb(221, 221, 221);");
						 globalToolManager->GetDataManager()->WriteLog("%s mbCurrentCameraCanOpen %d", __FUNCTION__, 0);
                    mbCurrentCameraCanOpen = false;				
					if (!ui.radioButton_pic->isChecked()) {
						ui.stackedWidget_preview->setCurrentIndex(1);
					}
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

void VhallIALiveSettingDlg::closeEvent(QCloseEvent* e) {
    e->ignore();
    QWidget::closeEvent(e);
    slot_OnClickedClose();
}

bool VhallIALiveSettingDlg::eventFilter(QObject * obj, QEvent * ev)
{
   if (ev->type() == QEvent::Enter) {
      if (obj == ui.labLayout && m_pNoiseTips != NULL /*&& m_pNoiseTips->isHidden()*/) {
         m_pNoiseTips->setText(QString::fromWCharArray(L"混流布局可设置各角色非上麦状态观看混流视频画面的布局；小课堂均匀排列布局不支持1V12设置"));
         m_pNoiseTips->show();
         QPoint pos = /*this->mapToGlobal*/(this->pos());
         QPoint posFram = ui.frame->pos();
         QPoint posStack = ui.stackedWidget->pos();
         QPoint posLayout = ui.mixedLayoutWdg->pos();
         QPoint poslab = ui.labLayout->pos();
         m_pNoiseTips->move(pos + posFram + posStack + posLayout + poslab - QPoint(16, m_pNoiseTips->height()));
         //m_pNoiseTips->setFixedHeight(125);
      }
   }
   if (ev->type() == QEvent::Leave) {
      if ((obj == ui.labLayout)
         && m_pNoiseTips != NULL && !m_pNoiseTips->isHidden()) {
         m_pNoiseTips->close();
      }
   }
	return QDialog::eventFilter(obj, ev);
}

void VhallIALiveSettingDlg::slot_CurrentCameraSelectChanged(int) {
    //ui.comboBox_camera->setEnabled(false);
	ui.stackedWidget_preview->setCurrentIndex(3);
    StartPreView();
}

void VhallIALiveSettingDlg::slot_CurrentMicSelectChanged(int index) {
   DevInfo info;
   info = ui.comboBox_micDev->currentData().value<DevInfo>();
   if (ui.checkBox_openDesktopCapture->isChecked()) {
       DevInfo playerInfo;
       playerInfo = ui.comboBox_playerDev->currentData().value<DevInfo>();
       int nRet = globalToolManager->GetClassSDK()->StartLocalCapturePlayer(info.index(), playerInfo.Id().toStdWString(), ui.horizontalSlider_deskTopCaptureVol->value());
       if (nRet == 0) {
           bool mute = ui.checkBox_muteRemote->isChecked();
           if (mute) {
               globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(true);
           }
       }
   }
   else {
       globalToolManager->GetClassSDK()->SetUsedMic(info.index(), info.Id().toStdString(), L"");
       globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(false);
   }
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
   QString msConfPath = globalToolManager->GetConfigPath();
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, info.Id());
}

void VhallIALiveSettingDlg::slot_CurrentPlayerSelectChanged(int index) {
    DevInfo info;
    info = ui.comboBox_playerDev->currentData().value<DevInfo>();
    globalToolManager->GetClassSDK()->SetUsePlayer(info.index(), info.Id().toStdString());
}

void VhallIALiveSettingDlg::slot_CurrentDeskTopCaptureChanged(int index) {
    DevInfo playerInfo;
    playerInfo = ui.comboBox_deskTopCapture->currentData().value<DevInfo>();
    //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
    
	QString msConfPath = globalToolManager->GetConfigPath();
	ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_DEVID, playerInfo.Id());
}

void VhallIALiveSettingDlg::slot_MicValueChanged(int value) {
    globalToolManager->GetClassSDK()->SetCurrentMicVol(value);
}

void VhallIALiveSettingDlg::slot_OnClickedVideoTab() {
    ui.stackedWidget->setCurrentIndex(0);
    ui.pushButton_video_tab->setStyleSheet("QPushButton#pushButton_video_tab{border-image: url(:/setting/img/setting/video_enable.png);}");
    ui.pushButton_audio_tab->setStyleSheet("QPushButton#pushButton_audio_tab{border-image: url(:/setting/img/setting/audio_disable.png);}");
	ui.pushButton_about_tab->setStyleSheet("QPushButton#pushButton_about_tab{border-image: url(:/setting/img/setting/about_disable.png);}");
}

void VhallIALiveSettingDlg::slot_OnClickedAudioTab() {
    ui.stackedWidget->setCurrentIndex(1);
    ui.pushButton_video_tab->setStyleSheet("QPushButton#pushButton_video_tab{border-image: url(:/setting/img/setting/video_disable.png);}");
    ui.pushButton_audio_tab->setStyleSheet("QPushButton#pushButton_audio_tab{border-image: url(:/setting/img/setting/audio_enable.png);}");
	ui.pushButton_about_tab->setStyleSheet("QPushButton#pushButton_about_tab{border-image: url(:/setting/img/setting/about_disable.png);}");
}

void VhallIALiveSettingDlg::slot_OnClickedAAboutTab() {
	ui.stackedWidget->setCurrentIndex(2);
	ui.pushButton_video_tab->setStyleSheet("QPushButton#pushButton_video_tab{border-image: url(:/setting/img/setting/video_disable.png);}");
	ui.pushButton_audio_tab->setStyleSheet("QPushButton#pushButton_audio_tab{border-image: url(:/setting/img/setting/audio_disable.png);}");
	ui.pushButton_about_tab->setStyleSheet("QPushButton#pushButton_about_tab{border-image: url(:/setting/img/setting/about_enable.png);}");
}

void VhallIALiveSettingDlg::slot_OnClickedSelectPic() {

	if (!ui.radioButton_pic->isChecked()) return;
    QString selectFileName;// = QFileDialog::getOpenFileName(NULL, QStringLiteral("打开文件"), "", QStringLiteral("文件(*.jpg *.JPG *.png *.PNG);"), 0, 0);
    TCHAR szPathName[2048];
    OPENFILENAME ofn = { sizeof(OPENFILENAME) };
    ofn.hwndOwner = NULL;// 打开OR保存文件对话框的父窗口
    ofn.lpstrFilter = TEXT("*.jpg\0*.JPG\0*.png\0*.PNG\0");
    lstrcpy(szPathName, TEXT(""));
    ofn.lpstrFile = szPathName;
    ofn.nMaxFile = sizeof(szPathName);//存放用户选择文件的 路径及文件名 缓冲区
    ofn.lpstrTitle = TEXT("选择文件");//选择文件对话框标题
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;//如果需要选择多个文件 则必须带有  OFN_ALLOWMULTISELECT标志
    BOOL bOk = GetOpenFileName(&ofn);//调用对话框打开文件
    if (bOk) {
        selectFileName = QString::fromStdWString(szPathName);
    }
    if (selectFileName.isEmpty()) {
        return;
    }
	
    if (0==globalToolManager->GetClassSDK()->CheckPicEffectiveness(selectFileName.toStdString())) {
        LoadPrivewPic(selectFileName);
		ui.lineEdit_picPath->setText(selectFileName);
        ui.stackedWidget_preview->setCurrentIndex(2);
    }
}

void VhallIALiveSettingDlg::LoadPrivewPic(const QString picPath) {
 //   QPixmap px;
 //   bool ok = px.load(picPath);
 //   if (!ok) {
	//	ok = px.load(picPath, "JPG");
	//	if (!ok)
	//	{
	//		ok = px.load(picPath, "PNG");
	//		if (!ok)
	//			return;
	//	}

 //   }
	//
	//int newWidth = ui.page_pic->width();
	//int newHeight = ui.page_pic->height();

	//int picWidth = px.width();
	//int picHeight = px.height();

	//float picScal = ((float)picWidth / (float)picHeight);
	//float wndScal = ((float)newWidth / (float)newHeight);

	//if (picHeight > newHeight && picWidth > newWidth) {//图片的长宽都大
	//	if (picScal > wndScal) {
	//		newHeight = newWidth * ((float)picHeight / (float)picWidth);
	//	}
	//	else {
	//		newWidth = newHeight * ((float)picWidth / (float)picHeight);
	//	}
	//}
	//else if (picHeight < ui.label_previewPic->height() && picWidth > ui.label_previewPic->width()) {//图片的宽度大，高度小
	//	newHeight = newWidth * ((float)picHeight / (float)picWidth);
	//}
	//else if (picHeight > ui.label_previewPic->height() && picWidth < ui.label_previewPic->width()) {//图片的高度大，宽度小
	//	newWidth = newHeight * ((float)picWidth / (float)picHeight);
	//}
	//else {
	//	newWidth = picWidth;
	//	newHeight = picHeight;
	//}

	//px = px.scaled(newWidth, newHeight);
	//ui.label_previewPic->setFixedSize(QSize(newWidth, newHeight));
 //   ui.label_previewPic->setPixmap(px);
 //   ui.label_previewPic->setScaledContents(true);
	globalToolManager->GetDataManager()->WriteLog("%s LoadPrivewPic file:%s", __FUNCTION__, picPath);
   ui.label_previewPic->clear();
   ui.label_previewPic->setFixedSize(QSize(320, 160));
   ui.label_previewPic->setText(QString::fromWCharArray(L"找不到图片，请重新选择"));
    QImage privew_image;
    bool bRet = privew_image.load(picPath);

    if (bRet) {
	 globalToolManager->GetDataManager()->WriteLog("%s bRet:%s", __FUNCTION__, "1");
       QPixmap image = QPixmap::fromImage(privew_image);
       int width = image.width();
       int height = image.height();
       QSize curSize = ui.label_previewPic->size();
       while (true) {
          if (width > curSize.width() || height > curSize.height()) {
             width = width * 9 / 10;
             height = height * 9 / 10;
          }
          else {
             break;
          }
       }

       image = image.scaled(QSize(width, height), Qt::KeepAspectRatio);
       ui.label_previewPic->setFixedSize(QSize(width, height));
       ui.label_previewPic->setPixmap(image);
       ui.label_previewPic->show();
       ui.label_previewPic->setScaledContents(true);
    }
}

void VhallIALiveSettingDlg::DisableAllControl() {
    //ui.pushButton_ok->setEnabled(false);
    //ui.comboBox_camera->setEnabled(false);
    //ui.comboBox_micDev->setEnabled(false);
    //ui.comboBox_playerDev->setEnabled(false);
}

void VhallIALiveSettingDlg::slot_comboBox_Quality_currentIndexChanged(int index)
{
	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
	//QString configQuality = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, Standard_definition);
	//ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, text);
   int iQuality = ui.comboBox_Quality->currentData(Qt::UserRole).toInt();
   globalToolManager->GetDataManager()->WriteLog("%s iQuality :%d", __FUNCTION__,iQuality);
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, iQuality);
}

void VhallIALiveSettingDlg::slot_TestPlay() {
    if (ui.comboBox_playerDev->count() > 0) {
        DevInfo playOutinfo = ui.comboBox_playerDev->currentData().value<DevInfo>();
        QString appPath = QCoreApplication::applicationDirPath();
        QString fileName = appPath + QString("/") + AUDIO_FILE;
        globalToolManager->GetClassSDK()->PlayAudioFile(fileName.toStdString(), playOutinfo.index());
    }
}


void VhallIALiveSettingDlg::slot_OpenPushPic(bool checked) {
    if (checked) {
		globalToolManager->GetDataManager()->WriteLog("%s slot_OpenPushPic(true)", __FUNCTION__);
		ui.comboBox_camera->setEnabled(false);
        ui.pushButton_selectPic->setEnabled(true);
		//ui.lineEdit_picPath->setEnabled(true);
        QString selectFileName = ui.lineEdit_picPath->text();
		globalToolManager->GetDataManager()->WriteLog("%s selectFileName:%s", __FUNCTION__ , selectFileName.toStdString().c_str());
        if (globalToolManager->GetClassSDK()->CheckPicEffectiveness(selectFileName.toStdString()) == 0) {
            LoadPrivewPic(selectFileName);
            ui.stackedWidget_preview->setCurrentIndex(2);
        }
		else {
			globalToolManager->GetDataManager()->WriteLog("%s CheckPicEffectiveness error", __FUNCTION__);
		}
    }
    else {
        ui.pushButton_selectPic->setEnabled(false);
		ui.lineEdit_picPath->setEnabled(false);
		ui.comboBox_camera->setEnabled(true);
        //ui.stackedWidget_preview->setCurrentIndex(0);
    }
}

void VhallIALiveSettingDlg::slot_RadioCamera(bool checked) {
    if (checked) {
        ui.stackedWidget_preview->setCurrentIndex(0);
		ui.lineEdit_picPath->setEnabled(false);
		ui.pushButton_selectPic->setEnabled(false);
		ui.comboBox_camera->setEnabled(true);
    }
	else
	{
		ui.comboBox_camera->setEnabled(false);
		ui.pushButton_selectPic->setEnabled(true);
		//ui.lineEdit_picPath->setEnabled(true);
	}
}

void VhallIALiveSettingDlg::slot_OpenDesktopAudio(bool checked) {
    ui.comboBox_deskTopCapture->setEnabled(checked);
    ui.horizontalSlider_deskTopCaptureVol->setEnabled(checked);
	if (checked)
	{
		ui.checkBox_muteRemote->setChecked(true);
		ui.checkBox_muteRemote->setEnabled(true);
	}
	else
	{
		ui.checkBox_muteRemote->setChecked(false);
		ui.checkBox_muteRemote->setEnabled(false);
	}
}

bool VhallIALiveSettingDlg::IsOpenDesktopCapture() {
	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
	int openDesktopCapture = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, 0);
	if (openDesktopCapture == 1) {
		return true;
	}
	else {
		return false;
	}
}

void VhallIALiveSettingDlg::slot_DesktopCaptureValueChanged(int value) {
    globalToolManager->GetClassSDK()->SetLocalCapturePlayerVolume(value);
    //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
    ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_VOLUME, value);
}

void VhallIALiveSettingDlg::slot__Adaptive(bool badaptive) {


}


void VhallIALiveSettingDlg::slot_OnClickedScreenShareCameraBottomLayout() {

}

void VhallIALiveSettingDlg::slot_OnClickedScreenShareCameraTopLayout() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, QString::fromWCharArray(L"上课中无法修改共享布局")));
      return;
   }

   //ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_TOP_SELECT);
   //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {//公开课
      ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_TOP_SELECT);
   }
   else
   {
      ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_BOTTOM_SELECT);
   }

   //   ui.pushButton_bottom_layout->setStyleSheet(SCRREN_LAYOUT_BOTTOM_NO_SELECT);
   ui.pushButton_normal_layout->setStyleSheet(SCRREN_LAYOUT_NORMAL_NO_SELECT);
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
   QString msConfPath = globalToolManager->GetConfigPath();
   mbShareScreenLayoutHasCamera = true;
   //ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_SHARE_LAYOUT, ShareScreenLayout_camera_top);

}

void VhallIALiveSettingDlg::slot_OnClickedScreenShareOnlyLayout() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, QString::fromWCharArray(L"上课中无法修改共享布局")));
      return;
   }
   
   //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {//公开课
      ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_TOP_NO_SELECT);
   }
   else
   {
      ui.pushButton_top_layout->setStyleSheet(SCRREN_LAYOUT_BOTTOM_NO_SELECT);
   }

   ui.pushButton_normal_layout->setStyleSheet(SCRREN_LAYOUT_NORMAL_SELECT);
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
   QString msConfPath = globalToolManager->GetConfigPath();
   mbShareScreenLayoutHasCamera = false;
   //ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_SHARE_LAYOUT, ShareScreenLayout_on_camera);
}

void VhallIALiveSettingDlg::on_btnMixedLayoutTiled_clicked(bool checked)
{
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, QString::fromWCharArray(L"上课中无法修改混流布局")));
      return;
   }
    miMixedLayoutMode  = eMixedLayoutMode_Tiled;
   //globalToolManager->GetClassSDK()->SetScreenLayout(false, eMixedLayoutMode_Tiled);
   ui.btnMixedLayoutTiled->setStyleSheet(MIXED_LAYOUT_TILED_SELECT);
   ui.btnMixedLayoutGrid->setStyleSheet(MIXED_LAYOUT_GRID_NO_SELECT);
}

void VhallIALiveSettingDlg::on_btnMixedLayoutGrid_clicked(bool checked)
{
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, QString::fromWCharArray(L"上课中无法修改混流布局")));
      return;
   }
   miMixedLayoutMode = eMixedLayoutMode_Grid;
   //globalToolManager->GetClassSDK()->SetScreenLayout(false, eMixedLayoutMode_Grid);
   ui.btnMixedLayoutTiled->setStyleSheet(MIXED_LAYOUT_TILED_NO_SELECT);
   ui.btnMixedLayoutGrid->setStyleSheet(MIXED_LAYOUT_GRID_SELECT);
}

void VhallIALiveSettingDlg::Apply() {
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
   QString msConfPath = globalToolManager->GetConfigPath();
   QString configCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, "");
   QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");
   QString configPlayId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, "");
   //讲师且没上课
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher
      && baseInfo.mWebinarInfo.type != CLASS_STATUS_IN_CLASS) {
      globalToolManager->GetClassSDK()->SetScreenLayout(mbShareScreenLayoutHasCamera, miMixedLayoutMode /*eMixedLayoutMode_Grid*/);
   }
   
   if (ui.checkBox_Adaptive->isChecked()) {
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, SCREEN_ADAPTIVE, 1);
   }
   else {
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, SCREEN_ADAPTIVE, 0);
   }

   std::string userCameraId, userMicId, userPlayerId;
   bool bCameraChanged = false;
   vhall::VideoDevProperty dev;
   if (mCameraList.size() > 0) {
      dev = ui.comboBox_camera->currentData().value<vhall::VideoDevProperty>();
      //当存在着摄像头设备，并且和当前设置的摄像头设备不一样，那么就要设置摄像头。
      if ((!configCameraId.isEmpty() && dev.mDevId != configCameraId.toStdString()) || configCameraId.isEmpty()) {
         bCameraChanged = true;
         //globalToolManager->GetClassSDK()->SetUsedCamera(dev.mIndex, dev.mDevId);
      }
      userCameraId = dev.mDevId;
   }

   std::wstring desktopCaptureId;
   DevInfo micInfo;
   if (micList.size() > 0) {
      micInfo = ui.comboBox_micDev->currentData().value<DevInfo>();
      //当存在着麦克风设备，并且和当前设置的麦克风设备不一样，那么就要设置麦克风。
      if (!micInfo.Id().isEmpty() || configMicId.isEmpty()) {
         if (ui.checkBox_openDesktopCapture->isChecked() && ui.comboBox_deskTopCapture->count() > 0) {
            DevInfo playerInfo;
            playerInfo = ui.comboBox_playerDev->currentData().value<DevInfo>();
            desktopCaptureId = playerInfo.Id().toStdWString();
            int nRet = globalToolManager->GetClassSDK()->StartLocalCapturePlayer(micInfo.index(), playerInfo.Id().toStdWString(), ui.horizontalSlider_deskTopCaptureVol->value());
            if (nRet == 0) {
               bool mute = ui.checkBox_muteRemote->isChecked();
               globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(mute);
               //emit sig_MuteAllSubScribeAudio(mute);
            }
         }
         else {
            globalToolManager->GetClassSDK()->SetUsedMic(micInfo.index(), micInfo.Id().toStdString(), L"");
            globalToolManager->GetClassSDK()->MuteAllSubScribeAudio(false);
            SetOpenDesktopCapture(false);
            //emit sig_MuteAllSubScribeAudio(false);
         }
         int nValue = ui.horizontalSlider_mic->value();
         globalToolManager->GetClassSDK()->SetCurrentMicVol(nValue);
      }
      userMicId = micInfo.Id().toStdString();
   }

   DevInfo playOutinfo;
   if (mPlayerList.size() > 0) {
      playOutinfo = ui.comboBox_playerDev->currentData().value<DevInfo>();
      if ((!playOutinfo.Id().isEmpty() && playOutinfo.Id().toStdString() != configPlayId.toStdString()) || configPlayId.isEmpty()) {
         globalToolManager->GetClassSDK()->SetUsePlayer(playOutinfo.index(), playOutinfo.Id().toStdString());
         //int nValue = ui.horizontalSlider_player->value();
         //globalToolManager->GetClassSDK()->SetCurrentPlayVol(nValue);
      }
      userPlayerId = playOutinfo.Id().toStdString();
   }
   //QString currentQulity = ui.comboBox_Quality->currentText();
   int configQualityNumber = ui.comboBox_Quality->currentData(Qt::UserRole).toInt();
   bool bPushPic = ui.radioButton_pic->isChecked();
   QString picPath = ui.lineEdit_picPath->text();

   int iVioeProFile = 360;
   globalToolManager->GetVideoProfileByConfig(configQualityNumber, iVioeProFile, ToolManager::eVideoProFileTypeVideo);
   VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(iVioeProFile);


   int lastOpenPicPush = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 1);

   if (bPushPic && globalToolManager->GetDataManager()->IsAlreadyPublished()) {
      QString configPic = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PUSH_PIC_PATH, "");
      if (globalToolManager->GetClassSDK()->CheckPicEffectiveness(picPath.toStdString()) == 0) {
         if (configPic != picPath || lastOpenPicPush == 0) {
            globalToolManager->GetClassSDK()->StartLocalCapturePicture(picPath.toStdString(), videoIndex);
         }
      }
      else {
         QString defaultPic = QApplication::applicationDirPath() + "/" + QString::fromStdString("default_pushPic.png");
         globalToolManager->GetClassSDK()->StartLocalCapturePicture(defaultPic.toStdString(), videoIndex);
      }
   }
   else {
      //如果摄像头改变了，并且处于上麦状态。则切换摄像头。
      if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {
         if (mbCurrentCameraCanOpen) {
            globalToolManager->GetClassSDK()->StartLocalCapture(userCameraId, videoIndex);
         }
         //else {
           // mpWebRtcLiveWdg->FadeOutTip(, 3);
         //}
      }
   }

   if (bPushPic) {
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 1);
      emit sigPicPush(true);
   }
   else {
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 0);
      emit sigPicPush(false);
   }

   //更新上报上麦、双推能力
   if (globalToolManager->GetDataManager()->IsAlreadyPublished())
   {
      bool bEnableDoublePub = isEnableDoublePublish();
      globalToolManager->GetClassSDK()->UpdatePublishAbility(globalToolManager->GetClassSDK()->HasVideoOrAudioDev(), bEnableDoublePub);
   }

   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_PIC_PATH, picPath);
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, QString::fromStdString(userCameraId));
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, QString::fromStdString(userMicId));
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, QString::fromStdString(userPlayerId));
   //ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, currentQulity);
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, configQualityNumber);
   globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, configQualityNumber);
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_VOLUME, ui.horizontalSlider_deskTopCaptureVol->value());
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_DEVID, QString::fromStdWString(desktopCaptureId));
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, ui.checkBox_openDesktopCapture->isChecked() ? 1 : 0);
   ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, ui.checkBox_muteRemote->isChecked() ? 1 : 0);
}
