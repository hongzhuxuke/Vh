#include "VhallLiveTitle.h"
#include "../Unility/Unility.h"
#include "MoreSettingWdg.h"
#include <qdebug.h>
#include "TipsWdg.h"
#include "WebRtcLiveWdg.h"
#include "VhallIALiveSettingDlg.h"

#define CREATE_DEFAULT_RECORD_TIME  30

#define QSS_MAX  "QPushButton#btnNomal{	\
border-image:url(:/button/img/button/title_Max__Normal.png);\
}\
QPushButton#btnNomal:hover{\
  border-image: url(:/button/img/button/title_Max_clicked.png);\
}\
QPushButton:pressed{\
   border-image: url(:/button/img/button/title_Max__Normal.png);\
}\
QPussButton:disable{\
   border-image: url(:/button/img/button/title_Max__Normal.png);\
}"

#define QSS_RECOVERY "QPushButton#btnNomal{	\
border-image: url(:/button/img/button/title_recovery_Normal.png);\
}\
QPushButton#btnNomal:hover{\
   border-image: url(:/button/img/button/title_recovery_click.png);\
}\
QPushButton:pressed{\
   border-image: url(:/button/img/button/title_recovery_Normal.png);\
}\
QPussButton:disable{\
   border-image:url(:/button/img/button/title_recovery_Normal.png);\
}"

extern ToolManager *globalToolManager;
VhallLiveTitle::VhallLiveTitle(QWidget *parent)
   : CWidget(parent)
{
   ui.setupUi(this);
   ui.btnNetState->setToolTip(QStringLiteral("����״̬"));
   ui.btnBeginClass->setToolTip(QStringLiteral("��/�¿�"));
   ui.btnMore->setToolTip(QStringLiteral("�������"));

   connect(ui.btnBeginClass, SIGNAL(clicked()), this, SLOT(slot_BeginClass()));
   //connect(ui.btnBeginClass, SIGNAL(clicked()), this, SLOT(slot_BeginClass()));

   ui.btnRaiseHands->SetNormalText(QStringLiteral("��������"));
   ui.btnRaiseHands->SetCountDownText(QStringLiteral("ȡ������"));
   ui.btnRaiseHands->SetCountDownTime(30);
   connect(ui.btnRaiseHands, &CCancelCountDownBtn::sigCliecked, this, &VhallLiveTitle::slot_HandsUp);

   CourseInfo baseCourseInfo = globalToolManager->GetClassSDK()->GetCourseInfo();
   QString webinar_subject = QString::fromStdString(baseCourseInfo.webinar_subject);//�������Ƽ��
   ui.labClassName->setText(webinar_subject);
   ui.label_userName->setText(QString::fromStdString(baseCourseInfo.webinar_id)); //����id

   ui.btnMore->setFocusPolicy(Qt::NoFocus);
   ui.btnMin->setFocusPolicy(Qt::NoFocus);
   ui.btnNomal->setFocusPolicy(Qt::NoFocus);
   ui.btnClose->setFocusPolicy(Qt::NoFocus);

   bool startClass = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type == CLASS_STATUS_IN_CLASS ? true : false;
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      if (startClass) {
         ui.btnBeginClass->hide();
      }
      else {
         ui.btnBeginClass->setHoverDisplayText(QStringLiteral("�Ͽ�"));
      }

      if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI 
         || globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type == CLASS_STATUS_RELAY) {
         ui.btnRaiseHands->hide();
      }
   }
   else {
      ui.btnBeginClass->setEnabled(false);
      ui.btnBeginClass->setHoverDisplayText("");
      if (!startClass) {
         ui.btnBeginClass->hide();
         ui.btnRaiseHands->hide();
      }
      else {
         //int ii = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().openhand;
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().openhand && !globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.bRrlay) {
            ui.btnRaiseHands->show();
         }
         else {
            ui.btnRaiseHands->hide();
         }
      }
   }
   ui.btnNetState->hide();
}

void VhallLiveTitle::ShowHandsUpBtn()
{
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.bRrlay)//ת����
   {
      ui.btnRaiseHands->hide();
   }
   else if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI
      && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().openhand ) {//������ �в��������
      ui.btnRaiseHands->hide();
   }
   else  if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_SMALL
      && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type == CLASS_STATUS_IN_CLASS) { //С���á��Ͽ���
      ui.btnRaiseHands->show();
   }
   else {
      ui.btnRaiseHands->hide();
   }
}

