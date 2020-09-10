#include "MemberItemWdg.h"
#include <QPainter>
#include <QListWidgetItem> 
#include <QTimer>
#include "MenuWdg.h"
#include "WebRtcLiveWdg.h"
#include "Menu.h"
#include "CAction.h"
#include "priConst.h"
//#include "cbasedlg.h"
#include "DebugTrace.h"
#include "../Unility/Unility.h"

#define ROLE_USER  100
#define ROLE_ASSISTANT  200
#define ROLE_GUEST  300
#define ROLE_SUPERVISE 400
#define ROLE_ADMIN 500
#define ROLE_TEACHER  600
#define ROLE_offline  700

#define FORBIDDEN   QStringLiteral("聊天禁言")
#define CANCELFORBIDDEN QStringLiteral("取消禁言")
//#define KICKOUT  QStringLiteral("踢出课堂")
//#define CANCELKICKOUT QStringLiteral("取消踢出")

#define ROLE_Teacher_Style "QLabel{\
												font: 9px \"微软雅黑\"; \
												color: rgb(82, 204, 144); \
												background-color: rgb(87, 102, 102); \
												border-radius:7px; \
											}"
#define ROLE_Assistant_Style  "QLabel{\
													font: 9px \"微软雅黑\";\
													color: rgb(77, 161, 255);\
													background-color: rgb(89, 96, 106);\
													border-radius:7px;\
												}"

#define MENUiTEMHOVER   "QToolButton{ \
	border:none;\
	font - size: 12px; \
	color: #52CC90;  /*字体颜色*/\
}"          

#define MENUiTEMLEAVE "QToolButton{ border:none;\
	font-size: 12px; \
	color: #666666;  /*字体颜色*/\
}\
QToolButton:hover{	color: #52CC90;  /*字体颜色*/\
}"

#define FORBIDDENHOVER QStringLiteral("QToolButton{ \
	border-image: url(:/ico/img/button/forbidden2.ico); \
}")

#define FORBIDDENLEAVE QStringLiteral("QToolButton{ \
	border:none;\
	border-image: url(:/ico/img/button/forbidden3.ico); \
} \
QToolButton:hover{ \
	border-image: url(:/ico/img/button/forbidden2.ico); \
}")

#define CANCLEFORBIDDENHOVER QStringLiteral("QToolButton{ \
	border-image: url(:/ico/img/button/unForbidden2.ico); \
}")

#define CANCLEFORBIDDENLEAVE QStringLiteral("QToolButton{ \
	border:none;\
	border-image: url(:/ico/img/button/unForbidden1.ico); \
} \
QToolButton:hover{ \
	border-image: url(:/ico/img/button/unForbidden2.ico); \
}")

#define KICKOUTHOVER QStringLiteral("QToolButton{ \
	border-image: url(:/ico/img/button/kickout2.ico); \
}")

#define KICKOUTLEAVE QStringLiteral("QToolButton{ \
	border:none;\
	border-image: url(:/ico/img/button/kickout3.ico); \
} \
QToolButton:hover{ \
	border-image: url(:/ico/img/button/kickout2.ico); \
}")

#define CANCLEKICKOUTHOVER QStringLiteral("QToolButton{ \
	border-image: url(:/ico/img/button/unkickout2.ico); \
}")

#define CANCLEKICKOUTLEAVE QStringLiteral("QToolButton{ \
	border:none;\
	border-image: url(:/ico/img/button/unkickout1.ico); \
} \
QToolButton:hover{ \
	border-image: url(:/ico/img/button/unkickout2.ico); \
}")

extern ToolManager *globalToolManager;

COnlineItemWdg::COnlineItemWdg(QWidget *parent)
	: CWidget(parent)
{
	ui.setupUi(this);
	static int iStatiId = 0;
	iStatiId++;
	m_iStaticId = iStatiId;
	createAction();
	ui.labKickOut->hide();
	ui.labForbidden->hide();
	ui.labNoMic->hide();
	this->setMaximumWidth(320);
	ui.btnUpper->setFocusPolicy(Qt::NoFocus);
}

