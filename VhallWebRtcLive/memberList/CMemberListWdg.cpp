#include "CMemberListWdg.h"
//#include "cbasedlg.h"
#include <QtAlgorithms>
#include <QPainter> 
#include <QTimer>
#include <QJsonDocument> 
#include <QValidator> 
#include <QDebug>

#include "pathmanager.h"
#include "DebugTrace.h"
#include "../Unility/Unility.h"
#include "WebRtcLiveWdg.h"
#include "AnswerWebView.h"

extern ToolManager *globalToolManager;

#define SELECT_STYLE    "QPushButton{	\
color: rgb(255, 255, 255);\
   font: 12px \"微软雅黑\";\
	   border-radius:4px;\
	   background-color: rgb(82, 204, 144);\
}\
QPushButton:hover{\
	background-color: rgb(52, 184, 119);\
}"
#define DEFAULT_STYLE   "QPushButton{	\
color: rgb(255, 255, 255);\
   font: 12px \"微软雅黑\";\
	   border-radius:4px;\
	   background-color: rgb(102, 102, 102);\
}\
QPushButton:hover{\
	background-color:  rgb(64, 64, 64);}"

CMemberListWdg::CMemberListWdg(QWidget *parent)
	: CWidget(parent)
{
	ui.setupUi(this);

	ui.tbtnAllowRaiseHands->setCheckable(true);
	ui.tbtnAllowRaiseHands->setCheckable(true);

	ui.onLineListWdt->setSortingEnabled(true);  //设置为可排序状态
	ui.onLineListWdt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.onLineListWdt->setFocusPolicy(Qt::NoFocus);

	ui.raiseHandsListWdt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.raiseHandsListWdt->setFocusPolicy(Qt::NoFocus);

	ui.restrictedlistWdt->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.restrictedlistWdt->setFocusPolicy(Qt::NoFocus);

	setWindowTitle(QStringLiteral("成员列表"));

	ui.btnRefresh->setEnabled(true);

	ui.labAllowRaiseHands->hide();
	ui.tbtnAllowRaiseHands->hide();

	//最多可输入12个字符（限英文、数字或下划线
	QRegExp regxRoomID("^[0-9a-zA-Z_]{1,}$");
	QRegExpValidator validatorRoomId(regxRoomID, ui.ledtLookUp);
	ui.ledtLookUp->setValidator(&validatorRoomId);

	ui.ledtLookUp->setMaxLength(MAX_NICKNAME_LEN);
	ui.grpBLookUp->hide();
	on_btnRefresh_clicked();
	on_btnOnLine_clicked();
   mDateTimeInvitationSpeech = QDateTime::currentDateTime().addDays(-1);
}

CMemberListWdg::~CMemberListWdg()
{
	if (NULL != mpRefreshTime)
	{
		slotEnableRefresh();
		delete mpRefreshTime;
		mpRefreshTime = NULL;
	}

	clearData();
}

bool CMemberListWdg::addOnlineMember(const CMember &info)
{
	globalToolManager->GetDataManager()->WriteLog("%s Enter[online] ID=%s name=%s RolType:%d ", __FUNCTION__, 
		info.nick_name.toLocal8Bit().data(), info.user_id.toStdString().c_str(), info.role_type);

	bool bAdd = false;
	COnlineItemWdg* temp = getOnlineItemWdt(info.user_id);
	if (NULL == temp)//没有当前用户
	{
		QListWidgetItem *itemWidget = new QListWidgetItem(ui.onLineListWdt);
		if (NULL != itemWidget)
		{
			itemWidget->setSelected(false);
			COnlineItemWdg* pWdg = NULL;
			pWdg = new COnlineItemWdg(ui.onLineListWdt);
			connect(pWdg, SIGNAL(sig_UserSpeaking(bool)), this, SLOT(slot_UserSpeaking(bool)));

         connect(pWdg, &COnlineItemWdg::sig_KickUserPublish, this, &CMemberListWdg::sig_KickUserPublish);
         connect(pWdg, &COnlineItemWdg::sign_AuditPublish, this, &CMemberListWdg::sign_AuditPublish);
         bool bd =  connect(pWdg, &COnlineItemWdg::sign_AskforPublish, this, &CMemberListWdg::sign_AskforPublish);


			globalToolManager->GetDataManager()->WriteLog("%s [online] ID=%s RolType:%d ", __FUNCTION__, info.user_id.toStdString().c_str(),info.role_type);

			itemWidget->setSizeHint(QSize(0, pWdg->height()));
			
			pWdg->SetUserInfo(info);
			
			QString strId = pWdg->GetId();
			mOnlineItemMap.insert(strId,pWdg);
			
			QString strContain = ui.ledtLookUp->text().trimmed();
			if (strId.contains(strContain))
			{
				pWdg->show();
			}
			else
			{
				pWdg->hide();
			}

         COnlineItemWdg* Item = getForbidItemWdt(strId);
			if (NULL != Item){
				pWdg->SetForbiddenState(Item->GetForbiddenState());
				pWdg->SetKickOutState(Item->GetKickOutState());
            Item->updateInfo(info);
			}
         //else{
         //   pWdg->SetKickOutState(JOIN_ROLE_Student == info.role_type);
         //}
			//	pWdg->SetLimitedState(COnlineItemWdg::eLimitedState_normal);
         COnlineItemWdg* pRaiseHandsWdg = getRaiseHandsItemWdt(strId);
         
			if (pRaiseHandsWdg)
			{
				pWdg->SetUpperStates(pRaiseHandsWdg->GetUpperStates());
            pWdg->updateInfo(info);
			}
			else
				pWdg->SetUpperStates(COnlineItemWdg::eRaiseHandsState_normal);


			int iWeight = pWdg->CalcWeight();
			//itemWidget->setTextColor(QColor(255,255,255,0));
			itemWidget->setText(QString::number(iWeight));

			ui.onLineListWdt->addItem(itemWidget);
			ui.onLineListWdt->setItemWidget(itemWidget, pWdg);
			pWdg->SetItem(itemWidget);
			bAdd = true;
		}
		else {
		}
	}
	else//存在的话仅仅更新
	{
		temp->SetUserInfo(info);
		//add_log("xk", " mOnlineItemList upd： name [%s] id [%s]", temp->GetName().toLocal8Bit().data(), temp->GetId().toLocal8Bit().data());
	}
	globalToolManager->GetDataManager()->WriteLog("%s Leave ", __FUNCTION__);

	if (info.is_joinmic) {
		SetInviteList(info.user_id.toStdString());
	}

	return bAdd;
}

