#include "RenderManager.h"
#include "../Unility/Unility.h"
#include "VhallUI_define.h"
#include "TeacherRenderWdg.h"
#include "WebRtcLiveWdg.h"
#include "FloatRender.h"

extern ToolManager *globalToolManager;

RenderManager::RenderManager(WebRtcLiveWdg* pWdg, QWidget *parent)
   : QWidget(parent)
{
   ui.setupUi(this);
   setAttribute(Qt::WA_TranslucentBackground, true);
   this->setAutoFillBackground(false);
   pRtcLiveWdg = pWdg;
   ui.mCenterManageWdg->installEventFilter(this);
}

bool RenderManager::eventFilter(QObject * obj, QEvent * event)
{
   if (event->type() == QEvent::Wheel) {//滚轮事件
      int a = 0;
   }
   else if (obj == ui.mCenterManageWdg)
   {
      if (event->type() != QEvent::MouseButtonPress && event->type() != QEvent::MouseButtonRelease)
      {
         return false;
      }
   }
   return QWidget::eventFilter(obj, event);
}

void RenderManager::wheelEvent(QWheelEvent * event)
{
   globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
   int iCurLen = ui.mCenterManageWdg->width();
   int iCenterWdg = ui.mCenterWdg->width();

   globalToolManager->GetDataManager()->WriteLog("%s iCurLen %d  iCenterWdg %d", __FUNCTION__, iCurLen , iCenterWdg);

   if (iCurLen > iCenterWdg)
   {
      int iCurX = ui.mCenterManageWdg->x();
      int ix = event->angleDelta().y();

      globalToolManager->GetDataManager()->WriteLog("%s iCurX %d  ix %d", __FUNCTION__, iCurX, ix);

      if (ix < 0) {//负数向左边移动
         if ((iCurX + iCurLen) > iCenterWdg) {
            ui.mCenterManageWdg->move(iCurX + ix, 0);
         }
         else {
            int iDisLen = iCenterWdg - iCurLen;
            ui.mCenterManageWdg->move(iDisLen, 0);
         }
      }
      else if (ix > 0) {
      
         if ((iCurX + ix) > 0) {
            ui.mCenterManageWdg->move(0, 0);
         }
         else {
            ui.mCenterManageWdg->move(iCurX + ix, 0);
         }

      }
   }
   qDebug() << event->angleDelta();
   return QWidget::wheelEvent( event);
}

RenderManager::~RenderManager()
{
}

int RenderManager::iRenderSize()
{
   int hiddenCount = 0;
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   while (iter != mRenderMap.end()) {
      if (iter->second->isHidden()) {
         hiddenCount++;
      }
      iter++;
   }
   globalToolManager->GetDataManager()->WriteLog("%s RenderSIze: %d  hiddenCount: %d", __FUNCTION__, mRenderMap.size() , hiddenCount);
   return mRenderMap.size() - hiddenCount;
}

