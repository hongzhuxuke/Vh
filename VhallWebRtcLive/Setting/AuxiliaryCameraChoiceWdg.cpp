#include "AuxiliaryCameraChoiceWdg.h"
#include <QApplication>
#include <QAbstractItemView>
#include "ConfigSetting.h"
#include "pathmanager.h"
#include "TipsWdg.h"

extern ToolManager *globalToolManager;
CREATE_WND_FUNCTION(AuxiliaryCameraChoiceWdg);

AuxiliaryCameraChoiceWdg::AuxiliaryCameraChoiceWdg(QWidget *parent)
   : CBaseWnd(parent)
{
   ui.setupUi(this);
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);
	setModal(true);
   ui.cmbcamera->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
   ui.cmbcamera->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

   connect(ui.pushButton_apply, SIGNAL(clicked()), this, SLOT(slot_OnClickedApply()));
   connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_OnClickedClose()));
	//ui.pushButton_apply->setEnabled(false);
}

AuxiliaryCameraChoiceWdg::~AuxiliaryCameraChoiceWdg()
{
   stopTimer();
   if (nullptr != mpOvertimeTimer) {
      delete mpOvertimeTimer;
      mpOvertimeTimer = nullptr;
   }
}

void AuxiliaryCameraChoiceWdg::Notice(const CSTDMapParam &) {
   std::string cameraId = "";
   std::list<vhall::VideoDevProperty> cameraList;
   globalToolManager->GetClassSDK()->GetCameraDevices(cameraList);
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
   QString msConfPath = globalToolManager->GetConfigPath();
   QString configCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, "");
   int openPic = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 0);
	//ui.pushButton_apply->setEnabled(false);

   std::list<vhall::VideoDevProperty>::iterator iter = cameraList.begin();
   ui.cmbcamera->clear();
   while (iter != cameraList.end()) {

	   if (1 == openPic || iter->mDevId != configCameraId.toStdString()) {

         if (cameraId.empty()) {
            cameraId = iter->mDevId;
         }

		   ui.cmbcamera->addItem(QString::fromStdString(iter->mDevName), QVariant::fromValue(*iter));
	   }
	   iter++;
   }

   StartLocalAuxiliaryCapture(cameraId);
}

void AuxiliaryCameraChoiceWdg::StartLocalAuxiliaryCapture(std::string cameraId)
{
   if (!cameraId.empty()) {
      //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	   QString msConfPath = globalToolManager->GetConfigPath();
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, DOUBLE_CAMERA_ID, QString::fromStdString(cameraId));

      //QString configDefinition = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, Fluent_definition);
      //int interactiveVideoProfile = 360;
      //int mixLayoutProfile = 720;
      //globalToolManager->GetVideoProfileByConfig(configDefinition, interactiveVideoProfile, mixLayoutProfile);
      //VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(interactiveVideoProfile);

      int iQualityId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, "1").toInt();
      globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, iQualityId);
      int iVioeProFile = 360;
      globalToolManager->GetVideoProfileByConfig(iQualityId, iVioeProFile, ToolManager::eVideoProFileTypeDoublePush);
      VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(iVioeProFile);

      ui.cmbcamera->setEnabled(false);
      //ui.pushButton_apply->setEnabled(true);
      this->setEnabled(true);

      globalToolManager->GetClassSDK()->StartLocalAuxiliaryCapture(cameraId, videoIndex);

      if (nullptr == mpOvertimeTimer) {
         mpOvertimeTimer = new QTimer(this);
         connect(mpOvertimeTimer, &QTimer::timeout, this, &AuxiliaryCameraChoiceWdg::slotOvertime);
      }
      if (nullptr != mpOvertimeTimer) {
         mpOvertimeTimer->start(5000);
      }

   }
}

void AuxiliaryCameraChoiceWdg::slotOvertime()
{
   stopTimer();
   ui.cmbcamera->setEnabled(true);
   //ui.pushButton_apply->setEnabled(true);
   this->setEnabled(true);
}

