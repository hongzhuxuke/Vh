#include "DeviceTestingWdg.h"
#include "../Unility/CustomEventType.h"
#include "../Unility/Unility.h"
#include "ConfigSetting.h"
#include "pathmanager.h"
#include <ctime>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>

#define AUDIO_FILE  "yisell_sound_48k.pcm"
extern ToolManager *globalToolManager;


#define LABEL_SUCESS    QStringLiteral("QLabel{border-image: url(:/ctrl/img/ctrl/big/Device_Sucess.png);background:rgba(82, 204, 144, 1);border-radius:16; }")

#define LABEL_FAILED    QStringLiteral("QLabel{border-image: url(:/ctrl/img/ctrl/big/Device_Error.png);background:rgba(252, 86, 89, 1);border-radius:16; }")

#define LABEL_ICO_SUCESS   QStringLiteral("QLabel{border-image:url(:/ctrl/img/ctrl/Dev_Scuess.png);}")

#define LABEL_ICO_FAILED    QStringLiteral("QLabel{border-image:url(:/ctrl/img/ctrl/Dev_Failed.png); }")

#define LABEL_ICO__SUCESS   QStringLiteral("QLabel{color:rgba(82,204,144,1);font-size:14px;font-family : \"微软雅黑\";}")

#define LABEL_ICO__FAILED    QStringLiteral("QLabel{color:rgba(252,86,89,1); font-size:14px;font-family : \"微软雅黑\";}")

#define LABEL_RESULT_FAILED   QStringLiteral("QLabel{border-image:url(:/ctrl/img/ctrl/Dev_Error.png); }")

#define LABEL_RESULT_SUCESS   QStringLiteral("QLabel{border-image:url(:/ctrl/img/ctrl/Dev_Sucess.png); }")

#define LABEL_TITLE_SUCESS   QStringLiteral("QLabel{color:rgba(82,204,144,1); font-size:14px;font-family : \"微软雅黑\"; }")

#define LABEL_TITLE_FAILED   QStringLiteral("QLabel{color:rgba(252,86,89,1); font-size:14px;font-family : \"微软雅黑\";}")

CREATE_WND_FUNCTION(DeviceTestingWdg);
DeviceTestingWdg::DeviceTestingWdg(QWidget *parent)
	: CBaseWnd(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool/* | Qt::WindowStaysOnTopHint*/);
	ui.setupUi(this);
	raise();
	setModal(true);
	ui.stackedWidget->setCurrentIndex(DEVICE_TESTING);
	setFixedSize(450, 278);
	QColor color(82, 204, 144);
	ui.widget_micVolume->SetColor(color);
	connect(ui.widget_micVolume, SIGNAL(sig_micHasAudio()), this, SLOT(slot_MicHasAudio()));
	ui.widget_micVolume->SetVolumeType(VolumeType_PreviewMic);

	ui.stackedWidget_camera->setCurrentIndex(CAMERA_RENDER);

	connect(ui.comboBox_camera, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cmbcamera_currentIndexChanged(int)));
   connect(ui.pushButton_gotest, SIGNAL(clicked()), this, SLOT(slot_Testing()));
	connect(ui.pushButton_TestScuess_camera, SIGNAL(clicked()), this, SLOT(slot_TestScuess()));
	connect(ui.pushButton_TestFaile_camera, SIGNAL(clicked()), this, SLOT(slot_TestFailed()));
	connect(ui.pushButton_TestScuess_mic, SIGNAL(clicked()), this, SLOT(slot_TestScuess()));
	connect(ui.pushButton_TestFaile_mic, SIGNAL(clicked()), this, SLOT(slot_TestFailed()));
	connect(ui.pushButton_TestScuess_speak, SIGNAL(clicked()), this, SLOT(slot_TestScuess()));
	connect(ui.pushButton_TestFaile_speak, SIGNAL(clicked()), this, SLOT(slot_TestFailed()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_TestPlay()));
	connect(ui.pushButton_ReDevTesting, SIGNAL(clicked()), this, SLOT(slot_ReDevTesting())); 
    connect(ui.pushButton_GoClass, SIGNAL(clicked()), this, SLOT(slot_GoClass()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_GoClass())); 
	connect(ui.pushButton_NotDTesting, SIGNAL(clicked()), this, SLOT(slot_GoClass()));
	connect(ui.pushButton_help, SIGNAL(clicked()), this, SLOT(slot_Help()));

	connect(ui.comboBox_micDev, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentMicSelectChanged(int))); 
	connect(ui.comboBox_playerDev, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentPlayerSelectChanged(int)));
	connect(ui.horizontalSlider_deskTopCaptureVol, SIGNAL(valueChanged(int)), this, SLOT(slot_DesktopCaptureValueChanged(int)));

	ui.pushButton_TestScuess_camera->setEnabled(false);
   globalToolManager->GetDataManager()->WriteLog("%s  pushButton_TestScuess_camera setEnabled(false)", __FUNCTION__);
	ui.pushButton_TestScuess_mic->setEnabled(false); 
	ui.pushButton_TestScuess_speak->setEnabled(false);
   ui.pushButton_help->SetCursorType(CDisEnablePushButton::eCursorType_PointingHandCursor);
   ui.pushButton->SetCursorType(CDisEnablePushButton::eCursorType_PointingHandCursor);
   this->setWindowTitle(QStringLiteral("设备检测"));

}