//void CMemberListWdg::slot_UserSpeaking(bool speak) {
//	mbIsUserSpeaking = speak;
//}

int CMemberListWdg::GetOnLineMemberSize() {
    return mOnlineItemMap.size();
}

void CMemberListWdg::ClearListRaiseHans()
{
   mListRaiseHans.clear();
}

bool CMemberListWdg::StartInvitationSpeech()
{
   bool bRef = false;
   QDateTime curDateTime = QDateTime::currentDateTime();
   
   globalToolManager->GetDataManager()->WriteLog("%s curDateTime %s mDateTimeInvitationSpeech %s", __FUNCTION__, 
      curDateTime.toString("yy-MM-dd hh:mm:ss").toStdString().c_str(),
      mDateTimeInvitationSpeech.toString("yy-MM-dd hh:mm:ss").toStdString().c_str()
   );

   int iNum = mDateTimeInvitationSpeech.msecsTo(curDateTime);
   if (iNum > 5000)
   {
      bRef = true;
      mDateTimeInvitationSpeech = curDateTime;
   }
   return bRef;
}

bool CMemberListWdg::IsUserSpeaking(QString strId)
{
   bool bRef = false;
   COnlineItemWdg* temp = getOnlineItemWdt(strId);
   if (temp) {
      bRef = (COnlineItemWdg::eRaiseHandsState_Speaking == temp->GetRaiseHandsState() );
   }

   return bRef;
}

bool CMemberListWdg::GetMember(const QString& strId,CMember& info)
{
	bool bRef = false;
	COnlineItemWdg *itemTemp = getOnlineItemWdt(strId);//在线用户列表
	if (NULL == itemTemp)
	{
		itemTemp = getForbidItemWdt(strId);//禁言用户列表
	}

	if (NULL == itemTemp)
	{
      itemTemp = getRaiseHandsItemWdt(strId);
	}

	if (NULL != itemTemp)
	{
		//info.status;            // 在线状态，默认为0不在线，1为在线
		info.role_type = itemTemp->GetRole();
		info.no_mic = itemTemp->GetNoMic();       //是否具有上麦能力  0 有能力。 1 无能力
		info.user_id = itemTemp->GetId();  //用户ID
		info.nick_name = itemTemp->GetName();
		info.status = itemTemp->GetOnlineStatusStates();
		info.is_joinmic = itemTemp->GetRaiseHandsState();
		bRef = true;
	}
	return bRef;
}

bool CMemberListWdg::addRaiseHandsMember(QString strUserId, const int& raiseHandsState)
{
	bool bRef = false;

	if (COnlineItemWdg::eRaiseHandsState_normal == raiseHandsState)//下麦
	{
		COnlineItemWdg* temp = getOnlineItemWdt(strUserId);
		if (temp) temp->SetUpperStates(COnlineItemWdg::eRaiseHandsState_normal);

		temp = getForbidItemWdt(strUserId);
		if (temp) temp->SetUpperStates(COnlineItemWdg::eRaiseHandsState_normal);

		RemoveInfo(ui.raiseHandsListWdt, strUserId);
	}
	else
	{
		CMember info;
		bRef = GetMember(strUserId, info);
		info.is_joinmic = raiseHandsState;
		if (bRef)
		{
			addRaiseHandsMember(info/*, raiseHandsState*/);
		}
	}
	return bRef;
}

void CMemberListWdg::addForbidChatMember(const QString& user_id, const int& forbid)
{
	if (user_id.trimmed().isEmpty())//禁言全部用户
	{
		QMap< QString, COnlineItemWdg* >::iterator it = mOnlineItemMap.begin();
		COnlineItemWdg* item = NULL;
		for (; mOnlineItemMap.end()!=it; it++)
		{
			item = it.value();
			if (NULL != item && JOIN_ROLE_Teacher != item->GetRole())
			{
				CMember info;
				//info.status;            // 在线状态，默认为0不在线，1为在线
				info.role_type = item->GetRole();
				info.no_mic = item->GetNoMic();           //是否具有上麦能力  0 有能力。 1 无能力
				info.user_id = item->GetId();  //用户ID
				info.nick_name = item->GetName();
				info.status = item->GetOnlineStatusStates();
				addForbidChatMember(info, eForbidden_Forbidden == forbid);
			}
		}
	}
	else//禁言指定用户
	{
		CMember info;
		bool bRef = GetMember(user_id, info);
		if (bRef)
		{
			addForbidChatMember(info, eForbidden_Forbidden == forbid);
		}
	}
}