void AuxiliaryCameraChoiceWdg::stopTimer()
{
   if (nullptr != mpOvertimeTimer && mpOvertimeTimer->isActive()) {
      mpOvertimeTimer->stop();
   }
}

void AuxiliaryCameraChoiceWdg::Notice(const CSTDMapParamPtr &) {

}

void AuxiliaryCameraChoiceWdg::PreviewAuxiliaryCamera(QEvent*event)
{
   switch (event->type())
   {
   case CustomEvent_OnCaptureCallback: {
      QEventStream *customEvent = dynamic_cast<QEventStream*>(event);
      if (customEvent && customEvent->mStreamType == vlive::VHStreamType::VHStreamType_Auxiliary_CamerCapture) {
         HWND *renderWnd = (HWND*)ui.widget_render->winId();
         if (customEvent->mIVHCapture == VHCapture_OK) {
            mbCaptureSuccess = true;
            globalToolManager->GetClassSDK()->StartRenderLocalStream(vlive::VHStreamType::VHStreamType_Auxiliary_CamerCapture, renderWnd);
            ui.cmbcamera->setEnabled(true);
				//ui.pushButton_apply->setEnabled(true);
         }
         else {
            mbCaptureSuccess = false;
				//ui.pushButton_apply->setEnabled(false);
            ui.widget_render->repaint();
         }
      }
      break;
   }
   default:
      break;
   }
}

void AuxiliaryCameraChoiceWdg::EnableUI()
{
   ui.cmbcamera->setEnabled(false);
   //ui.pushButton_apply->setEnabled(true);
   this->setEnabled(true);
}

void AuxiliaryCameraChoiceWdg::on_cmbcamera_currentIndexChanged(int index)
{
   QVariant vat = ui.cmbcamera->itemData(index);
   vhall::VideoDevProperty CamreDev = vat.value<vhall::VideoDevProperty>();// Qt::UserRole
   std::string cameraId = CamreDev.mDevId;
   StartLocalAuxiliaryCapture(cameraId);
}

//void AuxiliaryCameraChoiceWdg::customEvent(QEvent* event) {
//   if (event) {
//      switch (event->type())
//      {
//      case CustomEvent_OnCaptureCallback: {
//         QEventStream *customEvent = dynamic_cast<QEventStream*>(event);
//         if (customEvent && customEvent->mStreamType == vlive::VHStreamType::VHStreamType_Auxiliary_CamerCapture) {
//               HWND *renderWnd = (HWND*)ui.widget_render->winId();
//               if (customEvent->mIVHCapture == VHCapture_OK) {
//                  mbCaptureSuccess = true;
//                  globalToolManager->GetClassSDK()->StartRenderLocalStream(vlive::VHStreamType::VHStreamType_Auxiliary_CamerCapture, renderWnd);
//               }
//               else {
//                  mbCaptureSuccess = false;
//                  ui.widget_render->repaint();
//               }
//         }
//         break;
//      }
//      default:
//         break;
//      }
//   }
//}

void AuxiliaryCameraChoiceWdg::slot_OnClickedApply() {
  // if (mbCaptureSuccess) {
  //    globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_StartRenderCameraAuxiliary, ""));
		//hide();
		//close();
  // }
	globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_StartRenderCameraAuxiliary, ""));
	hide();
	close();

   //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_CAMERA_DOUBLE_PUSH_UI));
}

void AuxiliaryCameraChoiceWdg::slot_OnClickedClose() {
	TipsWdg* tips = new TipsWdg(this);
	if (tips) {
		CSTDMapParam param;
		param[TipsMsg] = QStringLiteral("是否确认结束双推?");
		param[ShowCancelButton] = PARAM_TRUE;
		tips->Notice(param);
	}
	int accept = tips->exec();
	if (accept == QDialog::Accepted) {
		globalToolManager->GetClassSDK()->StopLocalAuxiliaryCapture();
		hide();
		close();
	}
   //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_CAMERA_DOUBLE_PUSH_UI));
}