DeviceTestingWdg::~DeviceTestingWdg()
{
}

void DeviceTestingWdg::Notice(const CSTDMapParam &) {

}

void DeviceTestingWdg::Notice(const CSTDMapParamPtr &) {

}

void DeviceTestingWdg::slot_ReDevTesting() {
	ui.stackedWidget->setCurrentIndex(DEVICE_CAMERA);
	StartPreView();
	ui.label_result->setStyleSheet(LABEL_RESULT_SUCESS);
	ui.label_title->setStyleSheet(LABEL_TITLE_SUCESS);
   ui.label_title->setText( QStringLiteral("设备检测正常") );
}

void DeviceTestingWdg::slot_GoClass() {

	globalToolManager->GetClassSDK()->StopPreviewCamera();
	//if (!mbDevTested) {
	//	globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mDevTestResult = 1;
	//}
	//globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_DECICE_TESTING));
	//globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_DECICE_TESTING));


   accept();
}

void DeviceTestingWdg::slot_Help() {
	QDesktopServices::openUrl(QUrl("https://www.vhall.com/saas/doc/1722.html"));
}

void DeviceTestingWdg::slot_Testing() {

	hide();
	QDesktopWidget desktop;
	QWidget * deskWdg = desktop.screen(0);
	QRect rect = deskWdg->rect();
	move((rect.width() - 450) / 2, (rect.height() - 549) / 2);
	setFixedSize(450, 549);
	ui.stackedWidget->setCurrentIndex(ui.stackedWidget->currentIndex() + 1);

	show();

	std::string cameraId = "";
	std::list<vhall::VideoDevProperty> cameraList;
	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
	QString configCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, "");
	QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");
	QString configPlayId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, "");
	//int openPic = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 0);
	//ui.pushButton_apply->setEnabled(false);

	std::list<vhall::VideoDevProperty>::iterator iter = cameraList.begin();
	//ui.comboBox_camera->clear();
	//while (iter != cameraList.end()) {
	//	ui.comboBox_camera->addItem(QString::fromStdString(iter->mDevName), QVariant::fromValue(*iter));

	//	iter++;
	//}

	VHClassRoomInterface* pInterActionRoom = globalToolManager->GetClassSDK();
	if (pInterActionRoom) {
		disconnect(ui.comboBox_camera, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cmbcamera_currentIndexChanged(int)));

		globalToolManager->GetDataManager()->WriteLog("%s GetCameraDevices", __FUNCTION__);
		pInterActionRoom->GetCameraDevices(cameraList);
		//摄像设备显示
		ui.comboBox_camera->clear();
		if (cameraList.size() == 0) {
			ui.comboBox_camera->addItem(QStringLiteral("无摄像头设备"));
		}
		else {
			int nCount = 0;
			int currentIndex = 0;
			std::list<vhall::VideoDevProperty>::iterator it = cameraList.begin();
			for (; it != cameraList.end(); it++) {
				ui.comboBox_camera->addItem(QString::fromStdString(it->mDevName), QVariant::fromValue(*it));
				if ((*it).mDevId == configCameraId.toStdString()) {
					currentIndex = nCount;
				}
				nCount++;
			}
			ui.comboBox_camera->setCurrentIndex(currentIndex);
			ui.comboBox_camera->setEnabled(true);
		}

		connect(ui.comboBox_camera, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cmbcamera_currentIndexChanged(int)));
		//麦克风显示
		int currentMicIndex = 0;
		std::string micDevId;
		micList.clear();
		globalToolManager->GetDataManager()->WriteLog("%s GetMicDevices", __FUNCTION__);
		pInterActionRoom->GetMicDevices(micList);
		ui.comboBox_micDev->clear();
		if (micList.size() == 0) {
			ui.comboBox_micDev->addItem(QStringLiteral("无音频输入设备"));
		}
		else {
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
		}

		//扬声器
		std::list<vhall::AudioDevProperty> mPlayerList;
		globalToolManager->GetDataManager()->WriteLog("%s GetPlayerDevices", __FUNCTION__);
		pInterActionRoom->GetPlayerDevices(mPlayerList);
		ui.comboBox_playerDev->clear();
		if (mPlayerList.size() == 0) {
			ui.comboBox_playerDev->addItem(QStringLiteral("无音频输出设备"));
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
				if (QString::fromStdWString(player->mDevGuid).toStdString() == configPlayId.toStdString()) {
					currentIndex = nCount;
				}
				player++;
				nCount++;
			}
			
			ui.comboBox_playerDev->setCurrentIndex(currentIndex);
		}

		StartPreView();

		DevInfo micInfo;
		if (micList.size() > 0) {
			micInfo = ui.comboBox_micDev->currentData().value<DevInfo>();
			//当存在着麦克风设备，并且和当前设置的麦克风设备不一样，那么就要设置麦克风。

				DevInfo playerInfo;
				playerInfo = ui.comboBox_playerDev->currentData().value<DevInfo>();

				int nRet = globalToolManager->GetClassSDK()->StartLocalCapturePlayer(micInfo.index(), playerInfo.Id().toStdWString(), 80);

				globalToolManager->GetClassSDK()->SetCurrentMicVol(80);
			
		}

		DevInfo playOutinfo;
		if (mPlayerList.size() > 0) {
			playOutinfo = ui.comboBox_playerDev->currentData().value<DevInfo>();
			globalToolManager->GetClassSDK()->SetUsePlayer(playOutinfo.index(), playOutinfo.Id().toStdString());
		}

	}
}