void RenderManager::addRender(TeacherRenderWdg * pRender)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   if (pRender) {

      pRender->GetFloatType();
      InsertWidgetToListWidget(pRender, pRender->GetUserJoinID());
   }
   else {
      globalToolManager->GetDataManager()->WriteLog("%s pRender is nullptr", __FUNCTION__);
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void RenderManager::AddInitReadyPublishUser(QString userid, QString nickName, int role_type, bool hasVideo, bool hasAudio) {
   globalToolManager->GetDataManager()->WriteLog(" Enter %s ", __FUNCTION__);

   if (pRtcLiveWdg) {
      globalToolManager->GetDataManager()->WriteLog("%s uid:%s roleType:%d ", __FUNCTION__, userid.toStdString().c_str(), role_type);
      if (role_type == JOIN_ROLE_Teacher) {
         return;
      }
      //添加上麦用户有两种可能，一种是已经订阅了此流后收到上麦消息，第二种是先收到上麦消息后订阅到流。
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      globalToolManager->GetDataManager()->WriteLog("AddInitReadyPublishUser new user, user_id:%s hasVideo:%d hasAudio:%d", userid.toStdString().c_str(), hasVideo, hasAudio);
      std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(userid);
      TeacherRenderWdg* render = nullptr;
      bool bNewRender = false;

      if (pRtcLiveWdg && pRtcLiveWdg->IsExitFloatRender(userid)) {
         return;
      }
      else if (iter == mRenderMap.end()) {
         globalToolManager->GetDataManager()->WriteLog("AddPushlishUser user not exist, user_id:%s", userid.toStdString().c_str());
         render = new TeacherRenderWdg(this);
         
         InsertWidgetToListWidget(render, userid);
         connect(render, SIGNAL(sig_ClickedFloatRender(QString, QString, QString, int)), this, SLOT(slot_clickedFloatRender(QString, QString, QString, int)));
         bNewRender = true;
      } 
      else
      {
         QString strJoinId = QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
         render = iter->second;
         if (render && render->GetUserJoinID() == userid) {
            if (render->isHidden()) {
               globalToolManager->GetDataManager()->WriteLog("AddPushlishUser user is exist, Bnt Ivisible,user_id:%s", userid.toStdString().c_str());
               bNewRender = true;
               showRender(render);
               setCenterManageWidth();
               QString strId = render->GetUserJoinID();
               if (strId.compare(strJoinId) == 0) {
                  mShowSelf = true;
               }
               if (globalToolManager->GetDataManager()->IsAlreadyPublished()) { //如果在推流 
                  globalToolManager->GetDataManager()->WriteLog("%s RenderManager show() IsAlreadyPublished %d", __FUNCTION__, globalToolManager->GetDataManager()->IsAlreadyPublished());
                  show();
               }
               globalToolManager->GetDataManager()->WriteLog("%s  IsAlreadyPublished %d", __FUNCTION__, globalToolManager->GetDataManager()->IsAlreadyPublished());
            }
            else {
               globalToolManager->GetDataManager()->WriteLog("AddPushlishUser user is exist And Visible,user_id:%s", userid.toStdString().c_str());
            }
            //break;
         }
         //}
      //}

      }

      showRender(render);
      //if (globalToolManager->GetDataManager()->IsAlreadyPublished() && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type != JOIN_ROLE_Student) {  
       setCenterManageWidth();
      //}

      if (render && bNewRender) {

         render->SetStuSpacer(1);
         render->InitRenderInsertPos(InsertPos_WndManager);
         render->InitRenderCtrl(userid);
         render->SetUserJoinID(userid);

         render->ShowTrophyWdg(true);
         render->SetRoleType(role_type);
         render->SetNickName(nickName);
         render->SetCameraState(hasVideo);
         if (baseInfo.mutingAll)
         {
            render->SetMicState(!baseInfo.mutingAll);
            //render->SetStreamHasAudioFormat(!baseInfo.mutingAll);
         }
         else
         {
            render->SetMicState(hasAudio);
         }
         render->SetStreamHasAudioFormat(hasAudio);
         render->SetStreamHasVideoFormat(hasVideo);
         render->SetPageIndex(hasVideo == true ? TeacherRenderWdg_Page_Index_render : TeacherRenderWdg_Page_Index_NoCamera);
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && baseInfo.brush == userid.toInt()) {
            render->SetPenState(true);
         }

         if (pRtcLiveWdg && userid.compare(QString::fromStdString( baseInfo.mUserInfo.join_id )) == 0) {
            QString strId = QString::fromStdString(globalToolManager->GetClassSDK()->LocalStreamId());
            render->SetStreamID(strId);
         }
         else {
            std::string streamId = globalToolManager->GetClassSDK()->GetSubScribeStreamId(userid.toStdWString(), VHStreamType_AVCapture);
            render->SetStreamID(QString::fromStdString(streamId));
         }

         render->RenderBigStreamToWnd(userid);
         globalToolManager->GetDataManager()->WriteLog("%s --InsertWidgetToListWidget(render: %x  )", __FUNCTION__, render);

         bool bMute = globalToolManager->GetClassSDK()->GetMuteAllAudio();
         //render->SetSmallRenderMicState(!bMute);
      }
      else {
         globalToolManager->GetDataManager()->WriteLog(" %s  %s is exist, Do nothing!", __FUNCTION__, userid.toLocal8Bit().data());
      }
   }
   
   globalToolManager->GetDataManager()->WriteLog(" Leave %s ", __FUNCTION__);
}