void CMemberListWdg::updatePublishAbility(const QString& strId, const int& iStatus)
{
	COnlineItemWdg* tempItem = NULL;
	tempItem = getOnlineItemWdt(strId);
	if (NULL != tempItem)
	{
		tempItem->SetNoMic(iStatus);
	}

	tempItem = getForbidItemWdt(strId);
	if (NULL != tempItem)
	{
		tempItem->SetNoMic(iStatus);
	}
   tempItem = getRaiseHandsItemWdt(strId);
	if (NULL != tempItem)
	{
		tempItem->SetNoMic(iStatus);
	}
}

bool CMemberListWdg::addForbidChatMember(const CMember &info, const bool& bForbid /*= true*/, const bool& bForAll/*= true*/)
{
	bool badd = false;
	
	//CMember infoTemp;
	int upperState = COnlineItemWdg::eRaiseHandsState_normal;//获取举手状态
	
   COnlineItemWdg *itemTemp = getOnlineItemWdt(info.user_id);
	if (NULL != itemTemp)
	{
		//itemTemp->SetLimitedState(COnlineItemWdg::eLimitedState_Forbidden);
		itemTemp->SetForbiddenState(bForbid);
		itemTemp->updateInfo(info);
		upperState = itemTemp->GetUpperStates();
	}

   itemTemp = getRaiseHandsItemWdt(info.user_id);

	if (NULL != itemTemp)
	{
		itemTemp->SetForbiddenState(bForbid);
		itemTemp->updateInfo(info);
		upperState = itemTemp->GetUpperStates();
	}
	
	itemTemp = getForbidItemWdt(info.user_id);
	if (NULL==itemTemp) //不存在
	{
		if (bForbid)//禁言
		{
			QListWidgetItem *item = new QListWidgetItem(ui.restrictedlistWdt);
			if (NULL != item)
			{
				ui.restrictedlistWdt->addItem(item);
				COnlineItemWdg *itemWidget = new COnlineItemWdg(ui.restrictedlistWdt);
				if (NULL != itemWidget)
				{
               connect(itemWidget, SIGNAL(sig_UserSpeaking(bool)), this, SLOT(slot_UserSpeaking(bool)));
               connect(itemWidget, &COnlineItemWdg::sig_KickUserPublish, this, &CMemberListWdg::sig_KickUserPublish);
               connect(itemWidget, &COnlineItemWdg::sign_AuditPublish, this, &CMemberListWdg::sign_AuditPublish);
               connect(itemWidget, &COnlineItemWdg::sign_AskforPublish, this, &CMemberListWdg::sign_AskforPublish);

					badd = true;
					//itemWidget->SetAlive(mpAlive);
					itemWidget->SetUserInfo(info);
					item->setSizeHint(QSize(0, itemWidget->height()));
					ui.restrictedlistWdt->setItemWidget(item, itemWidget);
					//mForbidItemMap[info.user_id] = itemWidget;
               mForbidItemMap.insert(info.user_id, itemWidget);
					itemWidget->SetForbiddenState(true);
					itemWidget->SetUpperStates(upperState);
					//connect(itemWidget, &COnlineItemWdg::sig_btnUpper_clicked, this, &CMemberListWdg::signal_Upper);
					//connect(itemWidget, &COnlineItemWdg::sig_OperationUser, this, &CMemberListWdg::sig_OperationUser);
				}
			}
		}
		else//取消禁言
		{	
		}
	}
	else//存在
	{
		if (bForbid)//禁言
		{
			itemTemp->SetUserInfo(info);
			itemTemp->SetUpperStates(upperState);
		}
		else//取消禁言
		{
			RemoveInfo(ui.restrictedlistWdt, info.user_id);
			mForbidItemMap.remove(info.user_id);
		}
	}
	return badd;
}

void CMemberListWdg::SetInviteList(const std::string& invite_id)
{
	if(mInvitelist.size()==0)
	{
		mInvitelist.push_back(invite_id);
		return;
	}
	std::list<std::string>::iterator iter = mInvitelist.begin();
	bool is_find = false;
	while (iter != mInvitelist.end())
	{
		std::string user = (*iter);
		if (invite_id == user)
		{
			is_find = true;
			break;
		}
		iter++;
	}
	if (is_find)
	{
		mInvitelist.remove(invite_id);
	}
	else
	{
		mInvitelist.push_back(invite_id);
	}
}