void DeviceTestingWdg::slot_DesktopCaptureValueChanged(int value) {
	globalToolManager->GetClassSDK()->SetCurrentPlayVol(value);
	//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	//QString msConfPath = globalToolManager->GetConfigPath();
	//ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_VOLUME, value);
}

void DeviceTestingWdg::slot_CurrentMicSelectChanged(int index) {
	ui.pushButton_TestScuess_mic->setEnabled(false);
	DevInfo info;
	info = ui.comboBox_micDev->currentData().value<DevInfo>();
	DevInfo playerInfo;
	playerInfo = ui.comboBox_playerDev->currentData().value<DevInfo>();
	int nRet = globalToolManager->GetClassSDK()->StartLocalCapturePlayer(info.index(), playerInfo.Id().toStdWString(), ui.horizontalSlider_deskTopCaptureVol->value());
	
}

void DeviceTestingWdg::slot_CurrentPlayerSelectChanged(int index) {
	DevInfo info;
	info = ui.comboBox_playerDev->currentData().value<DevInfo>();
	globalToolManager->GetClassSDK()->SetUsePlayer(info.index(), info.Id().toStdString());
}

void DeviceTestingWdg::HandelCustomEvent(QEvent * event)
{
   QEventStream *customEvent = dynamic_cast<QEventStream*>(event);

   ui.comboBox_camera->setEnabled(true);
   if (customEvent && customEvent->mStreamType == vlive::VHStreamType::VHStreamType_Preview_Video) {
      //if (ui.comboBox_micDev->count() > 0) {
      //	DevInfo info;
      //	info = ui.comboBox_micDev->currentData().value<DevInfo>();
      //	globalToolManager->GetClassSDK()->ChangePreViewMic(info.index());
      //}
      if (customEvent->mIVHCapture == VHCapture_OK) {
         //ui.label_11->setStyleSheet(CameraOpenSuccess);
         //mbCurrentCameraCanOpen = true;
         ui.stackedWidget_camera->setCurrentIndex(CAMERA_RENDER);
         ui.pushButton_TestScuess_camera->setEnabled(true);
         globalToolManager->GetDataManager()->WriteLog("%s mbCurrentCameraCanOpen %d pushButton_TestScuess_camera setEnabled(true)", __FUNCTION__, 1);
      }
      else if (customEvent->mIVHCapture == VHCapture_STREAM_SOURCE_LOST) {
         //ui.label_11->setStyleSheet(CameraOpenFailed);
         //ui.label_message->setVisible(true);
         //ui.widget_render->setStyleSheet("background-color:rgb(221, 221, 221);");
         ui.stackedWidget_camera->setCurrentIndex(CAMERA_FAILED);
         globalToolManager->GetDataManager()->WriteLog("%s mbCurrentCameraCanOpen %d", __FUNCTION__, 0);
         //mbCurrentCameraCanOpen = false;
      }
      else {
         ui.stackedWidget_camera->setCurrentIndex(CAMERA_FAILED);
         globalToolManager->GetDataManager()->WriteLog("%s mbCurrentCameraCanOpen %d", __FUNCTION__, 0);
      }
   }
}