void RenderManager::AddLocalPublishUser(QString userid, QString streamId, QString nickName, int role_type, bool hasVideo, bool hasAudio) {
   globalToolManager->GetDataManager()->WriteLog("%s uid:%s streamId:%s roleType:%d", __FUNCTION__, 
      userid.toStdString().c_str(), streamId.toStdString().c_str(), role_type);

   if (role_type == JOIN_ROLE_Teacher) {
      return;
   }
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(userid);
   TeacherRenderWdg* render = nullptr;
   bool bNewRender = false;

   if (iter != mRenderMap.end()) {
      globalToolManager->GetDataManager()->WriteLog("%s find rend in map", __FUNCTION__);
      render = iter->second;


      if (render && render->GetUserJoinID() == userid) {
         if (render->isHidden()) {
            globalToolManager->GetDataManager()->WriteLog("AddLocalPublishUser user is exist, Bnt Ivisible,user_id:%s", userid.toStdString().c_str());
            bNewRender = true;
            showRender(render);
            setCenterManageWidth();

			}
            globalToolManager->GetDataManager()->WriteLog("%s render->show() userid:%s", __FUNCTION__, userid.toStdString().c_str());
            mShowSelf = true;
            this->show();
                  
         }
         else {
            globalToolManager->GetDataManager()->WriteLog("AddLocalPublishUser user is exist And Visible,user_id:%s", userid.toStdString().c_str());
         }


   }
   else {
      bNewRender = true;
      globalToolManager->GetDataManager()->WriteLog("AddLocalPublishUser user is not exist,user_id:%s", userid.toStdString().c_str());
      render = new TeacherRenderWdg(this);
      InsertWidgetToListWidget(render, userid);
      mShowSelf = true;
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (userid.compare(QString::fromStdString(baseInfo.mUserInfo.join_id).trimmed()) == 0) {
         CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
         CSTDMapParam pare;
         pare[TROPHYREWARDNUM] = "";
         pWdg->GetParam(pare);
         int iTrophyrewardNum = pare[TROPHYREWARDNUM].toInt();
         render->initTrophyRewardNum(iTrophyrewardNum);
      }

      connect(render, SIGNAL(sig_ClickedFloatRender(QString, QString, QString, int)), this, SLOT(slot_clickedFloatRender(QString, QString, QString, int)));
   }

   if (render) {
      if (globalToolManager->GetDataManager()->IsAlreadyPublished() && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type != JOIN_ROLE_Student) {
         
         setCenterManageWidth();
		 globalToolManager->GetDataManager()->WriteLog("%s render->show() userid:%s", __FUNCTION__, render->GetUserJoinID().toStdString().c_str());
      }
      render->ShowTrophyWdg(true);
      render->InitRenderInsertPos(InsertPos_WndManager);
      render->InitRenderCtrl(userid);
      
      render->SetStreamHasAudioFormat(hasAudio);
      render->SetStreamHasVideoFormat(hasVideo);
      render->SetUserJoinID(userid);
      render->SetStreamID(streamId);
      render->SetRoleType(role_type);
      render->SetNickName(nickName);
      render->SetCameraState(hasVideo);
      render->SetMicState(hasAudio);
	   render->SetStuSpacer(1);
      render->SetPageIndex(hasVideo == true ? TeacherRenderWdg_Page_Index_render : TeacherRenderWdg_Page_Index_NoCamera);
      globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, render->GetRenderWnd());

      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (pRtcLiveWdg && userid.compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0) {
         QString strId =QString::fromStdString( globalToolManager->GetClassSDK()->LocalStreamId() );
         render->SetStreamID(strId);
      }
      else {
         std::string streamId = globalToolManager->GetClassSDK()->GetSubScribeStreamId(userid.toStdWString(), VHStreamType_AVCapture);
         render->SetStreamID(QString::fromStdString(streamId));
      }
      showRender(render);
   }
   else if (!bNewRender && nullptr != render)
   {
      globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, render->GetRenderWnd());
   }

}