bool CMemberListWdg::addRaiseHandsMember(const CMember &info/*, const int& raiseHandsState,const bool& bForAll/*= true*/)
{
	bool badd = false;
	if (info.is_joinmic == COnlineItemWdg::eRaiseHandsState_normal)
	{
		RemoveInfo(ui.raiseHandsListWdt, info.user_id);
	}
	else{
      COnlineItemWdg* ptem = getRaiseHandsItemWdt(info.user_id);

		if (NULL!=ptem /*&& ptem->GetUpperStates() < COnlineItemWdg::eRaiseHandsState_Speaking*/)
		{
			return false;
		}

		bool bForbidden = false;//禁言
		bool bKickOut = false; //踢出

		COnlineItemWdg *itemTemp = getOnlineItemWdt(info.user_id);//在线用户列表
		if (NULL != itemTemp)
		{
			
			itemTemp->SetUpperStates(info.is_joinmic);
			itemTemp->updateInfo(info);
			bForbidden = itemTemp->GetForbiddenState();//禁言
			bKickOut = itemTemp->GetKickOutState(); //踢出
		}

		itemTemp = getForbidItemWdt(info.user_id);//禁言用户列表
		if (NULL != itemTemp)
		{
			itemTemp->SetUpperStates(info.is_joinmic);
			itemTemp->updateInfo(info);
			bForbidden = itemTemp->GetForbiddenState();//禁言
			bKickOut = itemTemp->GetKickOutState(); //踢出
		}

		//CMember infoTemp;
		QListWidgetItem *item = new QListWidgetItem(ui.raiseHandsListWdt);
		if (NULL != item)
		{
			ui.raiseHandsListWdt->addItem(item);
			COnlineItemWdg *itemWidget = new COnlineItemWdg(ui.raiseHandsListWdt);
			if (NULL != itemWidget)
			{

            connect(itemWidget, SIGNAL(sig_UserSpeaking(bool)), this, SLOT(slot_UserSpeaking(bool)));
            connect(itemWidget, &COnlineItemWdg::sig_KickUserPublish, this, &CMemberListWdg::sig_KickUserPublish);
            connect(itemWidget, &COnlineItemWdg::sign_AuditPublish, this, &CMemberListWdg::sign_AuditPublish);
            connect(itemWidget, &COnlineItemWdg::sign_AskforPublish, this, &CMemberListWdg::sign_AskforPublish);
				badd = true;
				//itemWidget->SetAlive(mpAlive);
				itemWidget->SetUserInfo(info);
				item->setSizeHint(QSize(0, itemWidget->height()));
				ui.raiseHandsListWdt->setItemWidget(item, itemWidget);
            globalToolManager->GetDataManager()->WriteLog(" %s ADD %s", __FUNCTION__, info.user_id.toStdString().c_str());

				mRaiseHandsItemMap[info.user_id] = itemWidget;
            mRaiseHandsItemMap.insert(info.user_id, itemWidget);
				itemWidget->SetForbiddenState(bForbidden);
				itemWidget->SetKickOutState(bKickOut);

				itemWidget->SetUpperStates(info.is_joinmic);

			}
		}
	}


	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	bool is_HandsUp = false;
	std::list<std::string>::iterator iter = mInvitelist.begin();
	while (iter != mInvitelist.end())
	{
		std::string user = (*iter);
		if (user == info.user_id.toStdString())
		{
			is_HandsUp = true;
			break;
		}
		iter++;
	}
	if (!mListRaiseHans.contains(info.user_id) && info.user_id.compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) != 0 && !is_HandsUp)
	{
		if (!ui.raiseHandsListWdt->isVisible())
		{
			ui.btnRaiseHands->SetHandsUpState(eHandsUpState_existence);
		}
		emit sig_HandsUpState(eHandsUpState_existence);
	}

	return badd;
}

void CMemberListWdg::addKickOutMember(const QString& user_id, const int& state)
{
	CMember info;
	bool bRef = GetMember(user_id, info);
	if (bRef)
	{
        addKickOutMember(info, 1 == state);
        RemoveOnLineItemWdt(user_id);
	}
}

bool CMemberListWdg::addKickOutMember(const CMember &info, const bool& bextend /*= true*/, const bool& bForAll/*= true*/)
{
	bool bAdd = false;
	if (bextend)//踢出
	{
		COnlineItemWdg *itemTemp = getOnlineItemWdt(info.user_id);
		bool bForbidden = false;
		if (NULL != itemTemp)
		{
			///RemoveOnLineItemWdt( info.user_id);
			itemTemp->updateInfo(info);
			itemTemp->SetKickOutState(true);
			bForbidden = itemTemp->GetForbiddenState();
		}


		itemTemp = getForbidItemWdt(info.user_id);
		if (NULL != itemTemp)
		{
			itemTemp->updateInfo(info);
			itemTemp->SetKickOutState(true);
			return true;
		}

		CMember infoTemp;
		infoTemp.nick_name = info.nick_name;
		infoTemp.role_type = info.role_type;
		infoTemp.user_id = info.user_id;
		//infoTemp.SetState(COnlineItemWdg::eState_KickOut);

		QListWidgetItem *item = new QListWidgetItem(ui.restrictedlistWdt);
		if (NULL != item)
		{
			ui.restrictedlistWdt->addItem(item);
			COnlineItemWdg *itemWidget = new COnlineItemWdg(ui.restrictedlistWdt);
			if (NULL != itemWidget)
			{
				bAdd = true;
                //mForbidItemMap[info.user_id] = itemWidget;
           mForbidItemMap.insert(info.user_id, itemWidget);
				//itemWidget->SetAlive(mpAlive);
				itemWidget->SetUserInfo(infoTemp);
				item->setSizeHint(QSize(0, itemWidget->height()));
				ui.restrictedlistWdt->setItemWidget(item, itemWidget);
				itemWidget->SetKickOutState(true);
				itemWidget->SetForbiddenState(bForbidden);
				//connect(itemWidget, &COnlineItemWdg::sig_OperationUser, this, &CMemberListWdg::sig_OperationUser);
			}
		}
	}
	else//取消踢出
	{
        QMap< QString, COnlineItemWdg* >::iterator iter = mForbidItemMap.find(info.user_id);
        if (iter != mForbidItemMap.end()) {
            mForbidItemMap.erase(iter);
        }
		RemoveInfo(ui.restrictedlistWdt, info.user_id);
	}
	return bAdd;
}

void CMemberListWdg::setTotalMemberCount(const int& iTotal)
{
	miCount = iTotal >= 0 ? iTotal : 0;
	ui.labCount->setText(QStringLiteral("%1人在线").arg(miCount));
}