void COnlineItemWdg::createAction()
{
	//ui.labMoreOp->hide();

	mPMoreOpMenu = new MenuWdg(this);
	mPMoreOpMenu->addItem(FORBIDDEN, MENUiTEMHOVER, MENUiTEMLEAVE, eOptype_Forbidden, FORBIDDENHOVER, FORBIDDENLEAVE);
	mPMoreOpMenu->addItem(CANCELFORBIDDEN, MENUiTEMHOVER, MENUiTEMLEAVE, eOptype_CancleForbidden, CANCLEFORBIDDENHOVER, CANCLEFORBIDDENLEAVE);
	mPMoreOpMenu->addItem(KICKOUT, MENUiTEMHOVER, MENUiTEMLEAVE, eOptype_KickOut, KICKOUTHOVER, KICKOUTLEAVE);
	mPMoreOpMenu->addItem(CANCELKICKOUT, MENUiTEMHOVER, MENUiTEMLEAVE, eOptype_CancleKickOut, CANCLEKICKOUTHOVER, CANCLEKICKOUTLEAVE);
	connect(mPMoreOpMenu, &MenuWdg::sigClicked, this, &COnlineItemWdg::slotCliecked);

}

COnlineItemWdg::COnlineItemWdg(COnlineItemWdg* pitem, QWidget *parent /*= Q_NULLPTR*/)
	: CWidget(parent)
{
	ui.setupUi(this);
	static int iStatiId = 990;
	iStatiId++;
	m_iStaticId = iStatiId;
	ui.labName->setText(pitem->GetName());
	mStrId = pitem->mStrId;
	miRole = pitem->miRole;
	initUserRoleId();
	CalcWeight();
	createAction();
	this->setMaximumWidth(320);
	ui.btnUpper->setFocusPolicy(Qt::NoFocus);
}

COnlineItemWdg::~COnlineItemWdg()
{
	//add_log("ixk", "%s  %d", __FUNCTION__, m_iStaticId);
	if (NULL != mpUpperTime)
	{
		if (mpUpperTime->isActive())
		{
			mpUpperTime->stop();
		}
		mpUpperTime->deleteLater();
		mpUpperTime = NULL;
	}
	if (nullptr != mPMoreOpMenu)
	{
		delete mPMoreOpMenu;
		mPMoreOpMenu = NULL;
	}
}

bool COnlineItemWdg::isCurrentUser(const QString& strRole)
{
	return mStrId.compare( strRole.trimmed() )==0;
}

QString COnlineItemWdg::GetId()
{
	return mStrId;
}

int COnlineItemWdg::GetRole()
{
	return miRole;
}

void COnlineItemWdg::slotCliecked(const int& iOpType)
{
	switch (iOpType)
	{
	case eOptype_Forbidden:
		slots_Forbidden();
		break;
	case	eOptype_CancleForbidden:
		slots_CancleForbidden();
		break;
	case eOptype_KickOut:
		slots_KickOut();
		break;
	case eOptype_CancleKickOut:
		slots_CancleKickOut();
		break;
	default:
		break;
	}
}

void COnlineItemWdg::slots_Forbidden() { 
	globalToolManager->GetClassSDK()->ForbiddenChat(mStrId.toStdString(), true);
}

void COnlineItemWdg::slots_CancleForbidden() {
	globalToolManager->GetClassSDK()->ForbiddenChat(mStrId.toStdString(), false);
}
void COnlineItemWdg::slots_KickOut() {//踢出
	if (eRaiseHandsState_Speaking == miRaiseHandsState) {
      globalToolManager->GetDataManager()->WriteLog("%s KickUserPublish", __FUNCTION__);
		KickUserPublish();
	}
	
	globalToolManager->GetClassSDK()->KickOutClassRoom(mStrId.toStdString(), 1);
}
void COnlineItemWdg::slots_CancleKickOut() {//取消踢出
	globalToolManager->GetClassSDK()->KickOutClassRoom(mStrId.toStdString(), 0);
}