void RenderManager::HandleSubScribStream(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s  ", __FUNCTION__);
   QEventStream *msgEvent = (QEventStream*)event;
   if (msgEvent) {
      std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(msgEvent->strUser);
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (iter != mRenderMap.end()) {
         TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
         if (render) {
            //HWND id = (HWND)(render->GetRenderWndID());
			 globalToolManager->GetDataManager()->WriteLog("%s find user %s  ", __FUNCTION__, msgEvent->strUser.toStdString().c_str());
          std::shared_ptr<vhall::VideoRenderReceiveInterface> pwd = render->GetRenderWnd();
            render->SetUserJoinID(msgEvent->strUser);
            render->SetStreamID(QString::fromStdString(msgEvent->strStream));
            render->SetStreamHasVideoFormat(msgEvent->mbHasVideo);
            render->SetCameraState(msgEvent->mbHasVideo);
            if (baseInfo.mutingAll)
            {
               render->SetMicState(!baseInfo.mutingAll);
               //render->SetStreamHasAudioFormat(!baseInfo.mutingAll);
            }
            else
            {
               render->SetMicState(msgEvent->mbHasAudio);
            }
            render->SetStreamHasAudioFormat(msgEvent->mbHasAudio);
            render->SetPageIndex(msgEvent->mbHasVideo == true ? TeacherRenderWdg_Page_Index_render : TeacherRenderWdg_Page_Index_NoCamera);
            

            showRender(render);
            setCenterManageWidth();
			   
            globalToolManager->GetDataManager()->WriteLog("%s render->show() userid:%s", __FUNCTION__, render->GetUserJoinID().toStdString().c_str());
            show();
            globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(msgEvent->strUser.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_SmallStream);

				globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(msgEvent->strUser.toStdWString(), VHStreamType_AVCapture, pwd);
            globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
            //}
         }
      }
   }
}

void RenderManager::RemovePublishUser(QString userid) {
   globalToolManager->GetDataManager()->WriteLog("%s userid:%s", __FUNCTION__, userid.toStdString().c_str());

   globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(userid.toStdWString(), VHStreamType_AVCapture, nullptr);
   int iHidden = 0;
   //if (iter != mRenderMap.end()) {
      bool bFind = false;
      QString strJoinId = QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
	  std::unordered_map<QString, TeacherRenderWdg*>::iterator Iter = mRenderMap.begin();
     // for (int i = 0; i < mRenderMap.size(); i++) {
	while (Iter != mRenderMap.end()) {
		TeacherRenderWdg* render = (TeacherRenderWdg*)Iter->second;
         if (!bFind) {         
            if (render) {

               if (render->GetUserJoinID() == userid) {
                  globalToolManager->GetDataManager()->WriteLog("%s find and delete userid:%s", __FUNCTION__, userid.toStdString().c_str());
				  render->hide();

				  globalToolManager->GetDataManager()->WriteLog("%s render->hide() userid:%s", __FUNCTION__, userid.toStdString().c_str());
                  bFind = true;

               }

               QString strId = render->GetUserJoinID();
               if (strId.compare(strJoinId) == 0) {
                  mShowSelf = false;
               }

            }
         }
  
         if (render->isHidden()) {
            iHidden++;
         }
		 Iter++;
      }
   //}

   setHidden(iHidden);
   setCenterManageWidth();
   //if (ui.listWidget->count() == 0) {
   //   hide();
   //}
}