void CMemberListWdg::initUpperStates()
{
	QMap< QString, COnlineItemWdg* >::iterator ite = mOnlineItemMap.begin();
	while (ite!= mOnlineItemMap.end())
	{
		COnlineItemWdg* pTemp = ite.value();
		if (pTemp) pTemp->SetUpperStates(COnlineItemWdg::eRaiseHandsState_normal);
		ite++;
	} 
}

void CMemberListWdg::SetUpperStatesById(const QString& strId, const bool& bUpper)
{
	bool bRef = false;
	int iCount = ui.onLineListWdt->count();
	for (int i = iCount-1; i >= 0; i--) {
		QListWidgetItem *item = ui.onLineListWdt->item(i);
		if (NULL != item) {
			QWidget *w = ui.onLineListWdt->itemWidget(item);
			if (NULL != w) {
				COnlineItemWdg *itemWidget = dynamic_cast<COnlineItemWdg *>(w);
				if (itemWidget->isCurrentUser(strId)) {
					itemWidget->SetUpperStates(bUpper);
				}
			}
			else {
			}
		}
		else {
		}
	}
}

bool compareOnlineData(const COnlineItemWdg& pwdg1, const COnlineItemWdg& pwdg2)
{
	return pwdg1 <pwdg2;
}

COnlineItemWdg* CMemberListWdg::getOnlineItemWdt(const QString& strId)
{
   COnlineItemWdg*  temp = nullptr;
   QMap< QString, COnlineItemWdg* >::iterator itTemp = mOnlineItemMap.find(strId);
   if (itTemp != mOnlineItemMap.end())
   {
      temp = itTemp.value();
   }
   
	return temp;
}

COnlineItemWdg * CMemberListWdg::getForbidItemWdt(const QString & strId)
{
   COnlineItemWdg*  temp = nullptr;
   QMap< QString, COnlineItemWdg* >::iterator itTemp = mForbidItemMap.find(strId);
   if (itTemp != mForbidItemMap.end())
   {
      temp = itTemp.value();
   }

   return temp;
}

COnlineItemWdg * CMemberListWdg::getRaiseHandsItemWdt(const QString & strId)
{
   COnlineItemWdg*  temp = nullptr;
   QMap< QString, COnlineItemWdg* >::iterator itTemp = mRaiseHandsItemMap.find(strId);
   if (itTemp != mRaiseHandsItemMap.end())
   {
      temp = itTemp.value();
   }

   return temp;
}


void CMemberListWdg::RemoveOnLineItemWdt(const QString& strId)
{
	globalToolManager->GetDataManager()->WriteLog("%s [online] ID=%s miCount %d ", \
		__FUNCTION__, strId.toStdString().c_str(), miCount);

	mOnlineItemMap.remove(strId);
	if (RemoveInfo(ui.onLineListWdt, strId))
	{
		miCount--;
		globalToolManager->GetDataManager()->WriteLog("%s [online] ID=%s miCount-- %d ", \
			__FUNCTION__, strId.toStdString().c_str(), miCount);
		UpdateMemberCount();
	}
}

void CMemberListWdg::on_btnLookUp_clicked(bool checked /*= false*/)
{
	//ui.grpBList->hide();
	ui.btnOnLine->hide();
	ui.btnRaiseHands->hide();
	ui.btnProhibitSpeaking->hide();

	ui.btnLookUp->hide();
	ui.horizontalSpacer->changeSize(0, 0);
	ui.ledtLookUp->setText("");
	ui.grpBLookUp->show();
	on_btnOnLine_clicked();
}

void CMemberListWdg::on_btnClear_clicked(bool checked /*= false*/)
{
	if (!ui.ledtLookUp->text().trimmed().isEmpty())
	{
		ui.ledtLookUp->setText("");
	}
	else
	{
		ui.grpBLookUp->hide();
		ui.horizontalSpacer->changeSize(20, 20, QSizePolicy::Expanding);

		ui.btnOnLine->show();
		ui.btnRaiseHands->show();
		ui.btnProhibitSpeaking->show();
		ui.btnLookUp->show();
	}
}

void CMemberListWdg::SetCurrentListTab(const eTabMember& tabM)
{
	switch (tabM)
	{
		case eTabMember_onLine:
		{
			ui.raiseHandsListWdt->hide();
			ui.restrictedlistWdt->hide();
			ui.onLineListWdt->showMaximized();
		}
			break;
		case eTabMember_raiseHands:
		{
			ui.onLineListWdt->hide();
			ui.restrictedlistWdt->hide();
			ui.raiseHandsListWdt->showMaximized();
		}
			break;
		case eTabMember_prohibitChat:
		{
			ui.onLineListWdt->hide();
			ui.raiseHandsListWdt->hide();
			ui.restrictedlistWdt->showMaximized();
		}
			break;
	default:
		break;
	}
}

void CMemberListWdg::setHansUpStates(const bool& bChecked)
{
	ui.tbtnAllowRaiseHands->setChecked(bChecked);
}



bool CMemberListWdg::GetHansUpShow()
{
	return ui.btnRaiseHands->isVisible();
}

void CMemberListWdg::NtfUserOffLine(const QString& strId)//下线
{
	RemoveOnLineItemWdt(strId);
	RemoveInfo(ui.raiseHandsListWdt, strId);
	mRaiseHandsItemMap.remove(strId);
   globalToolManager->GetDataManager()->WriteLog(" %s mRaiseHandsItemMap remove %s", __FUNCTION__, strId.toStdString().c_str());
	//判断是否是踢出用户
	COnlineItemWdg* pWdg = getForbidItemWdt(strId);
	if(pWdg && !pWdg->GetKickOutState() ){
		RemoveInfo(ui.restrictedlistWdt, strId);
		mForbidItemMap.remove(strId);
	}

	int iCont = mOnlineItemMap.size();
	if (0 == iCont)
		on_btnRefresh_clicked();
}

