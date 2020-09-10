#include "stdafx.h"

#include "CtrlDoublePubStuWdg.h"
#include "TipsWdg.h"

extern ToolManager *globalToolManager;
using namespace std;


CtrlDoublePubStuWdg::CtrlDoublePubStuWdg(QWidget *parent) :
   CWidget(parent)  {
	ui.setupUi(this);
   setFocusPolicy(Qt::NoFocus);

	ui.btnPre->hide();
	ui.btnNext->hide();
}

CtrlDoublePubStuWdg::~CtrlDoublePubStuWdg() {
}

void CtrlDoublePubStuWdg::SetType(const DoublePubStuChoiceUI::eChoiceType& Type)
{
   mType = Type;
}

void CtrlDoublePubStuWdg::initItem(const DoublePubStuChoiceUI::eChoiceType& Type)
{
   //mType = Type;
   disconnect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
   ui.cmbUserList->clear();
	repaint();
}

void CtrlDoublePubStuWdg::setCurrentDoubleStu(const QString& strId)
{
   mCurrDoubleStu = strId;
   int iCount = ui.cmbUserList->count();
   for (int i = 0; i < iCount; i++) {
      QString strCurId = ui.cmbUserList->itemData(i).toString().trimmed();
      if (strCurId.compare(strId) == 0) {
         ui.cmbUserList->setCurrentIndex(i);
         break;
      }
   }
}

void CtrlDoublePubStuWdg::on_btnClose_clicked(bool checked /*= false*/)
{
   //emit sig_close();
	stopPush();
}

bool CtrlDoublePubStuWdg::isExitStu(const QString& strId)
{
   bool bRef = false;

   int iCount = ui.cmbUserList->count();
   for (int i = 0; i < iCount; i++) {
      QString strCurId = ui.cmbUserList->itemData(i).toString().trimmed();
      if (strCurId.compare(strId) == 0) {
         bRef = true;
         break;
      }
   }
   return bRef;
}

void CtrlDoublePubStuWdg::AddUser(const CDoubleVrtcUsers& vrtcUser)
{
   if (mType == DoublePubStuChoiceUI::eChoiceType::eChoiceType_Deskop ||DoublePubStuChoiceItem::eEnableVrtc_enable  == vrtcUser.miNoDoubleVrtc)
   {
      if (!isExitStu(vrtcUser.mstrId.trimmed()))
      {
         int index = ui.cmbUserList->count();
         ui.cmbUserList->addItem(vrtcUser.mstrNickName, vrtcUser.mstrId);

         if (index <= 0) {
            ui.btnNext->setEnabled(false);
            ui.btnPre->setEnabled(false);
         }
         else {
            ui.btnNext->setEnabled(true);
            ui.btnPre->setEnabled(true);
         }

         setCurComboBoxById(mCurrDoubleStu);
      }
      
   }
   else
   {
      globalToolManager->GetDataManager()->WriteLog("%s %s %s enalbeVRTC:%d", __FUNCTION__, vrtcUser.mstrId.toStdString().c_str(), \
         vrtcUser.mstrNickName.toLatin1().data(), DoublePubStuChoiceItem::eEnableVrtc_enable == vrtcUser.miNoDoubleVrtc);
   }
}

void CtrlDoublePubStuWdg::setCurComboBoxById(const QString& strId)
{
   QString strCurId = ui.cmbUserList->currentData().toString().trimmed();
   mCurrDoubleStu = strId;
   if (0!=strCurId.compare(strId))
   {
      disconnect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
      setEnabledSig(false);
      
      int iCount = ui.cmbUserList->count();
      for (int i = 0; i < iCount; i++) {
         QString strCurId = ui.cmbUserList->itemData(i).toString().trimmed();
         if (strCurId.compare(strId) == 0) {
            ui.cmbUserList->setCurrentIndex(i);
            break;
         }
      }

      setEnabledSig(true);
      connect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
   }
}

QString CtrlDoublePubStuWdg::CurDoubleStuId()
{
   return mCurrDoubleStu;
}

bool CtrlDoublePubStuWdg::isInitDoublePushComb()
{
   bool bRef = false;

   if (!mCurrDoubleStu.trimmed().isEmpty() && isExitStu(mCurrDoubleStu.trimmed())) {
      bRef = true;
   }
   return bRef;
}

void CtrlDoublePubStuWdg::slot_DeleteItem(const QString& strID ) {
   disconnect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
   int iCount = ui.cmbUserList->count();
   for (int i = 0; i < iCount; i++) {
      QString strCurId = ui.cmbUserList->itemData(i).toString().trimmed();

      if ( strCurId.compare(strID) == 0) {
         ui.cmbUserList->removeItem(i);

         QString strTarget = mCurrDoubleStu;
         if (mCurrDoubleStu.isEmpty()) {
            QString strId = QString::fromStdString(globalToolManager->GetClassSDK()->GetAuxiliaryId());
            if (!strId.isEmpty()) {
               strTarget = strId;
            }
         }

         if (strID.compare(strTarget.trimmed())==0) {
			 stopPush(false);
         }
         break;
      }
   }
   connect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
}

void CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged(int index)
{
   //slot_StopPush();
	mbChange = true;
	mCurrentStrId = ui.cmbUserList->currentData().toString().trimmed();
   stopPush(); 
}

void CtrlDoublePubStuWdg::ChangeUser() {
	if (!mCurrentStrId.isEmpty() && mbChange) {
		//slot_StartStuDoublePush(mCurrentStrId, mType);
      emit sig_inviteDoubleVrtc(mCurrentStrId, mType);
		mbChange = false;
	}	
}