void RenderManager::setCenterManageWidth()
{
   globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
   int iCount = iRenderSize();
   int iRenderWdgW = iCount * ITEM_WIDTH;
   int iCenterWdgW = 461;

   int iObj = 0;
   if (1 >= iCenterWdgW) {
      iObj = iRenderWdgW ; 
   }
   else if (iCenterWdgW > 0) {
      iObj = iCenterWdgW;
      
   }
   ui.mCenterManageWdg->setFixedWidth(iObj);

   globalToolManager->GetDataManager()->WriteLog("%s iCount %d, iRenderWdgW %d, iCenterWdgW  %d, iObj %d",
      __FUNCTION__, iCount, iRenderWdgW, iCenterWdgW, iObj);
}

void RenderManager::TestLayout()
{
   int i = ui.RenderLayout->count();
   globalToolManager->GetDataManager()->WriteLog("%s Render.size() %d", __FUNCTION__, ui.RenderLayout->count());
   for (int i = ui.RenderLayout->count(); i>=0; i--)
   {
      QLayoutItem *it = ui.RenderLayout->itemAt(i);
      if (it && it->widget() && it->widget()->inherits("TeacherRenderWdg"))
      {
         TeacherRenderWdg* render= qobject_cast<TeacherRenderWdg*>(it->widget());
         if (render) {
            globalToolManager->GetDataManager()->WriteLog("%s i %d  id: %s isShow: %d ", __FUNCTION__, 
               i, render->GetUserJoinID().toLocal8Bit().data(), render->isVisible());
         }
         else {
            globalToolManager->GetDataManager()->WriteLog("%s i %d  render is NULL ", __FUNCTION__, i);
         }
         
      }
      else
      {
         globalToolManager->GetDataManager()->WriteLog("%s i %d Not TeacherRenderWdg", __FUNCTION__, i);
      }
   }
}

void RenderManager::setHidden(int iHiddenCount)
{
	globalToolManager->GetDataManager()->WriteLog("%s iHiddenCount:%d  mRenderMap.size():%d", __FUNCTION__, iHiddenCount, mRenderMap.size());
   if (iHiddenCount >= mRenderMap.size()) {
      hide();
	  globalToolManager->GetDataManager()->WriteLog("%s iHiddenCount == mRenderMap.size()", __FUNCTION__);
   }

   TestLayout();
   //ResizeWndManagerSize();
}

void RenderManager::slot_clickedFloatRender(QString userid, QString streamId, QString nickName, int role_type) {
   if (userid.isEmpty() || streamId.isEmpty()) {
      return;
   }
}

void RenderManager::CloseFloatRender(QString userid) {
   //关闭独立窗口之后，在顶层渲染窗口中显示对应窗口。然后重新渲染。
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(userid);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
       showRender(render);
       setCenterManageWidth();
       QString strId = render->GetUserJoinID();

       QString strJoinId = QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
       if (strId.compare(strJoinId) == 0) {
          mShowSelf = true;
       }
         globalToolManager->GetDataManager()->WriteLog("%s render->show() userid:%s", __FUNCTION__, render->GetUserJoinID().toStdString().c_str());
         render->RenderBigStreamToWnd(userid);
      }
      show();
   }
}

void RenderManager::SetUserMicState(QString userid, bool open) {
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(userid);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->SetMicState(open);
      }
   }
}

void RenderManager::SetAllMicState(const bool & bOpen)
{
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   while (iter!= mRenderMap.end())
   {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->SetMicState(bOpen);
      }
      iter++;
   } 
}

void RenderManager::SetUserCameraState(QString userid, bool open) {
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(userid);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->SetCameraState(open);
      }
   }
}

void RenderManager::SetUserPenAuth(QString userId, bool hasAuth) {
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(userId);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->SetPenState(hasAuth);
      }
   }
}

void RenderManager::UpdateLocalStreamID(QString userId, QString streamID) {
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(userId);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->SetStreamID(streamID);
      }
   }
}