int CMemberListWdg::calcWdgHeight(const int&  iRowCount) const
{
	int iheight = 0;
	iheight = 53 * iRowCount;
	//iheight = 40 * iRowCount;
	if (iheight > 417)//440
	{
		iheight = 417;
	}
	return iheight; //上下两个边框 距离是2
}

void CMemberListWdg::dealNotSpeak(const QString& strId)
{
	//TRACE6("%s  \n", __FUNCTION__);
	COnlineItemWdg *itemTemp = getOnlineItemWdt(strId);
	if (NULL!=itemTemp)
	{
		//add_log("xk", " SetUpperStates 553: name [%s] id [%s]", itemTemp->GetName().toLocal8Bit().data(), itemTemp->GetId().toLocal8Bit().data());
		itemTemp->SetUpperStates(false);
	}
	//updateOnlineListWdg();
}

void CMemberListWdg::addSpeaker(const QString& strId)
{
	COnlineItemWdg *itemTemp = getOnlineItemWdt(strId);
	if (NULL != itemTemp)
	{
		itemTemp->SetUpperStates(true);
	}
	//updateOnlineListWdg();
	RemoveInfo(ui.raiseHandsListWdt, strId);//上麦克
}

void CMemberListWdg::on_btnOnLine_clicked(bool checked /*= false*/)
{
	//ui.stackedWidget->setCurrentIndex(eTabMember_onLine);
	SetCurrentListTab(eTabMember_onLine);
    ui.btnProhibitSpeaking->setStyleSheet(DEFAULT_STYLE);
	ui.btnRaiseHands->setStyleSheet(DEFAULT_STYLE);
	ui.btnOnLine->setStyleSheet(SELECT_STYLE);
	ui.btnLookUp->setStyleSheet(DEFAULT_STYLE);
}

void CMemberListWdg::on_btnRaiseHands_clicked(bool checked /*= false*/)
{
	ui.btnRaiseHands->SetHandsUpState(eHandsUpState_No);
	//ui.stackedWidget->setCurrentIndex(eTabMember_raiseHands);
	SetCurrentListTab(eTabMember_raiseHands);
	ui.btnProhibitSpeaking->setStyleSheet(DEFAULT_STYLE);
	ui.btnRaiseHands->setStyleSheet(SELECT_STYLE);
	ui.btnOnLine->setStyleSheet(DEFAULT_STYLE);
	ui.btnLookUp->setStyleSheet(DEFAULT_STYLE);
}

void CMemberListWdg::on_btnProhibitSpeaking_clicked(bool checked /*= false*/)
{
	//ui.stackedWidget->setCurrentIndex(eTabMember_prohibitChat);
	SetCurrentListTab(eTabMember_prohibitChat);
	ui.btnProhibitSpeaking->setStyleSheet(SELECT_STYLE);
	ui.btnRaiseHands->setStyleSheet(DEFAULT_STYLE);
	ui.btnOnLine->setStyleSheet(DEFAULT_STYLE);
	ui.btnLookUp->setStyleSheet(DEFAULT_STYLE);
}

//void CMemberListWdg::on_btnKickOut_clicked(bool checked /*= false*/)
//{
//	ui.stackedWidget->setCurrentIndex(eTabMember_kickOut);
//}

void CMemberListWdg::clearData()
{
	//add_log("ixk", "%s", __FUNCTION__);
	mOnlineItemMap.clear();
	mForbidItemMap.clear();
	mRaiseHandsItemMap.clear();
//	add_log("xk", "%s", __FUNCTION__);
	//setUserCount();
	miCount = 0;
	UpdateMemberCount();
	//clearOnLineWdg();
//	add_log("xk", "%s   miOnlineUserCont:%d", __FUNCTION__, miOnlineUserCont);
	//先清空列表中数据
	clearInfoList(ui.onLineListWdt);
	clearInfoList(ui.raiseHandsListWdt);
	clearInfoList(ui.restrictedlistWdt);
	clearInfoList(ui.restrictedlistWdt);
}

void CMemberListWdg::startRefreshUpdateTime()
{
	if (mpRefreshTime == NULL)
	{
		mpRefreshTime = new QTimer(this);
		connect(mpRefreshTime, &QTimer::timeout, this, &CMemberListWdg::slotEnableRefresh);
	}

	if (NULL != mpRefreshTime)
	{
		if (mpRefreshTime->isActive())
		{
			mpRefreshTime->stop();
		}
		ui.btnRefresh->setEnabled(false);
		mpRefreshTime->start(5000);
	}

}

void CMemberListWdg::on_btnRefresh_clicked(bool checked /*= false*/)
{
	globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
	startRefreshUpdateTime();
   mListRaiseHans = mRaiseHandsItemMap.keys();
	mReqPage = 1;
	clearData();
	globalToolManager->GetClassSDK()->AsynGetClassRoomMembers(MEMBERLISTCOUNT);//按照MEMBERLISTCOUNT人/页分页，请求第一页面数据
    on_btnOnLine_clicked(true);
}