void COnlineItemWdg::on_btnMoreOp_clicked(bool checked /*= false*/)
{
	//QMenu* MoreOpMenu = ui.btnMoreOp->menu();
	if (nullptr == mPMoreOpMenu) return;
	

	if (ui.labKickOut->isVisible())//踢出
	{
		mPMoreOpMenu->SetItemShow(eOptype_CancleKickOut, true);
		mPMoreOpMenu->SetItemShow(eOptype_Forbidden, false);
		mPMoreOpMenu->SetItemShow(eOptype_CancleForbidden, false);
		mPMoreOpMenu->SetItemShow(eOptype_KickOut, false);
		mPMoreOpMenu->SetShowItemCount(1);
	}
	else//在房间
	{
		mPMoreOpMenu->SetItemShow(eOptype_CancleKickOut, false);
		mPMoreOpMenu->SetItemShow(eOptype_KickOut, true);

		if (ui.labForbidden->isVisible())//被禁言中
		{
			mPMoreOpMenu->SetItemShow(eOptype_Forbidden, false);
			mPMoreOpMenu->SetItemShow(eOptype_CancleForbidden, true);
		}
		else
		{
			mPMoreOpMenu->SetItemShow(eOptype_Forbidden, true);
			mPMoreOpMenu->SetItemShow(eOptype_CancleForbidden, false);
		}
		mPMoreOpMenu->SetShowItemCount(2);
	}	
	QPoint pos = mapToGlobal(/*temppos +*/ ui.btnMoreOp->pos());

	int iX = pos.x();
	int iY = pos.y();

	mPMoreOpMenu->show(iX - mPMoreOpMenu->width() + 10, iY -8);
}

void COnlineItemWdg::SetForbiddenState(const bool& bForbidden)
{
	mbForbiddenState = bForbidden;
	if (bForbidden && JOIN_ROLE_Teacher != miRole)
	{
		ui.labForbidden->show();
	}
	else
	{
		ui.labForbidden->hide();
	}
}

void COnlineItemWdg::SetKickOutState(const bool& bstate)
{
	mbKickOutState = bstate;
	if (bstate)
	{
		ui.labUpper->hide();
		ui.btnUpper->hide();
		ui.labKickOut->show();
	}
	else
	{
		ui.labKickOut->hide();
	}
}


int COnlineItemWdg::GetOnlineStatusStates()
{
	return mOnlineStatus;
}

int COnlineItemWdg::GetNoMic()
{
	return mINoMic;
}

int COnlineItemWdg::GetRaiseHandsState()
{
	return miRaiseHandsState;
}
void COnlineItemWdg::SetNoMic(const int& iNoMic)
{
	mINoMic = iNoMic;
	if (1 == iNoMic)
	{
		ui.labNoMic->show();
		ui.btnUpper->hide();
	}
	else {
		ui.labNoMic->hide();
      if (JOIN_ROLE_Student == miRole || JOIN_ROLE_Student == miRole){
         ui.btnUpper->show();
      }
	}

}


void COnlineItemWdg::updateInfo(const CMember& info)
{
	ui.labName->setText(info.nick_name);
	SetNoMic(info.no_mic);
	miRole = info.role_type;
	initUserRoleId();
	mOnlineStatus = info.status;
	CalcWeight();
}