void RenderManager::UnPublishAllUsers() {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   while (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = (TeacherRenderWdg*)iter->second;
      if (render) {
         QString userId = render->GetUserJoinID();
         if (true) {
            globalToolManager->GetDataManager()->WriteLog("%s  KickUserPublish %s", __FUNCTION__, userId.toStdString().c_str());
            globalToolManager->GetClassSDK()->KickUserPublish(userId.toStdString());
         }
         else {
            globalToolManager->GetDataManager()->WriteLog("%s UserPublishCallback(PushStreamEvent_UnPublish, %s) 762 PushStreamEvent_UnPublish", __FUNCTION__, userId.toStdString().c_str());
            globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish, userId.toStdString());
         }
      }
      iter++;
   }
   globalToolManager->GetDataManager()->WriteLog("%s  Leave", __FUNCTION__);
}

void RenderManager::ClearAllRender() {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);

   //for (int i = 0; i < ui.listWidget->count(); i++) {
   //   QListWidgetItem *item = ui.listWidget->item(i);
   //   TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(ui.listWidget->itemWidget(item));
   //   if (render) {
   //      //QString wndId = WND_ID_FLOAT_RENDER + render->GetUserJoinID();
   //      //if (globalToolManager->GetWndManager()->IsWndExist(wndId)) {
   //      //    globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, wndId));
   //      //}
   //      //QString FullWndId = WND_ID_FULL_SCREEN_RENDER + render->GetUserJoinID();
   //      //if (globalToolManager->GetWndManager()->IsWndExist(FullWndId)) {
   //      //   globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, FullWndId));
   //      //}
   //   }
   //   ui.listWidget->item(i)->setHidden(true);

	  //render->hide();
	  //globalToolManager->GetDataManager()->WriteLog("%s render->hide() userid:%s", __FUNCTION__, render->GetUserJoinID().toStdString().c_str());
   //   //item = ui.listWidget->takeItem(0);
   //   ////ResizeWndManagerSize();
   //   //delete render;
   //   //delete item;
   //}
   //mRenderMap.clear();
   //std::unordered_map<QString, TeacherRenderWdg*>  = mRenderMap.begin();

   for (auto& ite : mRenderMap) {
      TeacherRenderWdg* pRender = ite.second;
      if (pRender) {
         pRender->hide();
      }
   }


   hide();
   globalToolManager->GetDataManager()->WriteLog("%s  Leave", __FUNCTION__);
}

void RenderManager::ShowAllRender() {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   int iVisibleN = 0;
   std::string strJoinId = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id;
   while (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = (TeacherRenderWdg*)iter->second;
      if (render) {
         QString strId = (QString)iter->first;
         if (globalToolManager->GetClassSDK()->IsRemoteStreamIsExist(strId.toStdWString(), VHStreamType_AVCapture)
            || ((0 == strId.compare( QString::fromStdString( strJoinId) )) && mShowSelf/* globalToolManager->GetClassSDK()->IsStreamExit(strJoinId)*/))
         {
            showRender(render);
            setCenterManageWidth();
			   globalToolManager->GetDataManager()->WriteLog("%s render->show() userid:%s", __FUNCTION__, render->GetUserJoinID().toStdString().c_str());
            iVisibleN++;
         }
      }

      iter++;
   }

   if (iVisibleN > 0) {
      show();
   }

   
   globalToolManager->GetDataManager()->WriteLog("%s  Leave", __FUNCTION__);
}

void RenderManager::HideAllRender() {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   QString strJoinId = QString::fromStdString( globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);

   while (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = (TeacherRenderWdg*)iter->second;
      if (render) {
         render->hide();
		 globalToolManager->GetDataManager()->WriteLog("%s render->hide() userid:%s", __FUNCTION__, render->GetUserJoinID().toStdString().c_str());
      }
      QString strId = (QString)iter->first;
      if (strId.compare(strJoinId) == 0) {
         mShowSelf = false;
      }
	  //std::unordered_map<QString, QListWidgetItem*>::iterator itemiter = mRenderItemMap.find(strId);
	  //if (itemiter != mRenderItemMap.end()) {
		 // ui.listWidget->setItemHidden(itemiter->second, true);
		 // globalToolManager->GetDataManager()->WriteLog("%s mRenderItemMap itemiter->hide() userid:%s", __FUNCTION__, strId.toStdString().c_str());
	  //}
      iter++;
   }
   hide();
   globalToolManager->GetDataManager()->WriteLog("%s  Leave", __FUNCTION__);
}