void CMemberListWdg::slot_JsCallQtMsg(QString msg)
{                                                                                                                                     
	QJsonParseError parseJsonErr;
	QJsonDocument document = QJsonDocument::fromJson(msg.toUtf8(), &parseJsonErr);
	if (!(parseJsonErr.error == QJsonParseError::NoError))
	{
		qDebug() << tr("解析json文件错误！");
		return;
	}
	QJsonObject jsonObject = document.object();

    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	CBaseWnd* mainwnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	WebRtcLiveWdg* pwd = nullptr;
	if (mainwnd)
	{
		pwd = (WebRtcLiveWdg*)mainwnd;
	}

    if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
		//CBaseWnd* mainwnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
		if (pwd)
		{
			//WebRtcLiveWdg* pwd = (WebRtcLiveWdg*)mainwnd;
			if (pwd->IsExitAnserWebView() /*globalToolManager->GetWndManager()->IsWndExist(WND_ID_QUESTION_UI)*/) {
				//CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_QUESTION_UI);
				CBaseWnd* wnd = pwd->AnswerWebViewDlg();
				if (wnd) {
					QJsonArray json;
					json.insert(0, "cmd");
					json.insert(1, msg);

					QJsonDocument document;
					document.setArray(json);
					QByteArray byte_array = document.toJson(QJsonDocument::Compact);
					qDebug() << byte_array;
					QString base64EncodeData = byte_array.toBase64();
					QString method = QString("msgByClient('%1')").arg(base64EncodeData);
					qDebug() << method;
					CSTDMapParam param;
					param[MEDIA_FILE_CTRL_MSG] = method;
					wnd->Notice(param);
				}
			}
		}
    }

	if (0 == CPathManager::GetObjStrValue(jsonObject, "event").compare("offline"))//离线
	{
        if (jsonObject.contains("data") && jsonObject["data"].isObject()) {
            QJsonObject dataObj = jsonObject["data"].toObject();
            QString role = CPathManager::GetObjStrValue(dataObj, "role");
            if (role.toInt() == JOIN_ROLE_Teacher) {
                //师暂时离开了房间，请稍等...
                globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_RoomTeacherLeaveRoom));
            }
        }
        QString user_id = CPathManager::GetObjStrValue(jsonObject, "user_id");  //用户ID
        QString user_name = CPathManager::GetObjStrValue(jsonObject, "user_name");

        globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RecvUserOffLIne, user_id, user_name));

		NtfUserOffLine(user_id);
		//pwd->OffLine
	}
	else//上线
	{
		CMember mber;
		mber.user_id = CPathManager::GetObjStrValue(jsonObject, "user_id");  //用户ID
		mber.nick_name = CPathManager::GetObjStrValue(jsonObject, "user_name");

		QJsonObject jObject = jsonObject["data"].toObject();
		mber.status = 1;// 在线状态，默认为0不在线，1为在线       
		mber.role_type = CPathManager::GetObjStrValue(jObject, "role").toInt();
		mber.no_mic = CPathManager::GetObjStrValue(jObject, "no_mic").toInt();           //是否具有上麦能力  0 有能力。 1 无能力

        globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RecvUserOnLine , mber.user_id, mber.nick_name));
		if (addOnlineMember(mber)){
			globalToolManager->GetDataManager()->WriteLog("%s miCount %d", __FUNCTION__, miCount);
			miCount++;
			globalToolManager->GetDataManager()->WriteLog("%s miCount++ %d", __FUNCTION__, miCount);
		}
			

		if (1==CPathManager::GetObjStrValue(jObject, "is_gag").toInt())
			addForbidChatMember(mber);
	}
	//计算在线人数
	UpdateMemberCount();
}

void CMemberListWdg::on_tbtnAllowRaiseHands_clicked(bool checked /*= false*/)
{
	//TRACE6("%s  \n", __FUNCTION__);
	qDebug() << "on_tbtnAllowRaiseHands_clicked:"<< checked;
	ui.tbtnAllowRaiseHands->setChecked(!checked);
	//  checked true 允许举手  false 禁止举手

	globalToolManager->GetClassSDK()->OpenUserHandsUp(checked);
	//emit sigAllowRaiseHands(checked);
}

void CMemberListWdg::slotEnableRefresh()
{
	if (NULL!= mpRefreshTime &&mpRefreshTime->isActive())
	{
		mpRefreshTime->stop();
	}
	ui.btnRefresh->setEnabled(true);
}

bool CMemberListWdg::RemoveInfo(QListWidget *pListWdt, const QString &strId)
{
	bool bRef = false;
	int iCount = pListWdt->count();
	mRaiseHandsItemMap.remove(strId);
	for (int i = iCount -1; i>=0 ; i--) {
		QListWidgetItem *item = pListWdt->item(i);
		if (NULL!=item) {
			QWidget *w = pListWdt->itemWidget(item);
			if (NULL!=w) {
				COnlineItemWdg *itemWidget = dynamic_cast<COnlineItemWdg *>(w);
				if (itemWidget->isCurrentUser(strId)) {
					pListWdt->takeItem(i);
					itemWidget->deleteLater();
					itemWidget = NULL;	
					delete item;
					item = NULL;
					bRef = true;
					return bRef;
				}
			}
			else{
			}
		}
		else{
		}
	}
	return bRef;
}