void DeviceTestingWdg::customEvent(QEvent* event) {
	if (event) {
		switch (event->type())
		{
		case CustomEvent_PreOnCaptureCallback: {
         HandelCustomEvent(event);

			break;
		}
		default:
			break;
		}
	}
}

void DeviceTestingWdg::on_cmbcamera_currentIndexChanged(int index) {
	ui.stackedWidget_camera->setCurrentIndex(CAMERA_CHANGE);
	ui.pushButton_TestScuess_camera->setEnabled(false);
   globalToolManager->GetDataManager()->WriteLog("%s  pushButton_TestScuess_camera setEnabled(false)", __FUNCTION__);
	StartPreView();
}

void DeviceTestingWdg::slot_TestPlay() {
	ui.pushButton->setEnabled(false);
   ui.pushButton->SetCursorType(CDisEnablePushButton::eCursorType_ForbiddenCursor);
	if (!mPlayTimer) {
		mPlayTimer = new QTimer(this);
		connect(mPlayTimer, &QTimer::timeout, this, &DeviceTestingWdg::slot_PlayOver);
	}
   ui.pushButton_TestScuess_speak->setEnabled(true);
	
	if (ui.comboBox_playerDev->count() > 0) {
		DevInfo playOutinfo = ui.comboBox_playerDev->currentData().value<DevInfo>();
		QString appPath = QCoreApplication::applicationDirPath();
		QString fileName = appPath + QString("/") + AUDIO_FILE;
		globalToolManager->GetClassSDK()->PlayAudioFile(fileName.toStdString(), playOutinfo.index());
		mPlayTimer->start(8000);
	}

	//globalToolManager->GetClassSDK()->StopPlayAudioFile();
}

void DeviceTestingWdg::slot_PlayOver() {
	ui.pushButton->setEnabled(true);
   ui.pushButton->SetCursorType(CDisEnablePushButton::eCursorType_PointingHandCursor);
   if (mPlayTimer && mPlayTimer->isActive()) {
      mPlayTimer->stop();
   }
   
}

void DeviceTestingWdg::slot_MicHasAudio() {
	ui.pushButton_TestScuess_mic->setEnabled(true);
}

void DeviceTestingWdg::StartPreView(){

		vhall::VideoDevProperty dev;
	dev = ui.comboBox_camera->currentData().value<vhall::VideoDevProperty>();
	std::string devId = dev.mDevId;
	HWND id = (HWND)(ui.widget_render->winId());
	if (micList.size() > 0) {
		DevInfo info;
		info = ui.comboBox_micDev->currentData().value<DevInfo>();
		if (globalToolManager->GetClassSDK()->PreviewCamera((void*)id, devId, RTC_VIDEO_PROFILE_480P_4x3_M, info.index()) == 0) {
			//ui.comboBox_camera->setEnabled(true);
			//ui.comboBox_micDev->setEnabled(false);
		}
	}
	else {
		if (globalToolManager->GetClassSDK()->PreviewCamera((void*)id, devId, RTC_VIDEO_PROFILE_480P_4x3_M) == 0) {
			//ui.comboBox_camera->setEnabled(true);
			//ui.comboBox_micDev->setEnabled(false);
		}
	}
}