VhallLiveTitle::~VhallLiveTitle()
{
   if (nullptr != mpMoerSettingWdg)
   {
      delete mpMoerSettingWdg;
      mpMoerSettingWdg = nullptr;
   }

   if (nullptr != mpBeginClassTimer)
   {
      if (mpBeginClassTimer->isActive())
         mpBeginClassTimer->stop();
      delete mpBeginClassTimer;
      mpBeginClassTimer = nullptr;
   }

   if (nullptr != mpMoerSettingWdg)
   {
      delete mpMoerSettingWdg;
      mpMoerSettingWdg = nullptr;
   }
}

void VhallLiveTitle::ShowBeginClass(const bool& bShow /*= true*/)
{
   bShow ? ui.btnBeginClass->show() : ui.btnBeginClass->hide();
}

void VhallLiveTitle::SetStudentHandsUp(bool isPublish) {

   if (isPublish) {
      ui.btnRaiseHands->StopCountDown();
      ui.btnRaiseHands->setText(QStringLiteral("����"));
      ui.btnRaiseHands->setToolTip(QStringLiteral("����"));
      globalToolManager->GetDataManager()->SetPublished(true);
   }
   else {
      ui.btnRaiseHands->StopCountDown();
      ui.btnRaiseHands->setText(QStringLiteral("��������"));
      ui.btnRaiseHands->setToolTip(QStringLiteral("��������"));
      globalToolManager->GetDataManager()->SetPublished(false);
   }
   ui.btnRaiseHands->setEnabled(true);
}

void VhallLiveTitle::slot_changeLayout() {
   emit sig_changeLayout();
}

void VhallLiveTitle::SetTitleType(TitleType type) {
   switch (type)
   {
   case eTitleType_OpenClassLecturer: {
      ui.btnRaiseHands->hide();
   }
                                      break;
   case eTitleType_SmallClassLecturer: {
      //ui.btnAllowRaiseHands->hide();
      ui.btnRaiseHands->hide();
   }
                                       break;
   case eTitleType_Student: {
      //ui.btnAllowRaiseHands->hide();
      ui.btnBeginClass->setEnabled(false);
      //ui.btnBeginClass->hide();
      ui.btnBeginClass->setHoverDisplayText("");
   }
                            break;
   default:
      break;
   }
}



void VhallLiveTitle::startClassTime()
{
   ui.btnBeginClass->startClassTime();
}

void VhallLiveTitle::stopClassTime()
{
   ui.btnBeginClass->stopClassTime();
}

void VhallLiveTitle::SetStartClassBtnEnable(bool enable) {
   if (enable && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      ui.btnBeginClass->setEnabled(enable);
      ui.btnBeginClass->show();
   }
}

void VhallLiveTitle::SetHandsUpBtnEnable(bool enable) {
   ui.btnRaiseHands->setEnabled(enable);
}



void VhallLiveTitle::on_btnNetState_clicked(bool checked/* = false*/)
{
   emit sig_NetState();
}
void VhallLiveTitle::on_btnAllowRaiseHands_clicked(bool checked /*= false*/)
{
   emit sig_AllowRaiseHands(checked);
}

void VhallLiveTitle::hideMediaSet()
{
   creatMoerSettingWdg();
   if (nullptr != mpMoerSettingWdg)
   {
      mpMoerSettingWdg->hideMediaSet();
   }
}

void VhallLiveTitle::creatMoerSettingWdg()
{
   if (nullptr == mpMoerSettingWdg)
   {
      mpMoerSettingWdg = new MoreSettingWdg(this);
      mpMoerSettingWdg->hide();
      mpMoerSettingWdg->SetRoleType(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type);
      mpMoerSettingWdg->SetNickName(QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.nick_name));
   }
}

void VhallLiveTitle::on_btnMore_clicked(bool checked/* = false*/)
{
   creatMoerSettingWdg();
   if (nullptr != mpMoerSettingWdg)
   {
      QPoint pos = ui.groupWdg->pos() + ui.btnMore->pos() + this->pos();
      CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
      QPoint temppos = pWdg->mapToGlobal(pos);
      mpMoerSettingWdg->show(temppos.x() + 0.5*ui.btnMore->width() - 41, temppos.y() + ui.btnMore->height());
   }
}