void COnlineItemWdg::initUserRoleId()
{
	switch (miRole)
	{
	case JOIN_ROLE_Teacher: {// 1 : 老师
		ui.btnUpper->hide();
		ui.btnMoreOp->setEnabled(false);
//		ui.labMoreOp->show();
		ui.labRoleImage->setPixmap(QPixmap(":/idention/img/idention/lecturerHead.png"));
		ui.labIdentityTile->setPixmap(QPixmap(":/idention/img/idention/lecturerRole.png"));
		ui.labIdentityTile->setStyleSheet(ROLE_Teacher_Style);
	}
							break;
	case JOIN_ROLE_Student: { // 2 ; 学员
		ui.labRoleImage->setPixmap(QPixmap(":/idention/img/idention/StudentHead.png"));
		//ui.labIdentityTile->setText(QString::fromStdWString(L""));
		ui.labIdentityTile->hide();
	}
							break;
	case JOIN_ROLE_Assistant: { // 3 : 助教
		ui.btnUpper->hide();
		ui.btnMoreOp->setEnabled(false);
		ui.labRoleImage->setPixmap(QPixmap(":/idention/img/idention/StudentHead.png"));
		ui.labIdentityTile->setPixmap(QPixmap(":/idention/img/idention/AssistantRole.png"));
		ui.labIdentityTile->setStyleSheet(ROLE_Assistant_Style);
	}
							  break;
	case JOIN_ROLE_Guest: {// 4 : 嘉宾
		ui.btnUpper->show();
		ui.btnMoreOp->setEnabled(false);
		ui.labRoleImage->setPixmap(QPixmap(":/idention/img/idention/assistantHead.png"));
		ui.labIdentityTile->setPixmap(QPixmap(":/idention/img/idention/GuestRole.png"));
		ui.labIdentityTile->setStyleSheet(ROLE_Assistant_Style);
	}
						  break;
	case JOIN_ROLE_Supervise: {//5 : 监课
		ui.btnUpper->hide();
        ui.btnMoreOp->setEnabled(false);
		ui.labRoleImage->setPixmap(QPixmap(":/idention/img/idention/assistantHead.png"));
		ui.labIdentityTile->setText(QString::fromStdWString(L"监课"));
		ui.labIdentityTile->setStyleSheet(ROLE_Assistant_Style);
	}
							  break;
	case JOIN_ROLE_Admin: {//6 : 管理员
		ui.btnUpper->hide();
		ui.btnMoreOp->setEnabled(false);
		ui.labRoleImage->setPixmap(QPixmap(":/idention/img/idention/assistantHead.png"));
		ui.labIdentityTile->setText(QString::fromStdWString(L"管理员"));
		ui.labIdentityTile->setStyleSheet(ROLE_Assistant_Style);
	}
						  break;
	default: break;
	}
}
void COnlineItemWdg::SetUserInfo(const CMember& info)
{
	mStrId = info.user_id;
	updateInfo(info);
}

void COnlineItemWdg::enterEvent(QEvent *event)
{
   ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (QString::fromStdString( roomInfo.mUserInfo.join_id ).compare(mStrId) != 0)
	{
		int LoginRol = roomInfo.mUserInfo.role_type;//当前登陆用户类型
	
		if (JOIN_ROLE_Teacher == LoginRol && (JOIN_ROLE_Student == miRole || JOIN_ROLE_Guest == miRole))//讲师对学员
		{
			ui.btnMoreOp->setEnabled(true);
			//ui.btnMoreOp->show();
//			ui.labMoreOp->hide();
		}
	}
	
    CWidget::enterEvent(event);
}

void COnlineItemWdg::leaveEvent(QEvent *event)
{
   ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (QString::fromStdString(roomInfo.mUserInfo.join_id).compare(mStrId) != 0)
	{
		int LoginRol = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;//当前登陆用户类型

		if (JOIN_ROLE_Teacher == LoginRol && (JOIN_ROLE_Student == miRole || JOIN_ROLE_Guest == miRole))//讲师对学员
		{
			ui.btnMoreOp->setEnabled(false);
//			ui.labMoreOp->show();
		}

	}
    CWidget::leaveEvent(event);
}

QString COnlineItemWdg::GetName()
{
	return ui.labName->text().trimmed();
}