void CMemberListWdg::clearInfoList(QListWidget *pListWdt)
{
	while(pListWdt->count()>0 ){
		QListWidgetItem *item = pListWdt->item(0);
		if (NULL!=item) {
			//TRACE6("%s  Get Item SUC \n", __FUNCTION__);
			QWidget *w = pListWdt->itemWidget(item);
			if (NULL!=w) {
				//TRACE6("%s  Get w SUC \n", __FUNCTION__);
				COnlineItemWdg *itemWidget = dynamic_cast<COnlineItemWdg *>(w);

				item = pListWdt->takeItem(0);

				//TRACE6("%s  delete  SUC \n", __FUNCTION__);
				delete itemWidget;
				itemWidget = NULL;
				
				delete item;
				item = NULL;
			}
			else
			{
				//TRACE6("%s  Get w FAIL \n", __FUNCTION__);
			}
		}
		else
		{
			//TRACE6("%s  Get Item FAIL \n", __FUNCTION__);
		}
	}
	pListWdt->clear();
}

void CMemberListWdg::on_ledtLookUp_textChanged(const QString &text)
{
	QMap< QString, COnlineItemWdg* >::iterator ite = mOnlineItemMap.begin();

	QString strContain = text.trimmed().toLower();
	if (strContain.compare("null") == 0)
	{
		strContain = "";
	}

	QListWidgetItem * item = NULL;

	for ( int i = ui.onLineListWdt->count() - 1; i>=0; i--)
	{
		if (strContain.trimmed().isEmpty())
		{
			//显示
			ui.onLineListWdt->setItemHidden(ui.onLineListWdt->item(i), false);
		}
		else
		{
			item = ui.onLineListWdt->item(i);
			QWidget *w = ui.onLineListWdt->itemWidget(item);
			if (NULL != w) {
				COnlineItemWdg *itemWidget = dynamic_cast<COnlineItemWdg *>(w);
				if (itemWidget) {

					if (itemWidget->GetId().toLower().contains(strContain))//id匹配
					{
						//显示
						ui.onLineListWdt->setItemHidden(ui.onLineListWdt->item(i), false);
					}
					else if (itemWidget->GetName().toLower().contains(strContain)) //昵称匹配
					{
						//显示
						ui.onLineListWdt->setItemHidden(ui.onLineListWdt->item(i), false);
					}
					else
					{
						//隐藏
						ui.onLineListWdt->setItemHidden(ui.onLineListWdt->item(i), true);
					}
				}
			}
		}
	}
}

int CMemberListWdg::getRolType(const QString& strId)
{
	int iRolType = JOIN_ROLE_None;
	COnlineItemWdg *itemTemp = getOnlineItemWdt(strId);
	if (NULL != itemTemp)
	{
		iRolType = itemTemp->GetRole();
	}
	return iRolType;
}

void CMemberListWdg::SetHansUpShow(const bool& bshow)
{
	if (bshow)
	{
		ui.btnRaiseHands->show();
	}
	else
	{
		ui.btnRaiseHands->hide();
	}
}

void CMemberListWdg::showAllowRaiseHands(const bool& bShow /*= true*/)
{
	if (bShow)
	{
		ui.labAllowRaiseHands->show();
		ui.tbtnAllowRaiseHands->show();
	}
	else
	{
		ui.labAllowRaiseHands->hide();
		ui.tbtnAllowRaiseHands->hide();
	}
}

//设置用户上下麦状态
void CMemberListWdg::SetUpperStates(const QString& strUserId,int iRaiseHandsState) {
   COnlineItemWdg* onlinetemp = getOnlineItemWdt(strUserId);
    if (onlinetemp){
       onlinetemp->SetUpperStates(iRaiseHandsState);
    }
    COnlineItemWdg* temp = getOnlineItemWdt(strUserId);
    temp = getForbidItemWdt(strUserId);
	if (temp) {
      temp->SetUpperStates(iRaiseHandsState);
	}

	if (COnlineItemWdg::eRaiseHandsState_normal == iRaiseHandsState)//删除
	{
		mRaiseHandsItemMap.remove(strUserId);
      globalToolManager->GetDataManager()->WriteLog(" %s mRaiseHandsItemMap remove %s", __FUNCTION__, strUserId.toStdString().c_str());
		RemoveInfo(ui.raiseHandsListWdt, strUserId);
	}
	else//更改状态
	{
      temp = getRaiseHandsItemWdt(strUserId);
      if (temp) {
         temp->SetUpperStates(iRaiseHandsState);
      }
      else if(onlinetemp){

         CMember info;
         bool bRef = GetMember(strUserId, info);
         info.is_joinmic = (int)COnlineItemWdg::eRaiseHandsState_Speaking;
         if (bRef)
         {
            addRaiseHandsMember(info/*, raiseHandsState*/);
         }
      }
	}
}

//设置在线用户禁言状态
void CMemberListWdg::SetForbiddenState(const QString& strUserId, const bool& bState) {
    COnlineItemWdg*  temp = getOnlineItemWdt(strUserId);
    if (temp) {
       temp->SetForbiddenState(bState);
    }
}


void CMemberListWdg::UpdateMemberCount()
{
	globalToolManager->GetDataManager()->WriteLog("%s miCount %d", __FUNCTION__,miCount);
	ui.labCount->setText(QStringLiteral("%1人在线").arg(miCount));
	emit sig_UpdateMemberCount(miCount);
}

CMember CMemberListWdg::GetMemberPersonalInfo(const QString& strUserId) {
    CMember info;
	info.user_id = strUserId;
	info.nick_name = strUserId;
    bool bRef = GetMember(strUserId, info);
    return  info;
}

int CMemberListWdg::GetMemberCount() {

	return  mOnlineItemMap.size()-1;
}

const std::list<std::string>& CMemberListWdg::GetInvitelist() {
	return  mInvitelist;
}

bool CMemberListWdg::GetUserSpeaking() {
	return mbIsUserSpeaking;
}