bool VhallLiveTitle::IsEnableJoinActive() {
   std::list<vhall::VideoDevProperty> mCameraList;
   globalToolManager->GetClassSDK()->GetCameraDevices(mCameraList);
   std::list<vhall::AudioDevProperty> micList;
   globalToolManager->GetClassSDK()->GetMicDevices(micList);
   if (mCameraList.size() == 0 && micList.size() == 0) {
      CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
      if (wnd) {
         CSTDMapParam param;
         param[TipsMsg] = Str_NoDeviceCannotPublish;
         wnd->Notice(param);
         return false;
      }
   }
   return true;
}

void VhallLiveTitle::UpdateStartClassState(bool start) {
   //����ui
   if (JOIN_ROLE_Teacher == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type) {
      if (start) {//�Ͽ�״̬
         startClassTime();
         ui.btnBeginClass->setHoverDisplayText(QStringLiteral("�¿�"));
         ui.btnBeginClass->show();
         globalToolManager->GetDataManager()->SetPublished(true);
      }
      else {//�¿�״̬
         stopClassTime();
         ui.btnBeginClass->setHoverDisplayText(QStringLiteral("�Ͽ�"));
         ui.btnBeginClass->show();
         globalToolManager->GetDataManager()->SetPublished(false);
      }
   }
   else {//ѧ��
      if (start) {//�Ͽ�
         startClassTime();
         ui.btnBeginClass->show();
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().openhand && !globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.bRrlay)//����
            ui.btnRaiseHands->show();
		 else
			 ui.btnRaiseHands->hide();
      }
      else {
         stopClassTime();
         ui.btnBeginClass->setHoverDisplayText("");
         ui.btnBeginClass->hide();
         ui.btnRaiseHands->hide();
      }
   }
}

void VhallLiveTitle::setRaiseHandsState(const int& iState)
{    //��Ҫ���ж��Ƿ���ѧ�����
   globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().openhand = iState;
   if (1 == iState && JOIN_ROLE_Student == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type) {
      bool startClass = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type == CLASS_STATUS_IN_CLASS ? true : false;
      
      if (startClass && !globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.bRrlay)
         ui.btnRaiseHands->show();
      else
         ui.btnRaiseHands->hide();

      if (globalToolManager->GetDataManager()->IsAlreadyPublished())
      {
         ui.btnRaiseHands->setText(QStringLiteral("����"));
      }
      else
      {
         ui.btnRaiseHands->setText(QStringLiteral("��������"));
      }
      ui.btnRaiseHands->StopCountDown();

   }
   else {
      ui.btnRaiseHands->hide();
   }
}

void VhallLiveTitle::setMaxState(const bool & bMax)
{
   if (bMax) {
      ui.btnNomal->setStyleSheet(QSS_RECOVERY);
   }
   else {
      ui.btnNomal->setStyleSheet(QSS_MAX);
   }
}

int VhallLiveTitle::GetStartTime() {
   return ui.btnBeginClass->GetClassTime();
}

void VhallLiveTitle::CancleRaiseHands()
{
   ui.btnRaiseHands->StartCountDown(true);
   SetHandsUpBtnEnable(true);
}

void VhallLiveTitle::AddTrophyCount(const int& iAdd)
{
   creatMoerSettingWdg();
   if (nullptr != mpMoerSettingWdg)
      mpMoerSettingWdg->AddTrophyCount(iAdd);
}

int VhallLiveTitle::iTrophyCount()
{
   int iCount = 0;
   if (nullptr != mpMoerSettingWdg)
   {
      iCount = mpMoerSettingWdg->iTrophyCount();
   }
   return iCount;
}

void VhallLiveTitle::slot_BeginClass() {
 
   int class_status = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type;
   globalToolManager->GetDataManager()->WriteLog("%s %d", __FUNCTION__, class_status);
   if (class_status != CLASS_STATUS_IN_CLASS) {//δ�Ͽ�

      CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
      if (pWdg)
      {
      	WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
         if (pMainWdg) {
            pMainWdg->deleteWhiteBoradWebView();
         }
      }

      if (globalToolManager->GetClassSDK()->HasVideoOrAudioDev()) {
         if (!globalToolManager->GetClassSDK()->IsVHMediaConnected()) {
            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, QStringLiteral("ý�巿�����ӶϿ����޷����𻥶�")));
         }
         else {
            //������Ƶ�豸���������
            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_HandleStartLocalCapture, QString::number(1)));
            SetStartClassBtnEnable(false);
         }
      }
      else {
		  globalToolManager->GetDataManager()->WriteLog("%s Str_NoDeviceCannotPublish", __FUNCTION__);
		  NoDevShowTips();
         //globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_NoDeviceCannotPublish));
      }
   }
   else {
	   globalToolManager->GetDataManager()->WriteLog("%s sig_StopClass", __FUNCTION__);
       emit sig_StopClass();
   }
   ui.btnBeginClass->setEnabled(false);
   if (nullptr == mpBeginClassTimer)
   {
      mpBeginClassTimer = new QTimer(this);
      connect(mpBeginClassTimer, &QTimer::timeout, this, &VhallLiveTitle::slot_EnabledBeginClass);
   }
   if (nullptr != mpBeginClassTimer && !mpBeginClassTimer->isActive())
      mpBeginClassTimer->start(5000);
}