void DeviceTestingWdg::slot_TestScuess() {
   globalToolManager->GetClassSDK()->StopPlayAudioFile();
   slot_PlayOver();
	QString msConfPath = globalToolManager->GetConfigPath();
	std::string userCameraId, userMicId, userPlayerId;

	if (ui.stackedWidget->currentIndex() == DEVICE_CAMERA) {
		mbTestCamera = true;	


		//更新样式
		ui.label_camera->setStyleSheet(LABEL_SUCESS); 
		ui.label_camera_2->setStyleSheet(LABEL_SUCESS);
		ui.label_ico_camera->setStyleSheet(LABEL_ICO_SUCESS);
		ui.label_ico_c->setStyleSheet(LABEL_ICO__SUCESS); 
		userCameraId = ui.comboBox_camera->currentData().value<vhall::VideoDevProperty>().mDevId;
		ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, QString::fromStdString(userCameraId));
			
	}
	else if (ui.stackedWidget->currentIndex() == DEVICE_MIC) {
		mbTestMic = true;

		//更新样式
		ui.label_mic->setStyleSheet(LABEL_SUCESS);
		ui.label_ico_mic->setStyleSheet(LABEL_ICO_SUCESS);
		ui.label_ico_m->setStyleSheet(LABEL_ICO__SUCESS);
		userMicId = ui.comboBox_micDev->currentData().value<DevInfo>().Id().toStdString();
		ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, QString::fromStdString(userMicId));
	}
	else if(ui.stackedWidget->currentIndex() == DEVICE_SPEAKER){
		mbTestSpeaker = true;
		
		ui.label_ico_speak->setStyleSheet(LABEL_ICO_SUCESS);
		ui.label_ico_s->setStyleSheet(LABEL_ICO__SUCESS);
		userPlayerId = ui.comboBox_playerDev->currentData().value<DevInfo>().Id().toStdString();
		ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, QString::fromStdString(userPlayerId));
	}
	else {

	}
	ui.stackedWidget->setCurrentIndex(ui.stackedWidget->currentIndex() + 1);
}

void DeviceTestingWdg::slot_TestFailed() {
	if (ui.stackedWidget->currentIndex() == DEVICE_CAMERA) {
		mbTestCamera = false;
		globalToolManager->GetClassSDK()->StopPreviewCamera();

		//更新样式
		ui.label_camera->setStyleSheet(LABEL_FAILED);
		ui.label_camera_2->setStyleSheet(LABEL_FAILED);
		ui.label_ico_camera->setStyleSheet(LABEL_ICO_FAILED);
		ui.label_ico_c->setStyleSheet(LABEL_ICO__FAILED); 

		ui.label_ico_c->setText(QStringLiteral("异常"));
	}
	else if (ui.stackedWidget->currentIndex() == DEVICE_MIC) {
		mbTestMic = false;

		//更新样式
		ui.label_mic->setStyleSheet(LABEL_FAILED);
		ui.label_ico_mic->setStyleSheet(LABEL_ICO_FAILED);
		ui.label_ico_m->setStyleSheet(LABEL_ICO__FAILED);
		ui.label_ico_m->setText(QStringLiteral("异常"));
	}
	else if (ui.stackedWidget->currentIndex() == DEVICE_SPEAKER) {

		mbTestSpeaker = false;

		ui.label_ico_speak->setStyleSheet(LABEL_ICO_FAILED);
		ui.label_ico_s->setStyleSheet(LABEL_ICO__FAILED);

		ui.label_ico_s->setText(QStringLiteral("异常"));
	}
	else {

	}

	ui.label_result->setStyleSheet(LABEL_RESULT_FAILED);
	ui.label_title->setStyleSheet(LABEL_TITLE_FAILED);
   ui.label_title->setText(QStringLiteral("设备检测异常"));
	ui.stackedWidget->setCurrentIndex(ui.stackedWidget->currentIndex() + 1);
	mbDevTested = true;
}