QString CtrlDoublePubStuWdg::GetCurrentStrId() {
	return mCurrentStrId;
}

const DoublePubStuChoiceUI::eChoiceType&  CtrlDoublePubStuWdg::GetCurrentType() {
	return mType;

}

bool CtrlDoublePubStuWdg::isChangeUser() 
{
   return mbChange;
}

void CtrlDoublePubStuWdg::show()
{
   connect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
   CWidget::show();
}

void CtrlDoublePubStuWdg::setEnabledSig(const bool& bEnabled)
{
   if (bEnabled) {
      connect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
   }
   else {
      disconnect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);
   }
}

void CtrlDoublePubStuWdg::hide()
{
   disconnect(ui.cmbUserList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CtrlDoublePubStuWdg::slot_cmbUserList_currentIndexChanged);

   CWidget::hide();
}


void CtrlDoublePubStuWdg::slot_StartStuDoublePush(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType)
{
   bool bIsPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);
   if (bIsPushing)//正在本地双推
   {
      emit sig_CameraDoublePush(false);
   }
   else if (globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture) //学员在双推
      || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop))
   {
	   stopPush();
   }
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (strId.compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0) {
      mType = DoublePubStuChoiceUI::eChoiceType::eChoiceType_Invalid;
   }
   else {
      mType = eType;
   }
   
   setCurComboBoxById(strId);
   if (DoublePubStuChoiceUI::eChoiceType_Camera == eType) {
      globalToolManager->GetClassSDK()->inviteDoubleVrtc(mCurrDoubleStu.toStdString());
   }
   else {
      globalToolManager->GetClassSDK()->inviteDesktopVrtc(mCurrDoubleStu.toStdString());
   }
}

void CtrlDoublePubStuWdg::on_btnPre_clicked(bool checked /*= false*/)
{
   int iCurIndex = ui.cmbUserList->currentIndex();
   if (iCurIndex >0)
   {
      ui.cmbUserList->setCurrentIndex(iCurIndex-1);
   }
   else
   {
      ui.cmbUserList->setCurrentIndex(ui.cmbUserList->count() - 1);
   }
}

void CtrlDoublePubStuWdg::on_btnNext_clicked(bool checked /*= false*/)
{
   int iCurIndex = ui.cmbUserList->currentIndex();
   if (iCurIndex < ui.cmbUserList->count() - 1)
   {
      ui.cmbUserList->setCurrentIndex(iCurIndex + 1);
   }
   else
   {
      ui.cmbUserList->setCurrentIndex(0);
   }
}

void CtrlDoublePubStuWdg::stopPush(const bool bAsk/* = true*/)
{
	if (DoublePubStuChoiceUI::eChoiceType_Camera == mType) {
		if (mbChange) {
			slot_StopPush();
			return;
		}
      int accept = QDialog::Accepted;
      if (bAsk) {
         TipsWdg* tips = new TipsWdg(this);
         if (tips) {
            CSTDMapParam param;
            param[TipsMsg] = QStringLiteral("是否确认结束双推?");
            param[ShowCancelButton] = PARAM_TRUE;
            tips->Notice(param);
         }
         accept = tips->exec();
      }
      else {
         
      }

		if (accept == QDialog::Accepted) {
			slot_StopPush();
			return;
		}
	}
	else if (DoublePubStuChoiceUI::eChoiceType_Deskop == mType) {
		if (mbChange) {
			slot_StopDesktopSharing();
			return;
		}
      int accept = QDialog::Accepted;
      if (bAsk) {
         TipsWdg* tips = new TipsWdg(this);
         if (tips) {
            CSTDMapParam param;
            param[TipsMsg] = QStringLiteral("是否确认结束桌面共享?");
            param[ShowCancelButton] = PARAM_TRUE;
            tips->Notice(param);
         }
         accept = tips->exec();
      }
		if (accept == QDialog::Accepted) {
			slot_StopDesktopSharing();
		}
	}
}

void CtrlDoublePubStuWdg::slot_StopPush()
{
   if (mCurrDoubleStu.isEmpty()) {
      QString strId = QString::fromStdString(globalToolManager->GetClassSDK()->GetAuxiliaryId());
      if (strId.isEmpty()) {
         {
            strId = QString::fromStdString(globalToolManager->GetClassSDK()->GetLocalAuxiliaryId());
         }

         if (!strId.isEmpty()) {
            globalToolManager->GetClassSDK()->stopDoubleVrtc(strId.toStdString());
            emit sig_StopDoublePush();
         }
         else {
            globalToolManager->GetDataManager()->WriteLog("%s slot_StopPush Failed  AuxiliaryId Not Find");
         }

      }
   }
   else {
		globalToolManager->GetClassSDK()->stopDoubleVrtc(mCurrDoubleStu.toStdString());
		emit sig_StopDoublePush();
   }
	mCurrDoubleStu = "";
}

void CtrlDoublePubStuWdg::slot_StopDesktopSharing()
{
	if (mCurrDoubleStu.isEmpty()) {
		QString strId = QString::fromStdString(globalToolManager->GetClassSDK()->GetAuxiliaryId());
		if (!strId.isEmpty()) {
			globalToolManager->GetClassSDK()->stopDesktopSharing(strId.toStdString());
			emit sig_StopDoublePush();
		}
	}
	else {
		globalToolManager->GetClassSDK()->stopDesktopSharing(mCurrDoubleStu.toStdString());
		emit sig_StopDoublePush();
	}
   emit sig_StopDoublePush();
	mCurrDoubleStu = "";
}