void VhallLiveTitle::NoDevShowTips() {
	TipsWdg* tips = new TipsWdg(this, QStringLiteral("ϵͳ����豸�쳣���������ݼ��������Ժ����·���ֱ��"), "");
	if (tips) {

		int accept = tips->exec();
		if (accept == QDialog::Accepted) {
			QDesktopServices::openUrl(QUrl("https://www.vhall.com/saas/doc/1722.html"));
		}
		else {
			tips->hide();
			delete tips;
			tips = NULL;
		}
	}
}

void VhallLiveTitle::slot_StopClass() {
	globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
   //CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_TIPS_WND);
   TipsWdg* tips = new TipsWdg(this);
   if (tips) {
      CSTDMapParam param;
      param[TipsMsg] = QStringLiteral("�Ƿ�ȷ���¿�?");
      param[ShowCancelButton] = PARAM_TRUE;
      if (ui.btnBeginClass->GetClassTime() > CREATE_DEFAULT_RECORD_TIME) {
         param[ShowRecordSet] = "1";
      }
      int createDefaultRecord = 0;
      tips->Notice(param);
      int accept = tips->exec();
      if (accept == QDialog::Accepted) 
      {
         emit sig_OverClass();
         globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
         tips->GetParam(param);
         std::map<QString, QString>::iterator iterShowRecordSet = param.find(QString::fromStdString(ShowRecordSet));
         if (iterShowRecordSet != param.end()) {
            createDefaultRecord = iterShowRecordSet->second.toInt() == Qt::CheckState::Checked ? 1 : 0;
         }
         globalToolManager->GetClassSDK()->StopClass(createDefaultRecord);
         UpdateStartClassState(false);
         //���� ���������� false
         //CBaseWnd* wnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
         //CBaseWnd* wnd = nullptr;
         CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
         //if (pWdg)
         //{
         //	WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
         //	//wnd = pMainWdg->GetPtrVhallIALiveSettingDlg();
         //}

         if (pWdg) {
            CSTDMapParam mapParam;
            mapParam[Str_Quality_Enabeld] = Str_Quality_Enabeld;
            pWdg->Notice(mapParam);
            //wnd->exec();
         }


      }
      //globalToolManager->GetWndManager()->DestoryWnd(WND_ID_TIPS_WND);
   }
}

void VhallLiveTitle::slot_EnabledBeginClass()
{
   ui.btnBeginClass->setEnabled(true);
   mpBeginClassTimer->stop();
}

void VhallLiveTitle::slot_HandsUp(bool bCountDown) {
   if (!globalToolManager->GetDataManager()->IsAlreadyPublished()) {

      if (bCountDown)//����ʱ�У�ȡ������
      {
         ui.btnRaiseHands->setEnabled(false);
         globalToolManager->GetClassSDK()->CanclePublish();
      }
      else
      {
         //����
         ui.btnRaiseHands->setEnabled(false);
         globalToolManager->GetClassSDK()->ApplyPublish();
      }
   }
   else {
      //����
      globalToolManager->GetDataManager()->WriteLog("%s  PushStreamEvent_UnPublish 415", __FUNCTION__);
      globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish);
   }
}
void VhallLiveTitle::slot_AllowHandsUp() {
   globalToolManager->GetClassSDK()->OpenUserHandsUp(true);
}

void VhallLiveTitle::on_btnMin_clicked(bool checked /*= false*/)
{
   emit sig_min();
}
void VhallLiveTitle::on_btnNomal_clicked(bool checked /*= false*/)
{
   emit sig_normal();
}
void VhallLiveTitle::on_btnClose_clicked(bool checked /*= false*/)
{
   emit sig_close();
}