int RenderManager::GetListWidgetWidth() {
   //return ui.listWidget->width();
   return 0;
}

void RenderManager::show()
{
   int iSize = iRenderSize();
   globalToolManager->GetDataManager()->WriteLog("%s size: %d" , __FUNCTION__, iSize);
   if (iSize>0) {
      QWidget::show();
   }
   else
   {      
      QWidget::hide();
   }
   
}

void RenderManager::showEvent(QShowEvent * event)
{
   QWidget::showEvent(event);
   int iWidth = this->width();
   ui.mCenterManageWdg->setMinimumWidth(ui.mCenterWdg->width());
   ui.mCenterManageWdg->setFixedHeight(ITEM_HEIGHT);
   ui.mCenterWdg->setFixedHeight(ITEM_HEIGHT);
   this->setFixedHeight(ITEM_HEIGHT + 1);
   ui.mCenterManageWdg->move(0, 0);

}

void RenderManager::InsertWidgetToListWidget(TeacherRenderWdg* render, QString userid, bool bADD /*= true*/) {
   if (render != nullptr) {
      globalToolManager->GetDataManager()->WriteLog("***************%s start********************\n render: %x  %p", __FUNCTION__, render, render);
      render->setFixedSize(QSize(ITEM_WIDTH, ITEM_HEIGHT));
      globalToolManager->GetDataManager()->WriteLog("%s will addWidget Render : %d ", __FUNCTION__, userid.toStdString().data());
      TestLayout();
      globalToolManager->GetDataManager()->WriteLog("%s will ADDWIDGET Render : %d ", __FUNCTION__, userid.toStdString().data());
      ui.RenderLayout->addWidget(render);
      TestLayout();
      mRenderMap.insert(std::map<QString, TeacherRenderWdg*>::value_type(userid, render));
      render->InitRenderCtrl(userid);
      setCenterManageWidth();
      showRender(render);
      globalToolManager->GetDataManager()->WriteLog("***************End   mRenderMap.size()********************\n", mRenderMap.size());
   }
   
}

bool RenderManager::IsExistRender(const QString& uid) {
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(uid);
   if (iter != mRenderMap.end()) {
	   globalToolManager->GetDataManager()->WriteLog("%s find %s",__FUNCTION__,uid.toStdString().c_str());
      return true;
   }
   globalToolManager->GetDataManager()->WriteLog("%s not find %s", __FUNCTION__, uid.toStdString().c_str());
   return false;
}

void RenderManager::SetAllMuteState(bool open) {
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   while (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->SetMicState(open);
      }
      iter++;
   }
}

void RenderManager::HandleUserLineState(QString user_id, bool onLine) {
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(user_id);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->HandleUserLineState(onLine);
      }
      iter++;
   }
}



void RenderManager::initTrophyRewardNum(const QString& strId, const int& iNum)
{
   //return;
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(strId);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->initTrophyRewardNum(iNum);
      }
   }
}

void RenderManager::SetCameraQss(const QString& strId, const bool& bOpenState)
{
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(strId);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* Temp = iter->second;
      if (NULL != Temp && 0 == Temp->GetUserJoinID().compare(strId))
      {
         Temp->SetCameraState(bOpenState);
      }
   }
}

HWND RenderManager::GetRenderWndID(const QString& strUid)
{
   HWND id;
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(strUid);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         id = (HWND)(render->GetRenderWndID());
      }
   }
   return id;
}

std::shared_ptr<vhall::VideoRenderReceiveInterface> RenderManager::GetRenderWnd(const QString& strUid)
{
   std::shared_ptr<vhall::VideoRenderReceiveInterface> id;
   //globalToolManager->GetDataManager()->WriteLog("***************%s start********************\n", __FUNCTION__);
   //ResizeWndManagerSize();

   //globalToolManager->GetDataManager()->WriteLog("***************END********************\n");
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(strUid);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         id = (render->GetRenderWnd());
      }
   }
   return id;
}