void COnlineItemWdg::SetUpperStates(const int& iRaiseHandsState)
{
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	miRaiseHandsState = iRaiseHandsState;
    //ui.btnUpper->setEnabled(true);
    //globalToolManager->GetDataManager()->WriteLog("%s ui.btnUpper->setEnabled(true) mStrId %s Name %s",
    //   __FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data() );

	if (miRole == JOIN_ROLE_Student || JOIN_ROLE_Guest == miRole)
	{
		if (eRaiseHandsState_Speaking == miRaiseHandsState)//连麦中
		{
			ui.labUpper->show();
			ui.labUpper->setPixmap(QPixmap(":/button/img/button/hansUp_down.png"));
			ui.btnUpper->setText(DOWNMIC);//显示下麦下麦

			if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
				//emit sig_UserSpeaking(true);
				globalToolManager->GetDataManager()->WriteLog("%s eRaiseHandsState_Speaking isHasSpeaked:%s", __FUNCTION__, "1");
			}

		}
		else if (eRaiseHandsState_normal == miRaiseHandsState)//正常状态
		{
			if (NULL != mpUpperTime && mpUpperTime->isActive())
			{
				mpUpperTime->stop();
			}
			ui.btnUpper->setText(INVITATIONTOBARLEY);//显示邀请上麦
			ui.labUpper->hide();
			if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
				//emit sig_UserSpeaking(false);
				globalToolManager->GetDataManager()->WriteLog("%s eRaiseHandsState_normal isHasSpeaked:%s", __FUNCTION__, "0");
			}
		}
		else if (eRaiseHandsState_RaiseHands == miRaiseHandsState)//举手中
		{
			ui.labUpper->show();
			ui.labUpper->setPixmap(QPixmap(":/button/img/button/hansUp.png"));
			ui.btnUpper->setText(UPPERWHEAT);//显示上麦
			if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
				//emit sig_UserSpeaking(false);
				globalToolManager->GetDataManager()->WriteLog("%s eRaiseHandsState_RaiseHands isHasSpeaked:%s", __FUNCTION__, "0");
			}
		}
	}
	else if (JOIN_ROLE_Teacher == miRole)
	{
		ui.labUpper->show();
		ui.labUpper->setPixmap(QPixmap(":/idention/img/idention/FivePointedStar.png"));
	}
	else
	{
		ui.btnUpper->hide();
		ui.labUpper->hide();
	}
	

	CalcWeight();
}

int COnlineItemWdg::GetWeight()
{
	return mIWeight;
}

bool COnlineItemWdg::operator==(const COnlineItemWdg &obj)
{
	return this->mIWeight == obj.mIWeight;
}

bool COnlineItemWdg::operator>=(const COnlineItemWdg& obj)
{
	return this->mIWeight >= obj.mIWeight;
}

bool COnlineItemWdg::operator>(const COnlineItemWdg& obj)
{
	return this->mIWeight > obj.mIWeight;
}

bool COnlineItemWdg::operator<=(const COnlineItemWdg& obj)
{
	return this->mIWeight <= obj.mIWeight;
}

bool COnlineItemWdg::operator<(const COnlineItemWdg& obj)
{
	return this->mIWeight < obj.mIWeight;
}

int COnlineItemWdg::CalcWeight()
{
	mIWeight = 1000;
	switch (miRole)
	{
	case JOIN_ROLE_Teacher: {// 1 : 老师
		mIWeight -= ROLE_TEACHER;
	}
							break;
	case JOIN_ROLE_Student: { // 2 ; 学员
		mIWeight -= ROLE_USER;
	}
							break;
	case JOIN_ROLE_Assistant: { // 3 : 助教
		mIWeight -= ROLE_ASSISTANT;
	}
							  break;
	case JOIN_ROLE_Guest: {// 4 : 嘉宾
		mIWeight -= ROLE_GUEST;
	}
						  break;
	case JOIN_ROLE_Supervise: {//5 : 监课
		mIWeight -= ROLE_SUPERVISE;
	}
							  break;
	case JOIN_ROLE_Admin: {//6 : 管理员
		mIWeight -= ROLE_ADMIN;
	}
						  break;
	default: break;
	}
	//globalToolManager->GetDataManager()->WriteLog("%s ID=%s nickname:%s  RolType: %d mIWeight[0] = %d",  \
	//	__FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().trimmed().toStdString().c_str(), miRole, mIWeight);

	mIWeight -= miRaiseHandsState;//排麦：顺序  讲话中、举手、下麦

	//if (0 == mOnlineStatus)//不在线
	//{
	//	mIWeight += ROLE_TEACHER;
	//}

	//globalToolManager->GetDataManager()->WriteLog("%s ID=%s nickname:%s  RolType: %d mIWeight[1] = %d mOnlineStatus=%d", \
	//	__FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().trimmed().toStdString().c_str(), miRole, mIWeight, mOnlineStatus);

	if (mbForbiddenState)//禁言
		mIWeight += 2;

	//globalToolManager->GetDataManager()->WriteLog("%s ID=%s nickname:%s  RolType: %d mIWeight[2] = %d mbForbiddenState=%d",\
	//	__FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().trimmed().toStdString().c_str(), miRole, mIWeight, mbForbiddenState);

	if (1==mINoMic)//设备异常
		mIWeight += 3;
	//globalToolManager->GetDataManager()->WriteLog("%s ID=%s nickname:%s  RolType: %d mIWeight[3] = %d mINoMic=%d",\
	//	__FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().trimmed().toStdString().c_str(), miRole, mIWeight, mINoMic);

	if (mbKickOutState)//踢出
		mIWeight += 4;
	globalToolManager->GetDataManager()->WriteLog("%s ID=%s  RolType: %d mIWeight[4] = %d mbKickOutState=%d",__FUNCTION__, mStrId.toStdString().c_str(), miRole, mIWeight, mbKickOutState);

	if (NULL != mpItem)
	{
		mpItem->setText(QString::number(mIWeight));
	}

	return mIWeight;
}

void COnlineItemWdg::on_btnUpper_clicked(bool checked /*= false*/)
{
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	switch (miRaiseHandsState)
	{
	case eRaiseHandsState_normal://邀请上麦
	{/*IsUserSpeaking()*/
		bool isHasSpeaked = false;
      CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
      WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;

		if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
		   if (pWebRtcLiveWdg &&  pWebRtcLiveWdg->isStudentSpeaking() ) {
			   isHasSpeaked = true;
            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_IsInvitedUser));
			   globalToolManager->GetDataManager()->WriteLog("%s isHasSpeaked:%s", __FUNCTION__, "1");
            return;
		   }
	    }

         if (NULL == mpUpperTime) {
            globalToolManager->GetDataManager()->WriteLog("%s isHasSpeaked:%s  mpUpperTime is NULL ", __FUNCTION__, "0");
         }
         else {
            globalToolManager->GetDataManager()->WriteLog("%s isHasSpeaked:%s  mpUpperTime isisActive(%d）?", __FUNCTION__, "0", mpUpperTime->isActive());
         }
			

         if (pWebRtcLiveWdg->StartInvitationSpeech()/*NULL == mpUpperTime || (mpUpperTime && !mpUpperTime->isActive())*/) {
            globalToolManager->GetDataManager()->WriteLog("%s isHasSpeaked:%s", __FUNCTION__, mStrId.toStdString().c_str());

            //globalToolManager->GetClassSDK()->AskforPublish(mStrId.toStdString());
            emit sign_AskforPublish(mStrId);
            //mbIsInviting = true;
            startUpperTime(5000); //10秒
            //ui.btnUpper->setEnabled(false);
            //globalToolManager->GetDataManager()->WriteLog("%s ui.btnUpper->setEnabled(false) mStrId %s Name %s 629",
            //   __FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data());

            globalToolManager->GetDataManager()->WriteLog("%s mpUpperTime->start 5000", __FUNCTION__ );
			   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
				   //emit sig_UserSpeaking(true);
				   globalToolManager->GetDataManager()->WriteLog("%s mpUpperTime->start(5000) isHasSpeaked:%s", __FUNCTION__, "1");
			   }
	        
         }
         else {
            if (NULL == mpUpperTime) {
               globalToolManager->GetDataManager()->WriteLog("%s Str_IsInvitingUser  isHasSpeaked:%s  mpUpperTime is NULL ", __FUNCTION__, "0");
            }
            else {
               globalToolManager->GetDataManager()->WriteLog("%s Str_IsInvitingUser isHasSpeaked:%s  mpUpperTime isisActive(%d）?", __FUNCTION__, "0", mpUpperTime->isActive());
            }

            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_IsInvitingUser));
         }
		//}
	}
		break;
	case eRaiseHandsState_RaiseHands: { //批准上麦
         //if()
         globalToolManager->GetDataManager()->WriteLog("%s AuditPublish::AuditPublish_Accept %s", __FUNCTION__, mStrId.toStdString().c_str());

         //globalToolManager->GetClassSDK()->AuditPublish(mStrId.toStdString()); 
         emit sign_AuditPublish(mStrId);
         
			if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
				//emit sig_UserSpeaking(true);
				globalToolManager->GetDataManager()->WriteLog("%s eRaiseHandsState_RaiseHands isHasSpeaked:%s", __FUNCTION__, "1");
			}
         startUpperTime(5000);
         //ui.btnUpper->setEnabled(false);
         //globalToolManager->GetDataManager()->WriteLog("%s ui.btnUpper->setEnabled(false) mStrId %s Name %s 666",
         //   __FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data());
        }
		break;
	case eRaiseHandsState_Speaking://将其踢下麦(miRaiseHandsState)
      globalToolManager->GetDataManager()->WriteLog("%s KickUserPublish", __FUNCTION__);

		KickUserPublish();

		if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
			//emit sig_UserSpeaking(false);
			globalToolManager->GetDataManager()->WriteLog("%s eRaiseHandsState_Speaking isHasSpeaked:%s", __FUNCTION__, "1");
		}
		break;
	}
   globalToolManager->GetDataManager()->WriteLog("%s  Leave", __FUNCTION__);
}