void RenderManager::UpdateInfo(const QString & strUid, const QString& strName)
{
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(strUid);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render && strName.compare(render->NickName()) != 0) {
         render->SetNickName(strName);
      }
   }
}

int RenderManager::IUserCount()
{
   return mRenderMap.size();
}

TeacherRenderWdg* RenderManager::GetFloateRender(QString strId)
{
   TeacherRenderWdg* render = nullptr;
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(strId);
   //std::unordered_map<QString, QListWidgetItem*>::iterator iitemter = mRenderItemMap.find(strId);
   if (iter != mRenderMap.end()) {
      render = iter->second;
      mRenderMap.erase(iter);
      globalToolManager->GetDataManager()->WriteLog("%s will Remov Render : %d ", __FUNCTION__, strId.toStdString().data());
      TestLayout();
      globalToolManager->GetDataManager()->WriteLog("%s will REMOVED Render : %d ", __FUNCTION__, strId.toStdString().data());
      ui.RenderLayout->removeWidget(render);
      TestLayout();
	  //mRenderItemMap.erase(iitemter);
      if (mRenderMap.size() == 0) {
         globalToolManager->GetDataManager()->WriteLog("%s mRenderMap.size()： %d  hide()", __FUNCTION__, mRenderMap.size());
         hide();
      }
      else {
         globalToolManager->GetDataManager()->WriteLog("%s mRenderMap.size()： %d  show()", __FUNCTION__, mRenderMap.size());
      }
   }
   else {
      globalToolManager->GetDataManager()->WriteLog("%s not find Render Form map", __FUNCTION__);
   }
   setCenterManageWidth();
   return render;
}

void RenderManager::ShowRender(QStringList strListId)
{
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   for (; iter != mRenderMap.end(); iter++) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      showRender(render);
   }
}

void RenderManager::showRender(TeacherRenderWdg * render)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   if (render) {
      QString strId = render->GetUserJoinID().trimmed();
      ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (0 == strId.compare(QString::fromStdString(roomInfo.mUserInfo.join_id)) && globalToolManager->GetClassSDK()->LocalStreamId().length() > 0) {//本人切流正常
         
         render->show();
         show();
         globalToolManager->GetDataManager()->WriteLog(" %s show Render(self) %s ", __FUNCTION__, strId.toStdString().c_str());
      }
      else if (pRtcLiveWdg && pRtcLiveWdg->IsUserSpeaking(strId) 
         && globalToolManager->GetClassSDK()->GetSubScribeStreamId(strId.toStdWString(), VHStreamType_AVCapture).length()  >0) {//
         
         render->show();
         show();
         globalToolManager->GetDataManager()->WriteLog("%s  show Render %s", __FUNCTION__, strId.toStdString().c_str());
      }
      else {
         render->hide();
         globalToolManager->GetDataManager()->WriteLog("%s  strId 【%s】（isEmpty %d） streamId【%s】（isEmpty %d）  hide Render", __FUNCTION__,
            strId.toStdString().c_str(), strId.isEmpty(), globalToolManager->GetClassSDK()->GetSubScribeStreamId(strId.toStdWString(), VHStreamType_AVCapture).c_str(),
            globalToolManager->GetClassSDK()->GetSubScribeStreamId(strId.toStdWString(), VHStreamType_AVCapture).length());
      }
   }

   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}
void RenderManager::slot_MuteAllSubScribeAudio(const bool& bMute)
{
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.begin();
   while (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      render->SetSmallRenderMicState(!bMute);
      iter++;
   }
}

void RenderManager::AddTrophyCount(const QString& strId, const int& iAdd)
{
   std::unordered_map<QString, TeacherRenderWdg*>::iterator iter = mRenderMap.find(strId);
   if (iter != mRenderMap.end()) {
      TeacherRenderWdg* render = dynamic_cast<TeacherRenderWdg*>(iter->second);
      if (render) {
         render->slotTrophyAdd(iAdd);
      }
   }
}