void COnlineItemWdg::startUpperTime(const int & iTime)
{
   if (NULL == mpUpperTime) {
      mpUpperTime = new QTimer();
      connect(mpUpperTime, &QTimer::timeout, this, &COnlineItemWdg::slot_upperTimeOut);
   }
   else if (NULL != mpUpperTime && mpUpperTime->isActive())
   {
      mpUpperTime->stop();
   }

   if (NULL != mpUpperTime && mpUpperTime->isActive())
   {
      mpUpperTime->start(iTime);
      globalToolManager->GetDataManager()->WriteLog("%s mStrId %s Name %s",
         __FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data());
   }
   
}

void COnlineItemWdg::KickUserPublish()
{
	//ui.btnUpper->setEnabled(false);
 //  globalToolManager->GetDataManager()->WriteLog("%s ui.btnUpper->setEnabled(false) mStrId %s Name %s 704",
 //     __FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data());

	globalToolManager->GetDataManager()->WriteLog("%s  KickUserPublish %s", __FUNCTION__, mStrId.toStdString().c_str());
   emit sig_KickUserPublish(mStrId);
   //startUpperTime(5000);
   globalToolManager->GetDataManager()->WriteLog("%s mpUpperTime->start 5000", __FUNCTION__);
}

void COnlineItemWdg::slot_upperTimeOut()
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter mStrId %s Name %s",
      __FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data());

	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (NULL!=mpUpperTime && mpUpperTime->isActive())
	{
		mpUpperTime->stop();
		//if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {//公开课
		//	//emit sig_UserSpeaking(false);
		//	globalToolManager->GetDataManager()->WriteLog("%s isHasSpeaked:%s", __FUNCTION__, "1");
		//}
      globalToolManager->GetDataManager()->WriteLog("%s mpUpperTime->stop", __FUNCTION__);
	}

	//ui.btnUpper->setEnabled(true);
 //  globalToolManager->GetDataManager()->WriteLog("%s ui.btnUpper->setEnabled(true) mStrId %s Name %s 736",
 //     __FUNCTION__, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data());

    switch (miRaiseHandsState)
    {
       case eRaiseHandsState_normal: {//正常状态
           globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_InviteTimeOut));
           //mbIsInviting = false;
		   break;
       }
	   //case eRaiseHandsState_RaiseHands: {//举手中
		  // mbIsInviting = false;
		  // break;
	   //}
	   //case eRaiseHandsState_Speaking: {//连麦中
		  // mbIsInviting = false;
		  // break;
	   //}
    //   default: {
		  // mbIsInviting = false;
		  // break;
	   //}
    }
    //mbIsInviting = false;
    SetUpperStates(eRaiseHandsState_normal);
	if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
		//if (mbIsInviting) {
		//	emit sig_UserSpeaking(true);
		//	globalToolManager->GetDataManager()->WriteLog("%s mbIsInviting:%s isHasSpeaked:%s", __FUNCTION__, "1", "1");
		//}
		//else{
		//   emit sig_UserSpeaking(false);
		//   globalToolManager->GetDataManager()->WriteLog("%s mbIsInviting:%s isHasSpeaked:%s", __FUNCTION__, "0", "0");
		//}
	}
    globalToolManager->GetDataManager()->WriteLog("%s  miRaiseHandsState:%d  mStrId %s Name %s ", __FUNCTION__, 
       miRaiseHandsState, mStrId.toStdString().c_str(), ui.labName->text().toLocal8Bit().data());
    
}

