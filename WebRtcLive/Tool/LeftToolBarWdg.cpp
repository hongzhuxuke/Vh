#include "LeftToolBarWdg.h"

#include <QMenu> 
#include <QDesktopWidget> 
#include <QDebug>
#include <QFileDialog>
#include "ConfigSetting.h"
#include "MediaPlayWebView.h"
#include "BlackMenuWdg.h"

#include "menu.h"
#include "softwareSourceDlg.h"
#include "screenChoiceDlg.h"
#include "../Unility/Unility.h"
#include "WebRtcLiveWdg.h"
#include "SignInDlg.h"
#include "TipsWdg.h"

//#include "MediaFilePlay/VedioPlayUI.h"
#include "MediaFilePlay/VideoChoiceUI.h"
#include "DoubleRenderWdg.h"
#include "priConst.h"
#include "MediaPlayWebView.h"
#include "AnswerWebView.h"
#include "DoublePubStuChoiceUI.h"
#include "vhfadeouttipwidget.h"
#include "LibCefViewWdg.h"
#include "ChooseTestPaperDlg.h"
#include "ShadowBoxDlg.h"
//#include "BaiduPaperDlg.h"

#define DESKTOPSHAR  QStringLiteral("桌面共享")
#define SOFTWARESHAR QStringLiteral("软件共享")

#define STOPSHAR  QStringLiteral("结束共享")

#define FROMDB  QStringLiteral("来自资料库")
#define FROMLOCAL QStringLiteral("来自本地")

#define DOUBLECAMERA QStringLiteral("双推")
#define CLOSEDOUBLECAMERA QStringLiteral("关闭双推")

#define PLAYUI_PLAYSTATE
#define PLAYUI_PLAYSTATE_STOPPING  -2
#define PLAYUI_PLAYSTATE_FAIL      -1
#define PLAYUI_PLAYSTATE_NOTHING   0
#define PLAYUI_PLAYSTATE_OPENING   1
#define PLAYUI_PLAYSTATE_BUFFERING 2
#define PLAYUI_PLAYSTATE_PALYERING 3
#define PLAYUI_PLAYSTATE_PAUSED    4
#define PLAYUI_PLAYSTATE_STOP      5
#define PLAYUI_PLAYSTATE_END       6
#define PLAYUI_PLAYSTATE_ERROR     7

#define MENU_QSS              "QMenu {\
                     background-color:#ffffff; ; /*整个背景*/\
                     border: 3px solid #ffffff; ;/*整个菜单边缘*/  \
					 border-radius:8px; \
					}\
					 QMenu::item{ \
						 font-size: 12px; \
						 color: #666666;  /*字体颜色*/\
						 border: 3px solid #ffffff;    /*item选框*/\
						 background-color: #ffffff;\
						 padding:2px 8px 2px 8px; /*设置菜单项文字上下和左右的内边距，效果就是菜单中的条目左右上下有了间隔*/\
						 margin:2px 2px;/*设置菜单项的外边距*/\
					 }\
						 QMenu::item:selected{ \
														 background-color:#EEFAF4;/*选中的样式*/\
							border-color::#EEFAF4;/*选中的样式EEFAF4*/ \
							color: #52CC90;  /*字体颜色*/\
					 }\
						 QMenu::item:pressed{/*菜单项按下效果*/\
												   border: 1px solid #ffffff;  \
												   background-color: #ffffff;  \
}"

/*自定义控件文本样式*/
#define MAPLEAVE QStringLiteral("QToolButton{\
	border:none;\
	color: rgb(201, 201, 203);\
	   font: 13px \"微软雅黑\";\
	}\
	QToolButton:hover\
	{\
	color: rgb(82, 204, 144);\
	}\
	QToolButton:checked\
	{\
	color: rgb(82, 204, 144);\
	}\
QToolButton:disabled{ \
	color: rgb(145, 146, 150); \
}")

/*自定义控件文本鼠标移入样式*/
#define MAPHOVER QStringLiteral("QToolButton\
	{\
	border:none;\
	font: 13px \"微软雅黑\";\
	color: rgb(82, 204, 144);\
	}")

/*文档样式*/
#define DOCLEAVE QStringLiteral("QToolButton{ \
border:none;\
border-image: url(:/button/img/button/doc_Normal.png); \
	} \
	QToolButton:hover{ \
		border-image: url(:/button/img/button/doc_clicked.png); \
	} \
	QToolButton:checked{ \
		border-image: url(:/button/img/button/doc_clicked.png); \
	} \
	QToolButton:disabled{ \
	border-image: url(:/button/img/button/doc_Disabled.png); \
	}")

#define DOCHOVER QStringLiteral("QToolButton{ \
	border-image: url(:/button/img/button/doc_clicked.png); \
	}")

/*白板样式*/
#define WHITEBOARDLEAVE QStringLiteral("CMapButton{ \
border:none;\
	border-image: url(:/button/img/button/Whiteboard_Normal.png);\
	}\
	CMapButton:hover{\
	border-image: url(:/button/img/button/Whiteboard_Click.png);\
	}\
	CMapButton:checked{\
	border-image: url(:/button/img/button/Whiteboard_Click.png);\
	}\
	CMapButton:disabled{\
	border-image: url(:/button/img/button/Whiteboard_Disabled.png);\
	}")
#define WHITEBOARDHOVER QStringLiteral("CMapButton{\
	border-image: url(:/button/img/button/Whiteboard_Click.png);\
	}")

/*桌面共享样式*/
#define DESKTOPSHARLEAVE QStringLiteral("CMapButton::menu-indicator{ image: None; } \
	CMapButton{\
border:none;\
	border-image: url(:/button/img/button/DesktopSharping_Normal.png);\
	}\
	CMapButton:hover{\
	border-image: url(:/button/img/button/DesktopSharping_Click.png);\
	}\
	CMapButton:checked{\
	border-image: url(:/button/img/button/DesktopSharping_Click.png);\
	}\
	CMapButton:disabled{\
	border-image: url(:/button/img/button/DesktopSharping_Disabled.png);\
	}")
#define DESKTOPSHARHOVER QStringLiteral("CMapButton::menu-indicator{ image: None; } \
	CMapButton{\
	border-image: url(:/button/img/button/DesktopSharping_Click.png);\
	}")

/*插播样式*/
#define MEDIALEAVE QStringLiteral("CMapButton::menu-indicator{ image: None; } \
	CMapButton{\
border:none;\
	border-image: url(:/button/img/button/Media_Normal.png);\
	}\
	CMapButton:hover{\
	border-image: url(:/button/img/button/Media_Click.png);\
	}\
	CMapButton:checked{\
	border-image: url(:/button/img/button/Media_Click.png);\
	}\
	CMapButton:disabled{\
	border-image: url(:/button/img/button/Media_Disabled.png);\
	}")
#define MEDIAHOVER QStringLiteral("CMapButton::menu-indicator{ image: None; } \
	CMapButton{\
	border-image: url(:/button/img/button/Media_Click.png);\
	}")

/*问答样式*/
#define ANSWERLEAVE QStringLiteral("CMapButton{ \
border:none;\
	border-image: url(:/button/img/button/Answer_Normal.png);\
	}\
	CMapButton:hover{\
	border-image: url(:/button/img/button/Answer_Click.png);\
	}\
	CMapButton:checked{\
	border-image: url(:/button/img/button/Answer_Click.png);\
	}\
	CMapButton:disabled{\
	border-image: url(:/button/img/button/Answer_Disabled.png);\
	}")
#define ANSWERHOVER QStringLiteral("CMapButton{\
	border-image: url(:/button/img/button/Answer_Click.png);\
	}")

/*互动工具样式*/
#define INTERACTIONLEAVE QStringLiteral("CMapButton{ \
border:none;\
	border-image: url(:/button/img/button/InteractionTool.png);\
	}\
	CMapButton:hover{\
	border-image: url(:/button/img/button/InteractionTool_clicked.png);\
	}\
	CMapButton:checked{\
	border-image: url(:/button/img/button/InteractionTool_clicked.png);\
	}\
	CMapButton:disabled{\
	border-image: url(:/button/img/button/InteractionTool_Disable.png);\
	}")
#define INTERACTIONHOVER QStringLiteral("CMapButton{\
	border-image: url(:/button/img/button/InteractionTool_clicked.png);\
	}")

/*问答样式*/
#define QUESANDANSQSS "QPushButton{\
border-image:url(:/menu/img/Menu/QuesAndans.png);\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/QuesAndansHover.png);\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/QuesAndans.png);\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/QuesAndansHover.png);\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/QuesAndansDisenable.png);\
}"

/*试卷样式*/
#define TESTPAPERQSS "QPushButton{\
border-image:url(:/menu/img/Menu/testPaper.png);\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/testPaperHover.png);\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/testPaper.png);\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/testPaperHover.png);\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/testPaperDis.png);\
}"
/*签到样式*/
#define SIGNINQSS "QPushButton{\
border-image:url(:/menu/img/Menu/signIn.png);\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/signInHover.png);\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/signIn.png);\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/signInHover.png);\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/signInDis.png);\
}"
/*答题器样式*/
#define ANSWERQSS "QPushButton{\
border-image:url(:/menu/img/Menu/Answer.png);\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/AnswerHover.png);\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/Answer.png);\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/AnswerHover.png);\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/AnswerDis.png);\
}"
/*计时器样式*/
#define TIMERQSS "QPushButton{\
border-image:url(:/menu/img/Menu/timer.png);\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/timerHover.png);\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/timer.png);\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/timerHover.png);\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/timerDis.png);\
}"

/*桌面共享样式*/
#define DESKTOPSHAREQSS "QPushButton{\
border-image:url(:/menu/img/Menu/Sharing.png) 0 0 60 0;\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/Sharing_hover.png)  0 0 60 0;\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/Sharing.png) 0 0 60 0;\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/Sharing_hover.png)  0 0 60 0;\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/DesktopSharingDis.png);\
}"
/*软件共享样式*/
#define SOFTWARESHAREQSS "QPushButton{\
border-image:url(:/menu/img/Menu/Sharing.png) 30 0 30 0;\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/Sharing_hover.png)  30 0 30 0;\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/Sharing.png) 30 0 30 0;\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/Sharing_hover.png)  30 0 30 0;\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/SoftwareSharingDis.png);\
}"
/*学员软件共享样式*/
#define STUDESKTOPSHAREQSS "QPushButton{\
border-image:url(:/menu/img/Menu/Sharing.png) 60 0 0 0;\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/Sharing_hover.png) 60 0 0 0;\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/Sharing.png) 60 0 0 0;\
}\
	QPushButton:checked{\
	border-image:url(:/menu/img/Menu/Sharing_hover.png) 60 0 0 0;\
	}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/SoftwareSharingDis.png);\
}"
/*资料库样式*/
#define DATABASEQSS "QPushButton{\
border-image:url(:/menu/img/Menu/database.png);\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/databaseHover.png);\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/database.png);\
}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/databaseDis.png);\
}\
QPushButton:checked\
{\
border-image: url(:/menu/img/Menu/databaseHover.png);\
}"
/*本地插播样式*/
#define LOCALQSS "QPushButton{\
border-image:url(:/menu/img/Menu/local.png);\
}\
QPushButton:hover{\
border-image:url(:/menu/img/Menu/localHover.png);\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/local.png);\
}\
QPushButton:!enabled{\
border-image:url(:/menu/img/Menu/localDis.png);\
}\
QPushButton:checked\
{\
border-image: url(:/menu/img/Menu/localHover.png);\
}"

/*双推样式*/
#define DOUBLEPUSHQSS QStringLiteral("CMapButton::menu-indicator{ image: None; } \
	CMapButton{\
border:none;\
	border-image: url(:/button/img/button/btnDoubleNormal.png);\
	}\
	CMapButton:hover{\
	border-image: url(:/button/img/button/btnDoubleHover.png);\
	}\
	CMapButton:checked{\
	border-image: url(:/button/img/button/btnDoubleHover.png);\
	}\
	CMapButton:disabled{\
	border-image: url(:/button/img/button/btnDoubleDisenable.png);\
	}")
#define DOUBLEPUSHQSSHOVER QStringLiteral("CMapButton::menu-indicator{ image: None; } \
	CMapButton{\
	border-image: url(:/button/img/button/btnDoubleHover.png);\
	}")
//讲师双推样式
#define TEACHERDOUBLEQSS "QPushButton{\
border-image:url(:/menu/img/Menu/doubleCameraNormal.png) 0 0 30 0;\
}\
QPushButton:hover{\
background-color: rgb(42, 45, 50);\
border-image:url(:/menu/img/Menu/doubleCameHover.png)  0 0 30 0;\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/doubleCameraNormal.png)  0 0 30 0;\
}\
QPushButton:checked\
{\
border-image: url(:/menu/img/Menu/doubleCameHover.png)  0 0 30 0;\
}"
//学员双推样式
#define STUDOUBLEQSS "QPushButton{\
border-image:url(:/menu/img/Menu/doubleCameraNormal.png) 30 0 0 0;\
}\
QPushButton:hover{\
background-color: rgb(42, 45, 50);\
border-image:url(:/menu/img/Menu/doubleCameHover.png) 30 0 0 0;\
}\
QPushButton:pressed{\
border-image:url(:/menu/img/Menu/doubleCameraNormal.png) 30 0 0 0;\
}\
QPushButton:checked\
{\
border-image: url(:/menu/img/Menu/doubleCameHover.png) 30 0 0 0;\
}"

extern ToolManager *globalToolManager;
LeftToolBarWdg::LeftToolBarWdg(QWidget *parent)
   : CWidget(parent)
   /* , mbIsCameraOpenDoublePush(false)*/
{
   ui.setupUi(this);
   this->setFixedWidth(68);

   ui.btnDoc->setObjectName("btnDoc");
   ui.btnWhiteboard->setObjectName("btnWhiteboard");
   ui.btnDesktopSharing->setObjectName("btnDesktopSharing");
   ui.btnMedia->setObjectName("btnMedia");
   ui.btnAnswer->setObjectName("btnAnswer");
   ui.btnInteractionTool->setObjectName("btnInteractionTool");
   ui.btnDoublePushCamera->setObjectName("btnDoublePushCamera");

   ui.btnDoc2->setObjectName("btnDoc2");
   ui.btnWhiteboard2->setObjectName("btnWhiteboard2");
   ui.btnDesktopSharing2->setObjectName("btnDesktopSharing2");
   ui.btnMedia2->setObjectName("btnMedia2");
   ui.btnAnswer2->setObjectName("btnAnswer2");
   ui.btnInteractionTool2->setObjectName("btnInteractionTool2");
   ui.btnDoublePushCamera2->setObjectName("btnDoublePushCamera2");

   //桌面共享菜单
   mpDesktopSharWdg = new BlackMenuWdg(this);
   if (nullptr != mpDesktopSharWdg)
   {
      mpDesktopSharWdg->addTrigger(ui.btnDesktopSharing);
      mpDesktopSharWdg->addTrigger(ui.btnDesktopSharing2);
      mpDesktopSharWdg->addItem(OpDesktopSharType_DesktopShar, DESKTOPSHAREQSS, false);
      mpDesktopSharWdg->addItem(OpDesktopSharType_SoftwareShar, SOFTWARESHAREQSS, false);

      connect(mpDesktopSharWdg, &BlackMenuWdg::sigClicked, this, &LeftToolBarWdg::slots_DesktopSharing);
      connect(mpDesktopSharWdg, &BlackMenuWdg::sigClose, this, &LeftToolBarWdg::slots_MenuAboutToHide);
   }
   //插播菜单
   mpMediaWdg = new BlackMenuWdg(this);
   if (nullptr != mpMediaWdg)
   {
      mpMediaWdg->addTrigger(ui.btnMedia);
      mpMediaWdg->addTrigger(ui.btnMedia2);
      mpMediaWdg->addItem(OpMediaType_database, DATABASEQSS);
      mpMediaWdg->addItem(OpMediaType_Local, LOCALQSS);
      connect(mpMediaWdg, &BlackMenuWdg::sigClicked, this, &LeftToolBarWdg::slots_MediaMenu);
      connect(mpMediaWdg, &BlackMenuWdg::sigClose, this, &LeftToolBarWdg::slots_MediaMenuAboutToHide);
   }
   //双向双推菜单
   mpDoublePushWdg = new BlackMenuWdg(this);
   if (nullptr != mpDoublePushWdg)
   {
      mpDoublePushWdg->addTrigger(ui.btnDoublePushCamera);
      mpDoublePushWdg->addTrigger(ui.btnDoublePushCamera);

      mpDoublePushWdg->addItem(OpType_TeacherDoublCamera, TEACHERDOUBLEQSS, false);
      mpDoublePushWdg->addItem(OpType_StuDoublCamera, STUDOUBLEQSS, false);

      connect(mpDoublePushWdg, &BlackMenuWdg::sigClicked, this, &LeftToolBarWdg::slot_doublePush);
      connect(mpDoublePushWdg, &BlackMenuWdg::sigClose, this, &LeftToolBarWdg::slots_DoublePushToHide);

      mpDoublePushWdg->setItemEnabled(OpType_StuDoublCamera, true);
   }

   //ui.btnDoublePushCamera->hide();
   //ui.btnDoublePushCamera2->hide();
   ui.DoublePushCameraFrame->hide();

   ui.btnDoc2->setHoverStyle(MAPHOVER);
   ui.btnDoc2->setLeaveStyle(MAPLEAVE);

   ui.btnWhiteboard2->setHoverStyle(MAPHOVER);
   ui.btnWhiteboard2->setLeaveStyle(MAPLEAVE);

   ui.btnDesktopSharing2->setHoverStyle(MAPHOVER);
   ui.btnDesktopSharing2->setLeaveStyle(MAPLEAVE);

   ui.btnMedia2->setHoverStyle(MAPHOVER);
   ui.btnMedia2->setLeaveStyle(MAPLEAVE);

   ui.btnAnswer2->setHoverStyle(MAPHOVER);
   ui.btnAnswer2->setLeaveStyle(MAPLEAVE);

   ui.btnInteractionTool2->setHoverStyle(MAPHOVER);
   ui.btnInteractionTool2->setLeaveStyle(MAPLEAVE);

   ui.btnDoublePushCamera2->setHoverStyle(MAPHOVER);
   ui.btnDoublePushCamera2->setLeaveStyle(MAPLEAVE);

   ui.btnDoc->setHoverStyle(DOCHOVER);
   ui.btnDoc->setLeaveStyle(DOCLEAVE);
   ui.btnDoc->setMapButton(ui.btnDoc2);

   ui.btnWhiteboard->setHoverStyle(WHITEBOARDHOVER);
   ui.btnWhiteboard->setLeaveStyle(WHITEBOARDLEAVE);
   ui.btnWhiteboard->setMapButton(ui.btnWhiteboard2);

   ui.btnDesktopSharing->setHoverStyle(DESKTOPSHARHOVER);
   ui.btnDesktopSharing->setLeaveStyle(DESKTOPSHARLEAVE);
   ui.btnDesktopSharing->setMapButton(ui.btnDesktopSharing2);

   ui.btnMedia->setHoverStyle(MEDIAHOVER);
   ui.btnMedia->setLeaveStyle(MEDIALEAVE);
   ui.btnMedia->setMapButton(ui.btnMedia2);

   ui.btnAnswer->setHoverStyle(ANSWERHOVER);
   ui.btnAnswer->setLeaveStyle(ANSWERLEAVE);
   ui.btnAnswer->setMapButton(ui.btnAnswer2);

   ui.btnInteractionTool->setHoverStyle(INTERACTIONHOVER);
   ui.btnInteractionTool->setLeaveStyle(INTERACTIONLEAVE);
   ui.btnInteractionTool->setMapButton(ui.btnInteractionTool2);

   ui.btnDoublePushCamera->setHoverStyle(DOUBLEPUSHQSSHOVER);
   ui.btnDoublePushCamera->setLeaveStyle(DOUBLEPUSHQSS);
   ui.btnDoublePushCamera->setMapButton(ui.btnDoublePushCamera2);

   connect(ui.btnDoc2, &CMapButton::clicked, this, &LeftToolBarWdg::on_btnDoc_clicked);
   connect(ui.btnWhiteboard2, &CMapButton::clicked, this, &LeftToolBarWdg::on_btnWhiteboard_clicked);
   connect(ui.btnDesktopSharing2, &CMapButton::clicked, this, &LeftToolBarWdg::on_btnDesktopSharing_clicked);

   connect(ui.btnMedia2, &CMapButton::clicked, this, &LeftToolBarWdg::on_btnMedia_clicked);
   connect(ui.btnAnswer2, &CMapButton::clicked, this, &LeftToolBarWdg::on_btnAnswer_clicked);
   connect(ui.btnDoublePushCamera2, &CMapButton::clicked, this, &LeftToolBarWdg::on_btnDoublePushCamera_clicked);
   connect(ui.btnInteractionTool2, &CMapButton::clicked, this, &LeftToolBarWdg::on_btnInteractionTool_clicked);

   mpInteractionWdg = new BlackMenuWdg(this);
   if (nullptr != mpInteractionWdg)
   {
      mpInteractionWdg->addTrigger(ui.btnInteractionTool);
      mpInteractionWdg->addTrigger(ui.btnInteractionTool2);

      //mpInteractionWdg->addItem(InterActionOpType_QUESANDANS, QUESANDANSQSS);
      mpInteractionWdg->addItem(InterActionOpType_TESTPAPER, TESTPAPERQSS);
      mpInteractionWdg->addItem(InterActionOpType_SIGNIN, SIGNINQSS);
      mpInteractionWdg->addItem(InterActionOpType_ANSWER, ANSWERQSS);
      mpInteractionWdg->addItem(InterActionOpType_TIMER, TIMERQSS);

      connect(mpInteractionWdg, &BlackMenuWdg::sigClicked, this, &LeftToolBarWdg::slot_InteractionItemClicked);
      connect(mpInteractionWdg, &BlackMenuWdg::sigClose, this, &LeftToolBarWdg::slots_InteractionHide);

      //mpInteractionWdg->setItemEnabled(InterActionOpType_QUESANDANS, false);
      mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, true);//上课后，启用
      mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, true);
      mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, true);
      mpInteractionWdg->setItemEnabled(InterActionOpType_TIMER, true);
   }

   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI || baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      /*ui.btnDoublePushCamera->hide();
      ui.btnDoublePushCamera2->hide();*/
      ui.DoublePushCameraFrame->hide();
   }
   setEnabled(false);
}

//void LeftToolBarWdg::LeftToolBarWdgShow()
//{
//   qDebug() << this->width() << ui.label->width() << ui.btnDoublePushCamera->width() << ui.label_2->width();
//}

void LeftToolBarWdg::closeScreenChoiceDlg()
{
   if (nullptr != mpScreenChoiceDlg && mpScreenChoiceDlg->isVisible()) {
      mpScreenChoiceDlg->on_btnClose_clicked();
   }
}

LeftToolBarWdg::~LeftToolBarWdg()
{
   if (nullptr != mpScreenChoiceDlg) {
      delete mpScreenChoiceDlg;
      mpScreenChoiceDlg = nullptr;
   }

   stopTimer();
   if (nullptr != mpOvertimeTimer) {
      delete mpOvertimeTimer;
      mpOvertimeTimer = nullptr;
   }

   if (nullptr != mpSignInDlg)
   {
      delete mpSignInDlg;
      mpSignInDlg = nullptr;
   }

   if (nullptr == mpVideoChoiceUi)
   {
      delete mpVideoChoiceUi;
      mpVideoChoiceUi = nullptr;
   }
}

void LeftToolBarWdg::setEnableDoublePush(const bool& bEnable)
{
   if (bEnable) {
      mpDesktopSharWdg->addItem(OpDesktopSharType_StuDesktopShar, STUDESKTOPSHAREQSS, false);
      //ui.btnDoublePushCamera->show();
      //ui.btnDoublePushCamera2->show();
      ui.DoublePushCameraFrame->show();
   }
   else {
      /*ui.btnDoublePushCamera->hide();
      ui.btnDoublePushCamera2->hide();*/
      ui.DoublePushCameraFrame->hide();
   }
}

void LeftToolBarWdg::setEnabled(bool benabled)
{
   if (mCurType != OpType_StuDoublCamera && mCurType != OpType_TeacherDoublCamera
      && mCurType != OpType_StuDesktopShar) {

      ui.btnDoc->setEnabled(benabled);
      ui.btnDoc2->setEnabled(benabled);

      ui.btnWhiteboard->setEnabled(benabled);
      ui.btnWhiteboard2->setEnabled(benabled);

      //ui.btnDesktopSharing->setEnabled(benabled);
      //ui.btnDesktopSharing2->setEnabled(benabled);
      SetBtnDesktopSharingEnabled(benabled);

      ui.btnMedia->setEnabled(benabled);
      ui.btnMedia2->setEnabled(benabled);

      ui.btnAnswer->setEnabled(benabled);
      ui.btnAnswer2->setEnabled(benabled);

      ui.btnInteractionTool->setEnabled(benabled);
      ui.btnInteractionTool2->setEnabled(benabled);

      //ui.btnDoublePushCamera->setEnabled(benabled);
      //ui.btnDoublePushCamera2->setEnabled(benabled);
      SetDoublePushCameraEnabled(benabled);
   }
   CWidget::setEnabled(benabled);
}

//void LeftToolBarWdg::slot_DoublePushCameraWdgClose() {
//
//	mbIsCameraOpenDoublePush = false;
//}

void LeftToolBarWdg::mousePressEvent(QMouseEvent *event) {
   return;
   if (event) {
      mPressPoint = event->pos();
      mIsMoved = true;
   }
}

void LeftToolBarWdg::mouseMoveEvent(QMouseEvent *event) {

   return;
   if (event && (event->buttons() == Qt::LeftButton) && mIsMoved  && m_bIsEnableMove) {
      QWidget* parent_widget = this->nativeParentWidget();
      QString objectName = parent_widget->objectName();
      QPoint parent_point = parent_widget->pos();
      parent_point.setX(parent_point.x() + event->x() - mPressPoint.x());
      parent_point.setY(parent_point.y() + event->y() - mPressPoint.y());
      parent_widget->move(parent_point);
      emit signal_move(parent_point);
   }
}

void LeftToolBarWdg::mouseReleaseEvent(QMouseEvent *) {
   return;
   if (mIsMoved) {
      mIsMoved = false;
   }
}

void LeftToolBarWdg::on_btnDoc_clicked(bool checked /*= false*/) {
   if (mpWebRtcLiveWdg->HasPenAuth()) {
      //LeftToolBarWdgShow();
      SetOpType(OpType_Doc);
      emit sig_btnDoc();
   }
   else if (OpType_Doc != mCurType) {
      ui.btnDoc->setChecked(false);
      ui.btnDoc2->setChecked(false);
      mpWebRtcLiveWdg->FadeOutTip(Str_PleaseBackPenAuth, Tost_Warning);
   }
   
}

void LeftToolBarWdg::show()
{
   CWidget::show();
}

void LeftToolBarWdg::slot_MutexWdgClose()
{
   if (CLASS_STATUS_IN_CLASS == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type)
   {
      SetBtnAnswerEnabled(true);
      setAnserChecked(false);

      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_database, false);
         mpMediaWdg->setItemEnabled(OpMediaType_database, true);
      }

      if (nullptr != mpInteractionWdg)
      {
         mpInteractionWdg->SetItemChecked(InterActionOpType_SIGNIN, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, true);
         mpInteractionWdg->SetItemChecked(InterActionOpType_ANSWER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, true);
         mpInteractionWdg->SetItemChecked(InterActionOpType_TESTPAPER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, true);
      }
   }
}

void LeftToolBarWdg::slot_ReSelection(const DoublePubStuChoiceUI::eChoiceType& eType)
{
   if (DoublePubStuChoiceUI::eChoiceType_Deskop == eType) {
      SetShareType(eDestopType_StuDesktop);
      slots_ScreenSharing();
   }
   else if (DoublePubStuChoiceUI::eChoiceType_Camera == eType) {
	   mbRequestByManual = true;
      StuDoublePush();
   }
}

void LeftToolBarWdg::setMutexItemChecked(const OpType& eType)
{

   switch (eType)
   {
   case OpType_Anser: {//答题
      SetBtnAnswerEnabled(true);
      setAnserChecked(true);

      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_database, false);
         mpMediaWdg->setItemEnabled(OpMediaType_database, false);
      }

      if (nullptr != mpInteractionWdg)
      {//禁用签到、答题器、考试
         mpInteractionWdg->SetItemChecked(InterActionOpType_SIGNIN, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, false);
         mpInteractionWdg->SetItemChecked(InterActionOpType_ANSWER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, false);

         mpInteractionWdg->SetItemChecked(InterActionOpType_TESTPAPER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, false);
      }

   }
                      break;
   case OpMediaType_database: {//插播--资料库

      ui.btnMedia->setChecked(false);
      ui.btnMedia2->setChecked(false);

      setAnserChecked(false);
      SetBtnAnswerEnabled(false);

      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_database, true);
         mpMediaWdg->setItemEnabled(OpMediaType_database, true);
      }

      if (nullptr != mpInteractionWdg)
      {
         mpInteractionWdg->SetItemChecked(InterActionOpType_SIGNIN, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, false);
         mpInteractionWdg->SetItemChecked(InterActionOpType_ANSWER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, false);
         mpInteractionWdg->SetItemChecked(InterActionOpType_TESTPAPER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, false);
      }
   }
                              break;
   case InterActionOpType_SIGNIN: {//互动工具--签到
      ui.btnInteractionTool->setChecked(false);
      ui.btnInteractionTool2->setChecked(false);

      setAnserChecked(false);
      SetBtnAnswerEnabled(false);

      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_database, false);
         mpMediaWdg->setItemEnabled(OpMediaType_database, false);

      }

      if (nullptr != mpInteractionWdg)
      {
         mpInteractionWdg->SetItemChecked(InterActionOpType_SIGNIN, true);
         mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, true);

         mpInteractionWdg->SetItemChecked(InterActionOpType_ANSWER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, false);
         mpInteractionWdg->SetItemChecked(InterActionOpType_TESTPAPER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, false);
      }
   }
                                  break;
   case   InterActionOpType_ANSWER: {//答题器

      ui.btnInteractionTool->setChecked(false);
      ui.btnInteractionTool2->setChecked(false);

      setAnserChecked(false);
      SetBtnAnswerEnabled(false);

      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_database, false);
         mpMediaWdg->setItemEnabled(OpMediaType_database, false);
      }

      if (nullptr != mpInteractionWdg)
      {
         mpInteractionWdg->SetItemChecked(InterActionOpType_ANSWER, true);
         mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, true);

         mpInteractionWdg->SetItemChecked(InterActionOpType_SIGNIN, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, false);

         mpInteractionWdg->SetItemChecked(InterActionOpType_TESTPAPER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, false);

      }
   }
                                    break;
   case   InterActionOpType_TESTPAPER: {//考试

      ui.btnInteractionTool->setChecked(false);
      ui.btnInteractionTool2->setChecked(false);

      setAnserChecked(false);
      SetBtnAnswerEnabled(false);

      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_database, false);
         mpMediaWdg->setItemEnabled(OpMediaType_database, false);
      }

      if (nullptr != mpInteractionWdg)
      {
         mpInteractionWdg->SetItemChecked(InterActionOpType_ANSWER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, false);

         mpInteractionWdg->SetItemChecked(InterActionOpType_SIGNIN, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, false);

         mpInteractionWdg->SetItemChecked(InterActionOpType_TESTPAPER, true);
         mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, true);

      }
   }
                                    break;
   case   InterActionOpType_TIMER: {//倒计时

      ui.btnInteractionTool->setChecked(false);
      ui.btnInteractionTool2->setChecked(false);

      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_database, false);
         mpMediaWdg->setItemEnabled(OpMediaType_database, false);
      }

      if (nullptr != mpInteractionWdg)
      {
         mpInteractionWdg->SetItemChecked(InterActionOpType_TIMER, true);
         mpInteractionWdg->setItemEnabled(InterActionOpType_TIMER, true);
      }
   }
                                   break;
   default: break;
   }

}

void LeftToolBarWdg::PreviewAuxiliaryCamera(QEvent*event)
{
   if (nullptr != mpAuxiliaryWdg) {
      mpAuxiliaryWdg->PreviewAuxiliaryCamera(event);
   }
}

void LeftToolBarWdg::EnablePreviewAuxiliary()
{
   if (nullptr != mpAuxiliaryWdg) {
      mpAuxiliaryWdg->EnableUI();
   }
}

bool LeftToolBarWdg::isPreViewAuxiliaryVisible()
{
   bool bRef = false;
   if (nullptr != mpAuxiliaryWdg) {
      bRef = mpAuxiliaryWdg->isVisible();
   }
   return bRef;
}

void LeftToolBarWdg::ShowPreViewAuxiliaryWdg()
{
   if (nullptr == mpAuxiliaryWdg) {
      mpAuxiliaryWdg = new AuxiliaryCameraChoiceWdg(this);
      //connect();
   }

   if (nullptr != mpAuxiliaryWdg) {
      CSTDMapParam mapParam;
      mpAuxiliaryWdg->Notice(mapParam);
      mpAuxiliaryWdg->show();

      StartOvertimeTimer();
   }
}

void LeftToolBarWdg::StartOvertimeTimer()
{
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (JOIN_ROLE_Student == baseInfo.mUserInfo.role_type) {//如果是学员 开启倒计时
      if (nullptr == mpOvertimeTimer) {
         mpOvertimeTimer = new QTimer(this);
         connect(mpOvertimeTimer, &QTimer::timeout, this, &LeftToolBarWdg::slotOvertime);
      }
      if (nullptr != mpOvertimeTimer) {
         mpOvertimeTimer->start(10000);
      }
   }
}

void  LeftToolBarWdg::StartMadiePlayTimer()
{
   CreateVideoChoiceUi();
   if (nullptr != mpVideoChoiceUi) {
      mpVideoChoiceUi->StartMeadiePlayTime();
   }
}

void LeftToolBarWdg::stopTimer()
{
   if (nullptr != mpOvertimeTimer && mpOvertimeTimer->isActive()) {
      mpOvertimeTimer->stop();
   }
}

void LeftToolBarWdg::slotOvertime()
{
   stopTimer();
   if (nullptr != mpAuxiliaryWdg) {
      mpAuxiliaryWdg->hide();
   }

   if (nullptr != mpScreenChoiceDlg && mpScreenChoiceDlg->isVisible()) {
      mpScreenChoiceDlg->on_btnClose_clicked();
   }

}

void LeftToolBarWdg::setInterActionItemEnabled(const bool& enabled)
{
   if (nullptr != mpInteractionWdg)//不包含  计时器、问答
   {
      //mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, enabled);
      mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, enabled);
      mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, enabled);
   }
}

void LeftToolBarWdg::slot_InteractionItemClicked(const int& iOpType)
{
   if (InterActionOpType_SIGNIN == iOpType)
   {
      CreateSignInDlg();
      if (nullptr != mpSignInDlg)
      {
         CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
         WebRtcLiveWdg* webmain = (WebRtcLiveWdg*)tips;

         mpSignInDlg->move((webmain->width() - mpSignInDlg->width()) / 2 + webmain->x(), (webmain->height() - mpSignInDlg->height()) / 2 + webmain->y());
         mpSignInDlg->show();
      }
   }
   else if (InterActionOpType_ANSWER == iOpType){
      slot_answerTool();
   }
   else if (InterActionOpType_TESTPAPER == iOpType) {
	   /*CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(CHOOSE_TEST_PAPER_DLG);
	   if (nullptr != pWdg) {
		   pWdg->show();
	   }*/
      ShowCEFWebTestView();
   }
   else
      emit sigInteractionItemClicked(iOpType);

   setMutexItemChecked((OpType)iOpType);
   //if (/*InterActionOpType_QUESANDANS!= iOpType &&*/ InterActionOpType_TIMER!= iOpType)
   //{
   //	setMutexItemChecked((OpType)iOpType);
   //}
}

void LeftToolBarWdg::ShowCEFWebTestView() {

   //if (nullptr == mpShadowBoxDlg) {
   //   mpShadowBoxDlg = new ShadowBoxDlg(this);
   //}

   if (mpTestPaper == nullptr) {
      mpTestPaper = new ChooseTestPaperDlg(this);
      connect(mpTestPaper, &ChooseTestPaperDlg::sig_close, this, &LeftToolBarWdg::slot_closeTestPaper);

      //mpbTemp = new BaiduPaperDlg(this);
   }
   //mpShadowBoxDlg->show();
   mpTestPaper->show();
   //mpbTemp->show();
}

void LeftToolBarWdg::StuStartExamination()
{
   ShowCEFWebTestView();
}

void LeftToolBarWdg::slot_closeTestPaper()
{
   slot_MutexWdgClose();
   if (mpTestPaper != nullptr) {
      delete mpTestPaper;
      mpTestPaper = nullptr;
   }
}

void LeftToolBarWdg::on_btnDoublePushCamera_clicked(bool checked /*= false*/) {
   //if (!mpWebRtcLiveWdg->CheckAndStopScreenShare(DoublePubStuChoiceUI::eChoiceType_Camera)) {
   //	return;
   //}
   if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
   {
      mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"正在本地视频插播，请结束视频插播后重试！"), 0);
      return;
   }

   SetDoublePushChecked(true);

   bool bIsPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);
   TipsWdg* tips = new TipsWdg(this);
   if (tips) {
      CSTDMapParam param;
      param[TipsMsg] = QStringLiteral("是否确认结束双推?");
      param[ShowCancelButton] = PARAM_TRUE;
      tips->Notice(param);
   }

   if (bIsPushing)//正在本地双推
   {
      int accept = tips->exec();
      if (accept == QDialog::Accepted) {
         emit sig_CameraDoublePush(false);
         //emit sig_stopStuDoublePush();
      }

   }
   else if (globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture) //学员在双推
      || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop))
   {

      int accept = tips->exec();
      if (accept == QDialog::Accepted) {
         emit sig_stopStuDoublePush();
      }

   }
   else if (ui.btnDoublePushCamera2->text().trimmed().compare(CLOSEDOUBLECAMERA) == 0)
   {
      //ui.btnDoublePushCamera->setChecked(false);
      //ui.btnDoublePushCamera2->setChecked(false);
      ////mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);
      int accept = tips->exec();
      if (accept == QDialog::Accepted) {
         emit sigStopReDoublePush();
      }

   }
   else
   {
      //emit sigStopReDoublePush();
      mpDoublePushWdg->SetItemChecked(OpType_TeacherDoublCamera);
      mpDoublePushWdg->SetItemChecked(OpType_StuDoublCamera);

      //ui.btnDoublePushCamera2->setChecked(false);
      //ui.btnDoublePushCamera->setChecked(false);
      //SetDoublePushChecked(false);
      emit sig_DoublePush();
   }

}


void LeftToolBarWdg::on_btnTimerCountDown_clicked(bool checked /*= false*/)
{
   emit sig_TimerCountDown();
}

void LeftToolBarWdg::on_btnWhiteboard_clicked(bool checked /*= false*/) {
   if (mpWebRtcLiveWdg->HasPenAuth()) {
      SetOpType(OpType_Whiteboard);
      emit sig_Whiteboard(checked);
   }
   else if(OpType_Whiteboard!=mCurType){
      ui.btnWhiteboard->setChecked(false);
      ui.btnWhiteboard2->setChecked(false);
      mpWebRtcLiveWdg->FadeOutTip(Str_PleaseBackPenAuth, Tost_Warning);
   }
}

void LeftToolBarWdg::on_btnDesktopSharing_clicked(bool checked /*= false*/) {

   if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
   {
      mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"正在本地视频插播，请结束视频插播后重试！"), 0);
      return;
   }

   //if (!mpWebRtcLiveWdg->CheckAndStopScreenShare(DoublePubStuChoiceUI::eChoiceType_Deskop)) {
   //	return;
   //}

   SetDesktoSharingChecked(true);
   bool isPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Desktop)
      | globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_SoftWare);

   if (isPushing) {//讲师桌面共享/软件共享
      emit  sigStopScreenSharing();
   }
   else if (ui.btnDesktopSharing2->text().trimmed().compare(STOPSHAR) == 0)
   {
      TipsWdg* tips = new TipsWdg(this);
      if (tips) {
         CSTDMapParam param;
         param[TipsMsg] = QStringLiteral("是否确认结束共享?");
         param[ShowCancelButton] = PARAM_TRUE;
         tips->Notice(param);
      }
      int accept = tips->exec();
      if (accept == QDialog::Accepted) {
         emit sigStopReDoublePush();
         //emit sig_stopStuDoublePush();
         emit sig_stopStuDestopSharingPush();
      }

   }
   else
   {
      bool isStuPushing = globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop);
      if (isStuPushing) {//学员桌面共享
         //emit sig_stopStuDoublePush();
         emit sig_stopStuDestopSharingPush();
      }
      else {
         /*ui.btnDesktopSharing2->setChecked(true);
         ui.btnDesktopSharing->setChecked(true);*/
         SetDesktoSharingChecked(true);
         emit sig_DesktopSharing();
      }
   }
}

void LeftToolBarWdg::on_btnInteractionTool_clicked(bool checked /*= false*/) {
   ui.btnInteractionTool2->setChecked(true);
   ui.btnInteractionTool->setChecked(true);
   emit sig_InteractionTool();
}

void LeftToolBarWdg::PopInteractionToolMenu(const QPoint& temppos)
{

   if (NULL != mpInteractionWdg)
   {
      QPoint pos = mapToGlobal(/*temppos */ui.btnInteractionTool->pos());
      int iX = pos.x();
      int iY = pos.y();

      QRect  rect = ui.btnInteractionTool->geometry();
      qDebug() << iX << iY << ui.btnInteractionTool->width() << rect.x() << rect.y() << rect.width() << rect.height();

      mpInteractionWdg->show(iX + 0.5*rect.width() + 0.5*width(), iY /* rect.height()*/);
   }
}

void LeftToolBarWdg::PopDoublePushMenu(const QPoint& temppos)
{
   if (nullptr != mpDoublePushWdg)
   {
      QPoint pos = mapToGlobal(ui.DoublePushCameraFrame->pos());

      int iX = pos.x();
      int iY = pos.y();

      QRect  rect = ui.DoublePushCameraFrame->geometry();
      //qDebug() << iX << iY << ui.btnDesktopSharing->width() << rect.x() << rect.y() << rect.width() << rect.height();
      mpDoublePushWdg->show(iX + 0.5*rect.width() + 0.5*width(), iY);
   }
}

void LeftToolBarWdg::slots_MediaMenuAboutToHide()
{
   ui.btnMedia->setChecked(false);
   ui.btnMedia2->setChecked(false);
}

void LeftToolBarWdg::slots_DoublePushToHide()
{
   //ui.btnDoublePushCamera->setChecked(false);
   //ui.btnDoublePushCamera2->setChecked(false);
   SetDoublePushChecked(false);
}

void LeftToolBarWdg::on_btnMedia_clicked(bool checked/* = false*/)
{
   //SetOpType(OpType_MediaMenu);
   ui.btnMedia2->setChecked(true);
   ui.btnMedia->setChecked(true);
   //SetOpType(OpType_MediaMenu);
   emit sig_Media();
}

void LeftToolBarWdg::PopDesktopSharingMenu(const QPoint& temppos)
{
   if (nullptr != mpDesktopSharWdg)
   {
      QPoint pos = mapToGlobal(/*temppos */ui.btnDesktopSharing->pos());
      int iX = pos.x();
      int iY = pos.y();

      QRect  rect = ui.btnDesktopSharing->geometry();
      qDebug() << iX << iY << ui.btnDesktopSharing->width() << rect.x() << rect.y() << rect.width() << rect.height();
      mpDesktopSharWdg->show(iX + 0.5*rect.width() + 0.5*width(), iY /* rect.height()*/);
   }
}

bool LeftToolBarWdg::IsExitAnserWebView()
{
   bool bExet = false;
   if (nullptr != mpAnswerWebView)
      bExet = mpAnswerWebView->isActiveWindow() || mpAnswerWebView->isVisible();
   return bExet;
}

bool LeftToolBarWdg::IsTestPaperDlg()
{
   bool bExet = false;
   if (nullptr != mpTestPaper)
      bExet = mpTestPaper->isActiveWindow() || mpTestPaper->isVisible();
   return bExet;
}

bool LeftToolBarWdg::IsExitSignInDlg() {
   bool bExet = false;
   if (nullptr != mpSignInDlg)
      bExet = mpSignInDlg->isActiveWindow() || mpSignInDlg->isVisible();
   return bExet;
}

void LeftToolBarWdg::AddOnline(const int& membercount)
{
   mTotal = membercount;
   if (nullptr != mpSignInDlg)
   {
      mpSignInDlg->AddonlineCount(membercount);
   }
   //答题在线总人数实时刷新人数
   CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
   if (wnd->isVisible() && wnd->isActiveWindow()) {
      CSTDMapParam mapParam;
      mapParam[Str_OnLineNum] = QString::number(membercount);
      wnd->Notice(mapParam);
   }

}

void LeftToolBarWdg::createDoubleStuChoiceUi()
{
   if (nullptr == mpDoublePubStuChoiceUI)
   {
      mpDoublePubStuChoiceUI = new DoublePubStuChoiceUI(this);
      connect(mpDoublePubStuChoiceUI, &DoublePubStuChoiceUI::sig_inviteDoubleVrtc, this, &LeftToolBarWdg::slot_startInviteTimerOut);
      connect(mpDoublePubStuChoiceUI, &DoublePubStuChoiceUI::sig_inviteDoubleVrtc, this, &LeftToolBarWdg::sig_inviteDoubleVrtc);

   }
}

void LeftToolBarWdg::slot_startInviteTimerOut()
{
   if (nullptr == mpInviteDoublePubTimer) {
      mpInviteDoublePubTimer = new QTimer(this);
      connect(mpInviteDoublePubTimer, &QTimer::timeout, this, &LeftToolBarWdg::slot_InviteStuDoublePubTimerOut);
   }

   if (nullptr != mpInviteDoublePubTimer) {
      mpInviteDoublePubTimer->start(10000);
      mpDoublePushWdg->setItemEnabled(OpType_StuDoublCamera, false);
   }

   if (nullptr != mpDoublePubStuChoiceUI) {
      mpDoublePubStuChoiceUI->hide();
   }

}

void LeftToolBarWdg::slot_InviteStuDoublePubTimerOut()
{
   mpDoublePushWdg->setItemEnabled(OpType_StuDoublCamera, true);
}

void LeftToolBarWdg::DoubleVrtcUsers(const CDoubleVrtcUsers& vrtcUser)
{
   createDoubleStuChoiceUi();
   if (nullptr != mpDoublePubStuChoiceUI) {
      mpDoublePubStuChoiceUI->AddUser(vrtcUser);
   }
}

void LeftToolBarWdg::CloseVrtcUsersDlg()
{
   if (nullptr != mpDoublePubStuChoiceUI && mpDoublePubStuChoiceUI->isVisible()) {
      mpDoublePubStuChoiceUI->on_btnClose_clicked();
   }
}

void LeftToolBarWdg::EnableStuDoubleVrtc(const bool bEnalbe)
{
   if (nullptr != mpDoublePushWdg) {
      mpDoublePushWdg->setItemEnabled(OpType_StuDoublCamera, bEnalbe);
   }
}

void LeftToolBarWdg::initDoubleVrtcUsers(const DoublePubStuChoiceUI::eChoiceType& type)
{
   if (nullptr != mpDoublePubStuChoiceUI) {
      mpDoublePubStuChoiceUI->initItem(type);
   }
}

void LeftToolBarWdg::PopMediaMenu(const QPoint& temppos)
{
   //SetOpType(OpType_NULL);
   if (nullptr != mpMediaWdg)
   {
      if (IsExitAnserWebView() || IsExitSignInDlg() || IsTestPaperDlg() /*globalToolManager->GetWndManager()->IsWndExist(WND_ID_QUESTION_UI)*/) {//
         mpMediaWdg->setItemEnabled(OpMediaType_database, false);
      }
      else
      {
         mpMediaWdg->setItemEnabled(OpMediaType_database, true);
      }

      QPoint pos = mapToGlobal(/*temppos */ ui.btnMedia->pos());

      int iX = pos.x();
      int iY = pos.y();

      QRect  rect = ui.btnMedia->geometry();
      //qDebug() << iX << iY << ui.btnDesktopSharing->width() << rect.x() << rect.y() << rect.width() << rect.height();
      mpMediaWdg->show(iX + 0.5*rect.width() + 0.5*width(), iY /* rect.height()*/);
      //SetOpType(OpMediaType_Local);
   }
}

void LeftToolBarWdg::on_btnAnswer_clicked(bool checked /*= false*/) {
   setMutexItemChecked(OpType_Anser);
   emit sig_Answer();
   //SetOpType(OpType_Anser);
}

void LeftToolBarWdg::slot_answerTool() {
   bool exist = globalToolManager->GetWndManager()->IsWndExist(WND_ID_ANSWER_TOOL);
   if (exist) {
      return;
   }
   CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
   if (wnd) {
      CSTDMapParam mapParam;
      mapParam[Str_Function] = Str_Function_AnswerToolTeacher;
      mapParam[Str_OnLineNum] = QString::number(mpWebRtcLiveWdg->GetOnlineTotal());
      wnd->setParent(this);
      wnd->Notice(mapParam);
      wnd->show();
   }
}

void LeftToolBarWdg::requestVrtcUsers(const DoublePubStuChoiceUI::eChoiceType& type)
{
   bool bIsPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);
   if (bIsPushing)//正在本地双推
   {
      emit sig_CameraDoublePush(false);
   }
   else if (globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture) //学员在双推
      || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop))
   {
      emit sig_stopStuDoublePush();
   }

   if (mpWebRtcLiveWdg->CheckAndStopScreenShare(type))
   {
      createDoubleStuChoiceUi();
      mDoublePushType = type;
      if (nullptr != mpDoublePubStuChoiceUI) {
         mpDoublePubStuChoiceUI->initItem(type);
         //mpDoublePubStuChoiceUI->show();
      }
      emit sig_StudentDoublePush();
   }
}

void LeftToolBarWdg::slot_StopClass() {
   //下课前判断插播、答题、答题器、签到、倒计时页面是否开启，若开启 提示关闭
   if (mpVideoChoiceUi && mpVideoChoiceUi->isVisible()) {
      mpWebRtcLiveWdg->FadeOutTip(QStringLiteral("请先结束插播"), Tost_Warning);
   }
   else if (mpSignInDlg && mpSignInDlg->isVisible()) {
      mpWebRtcLiveWdg->FadeOutTip(QStringLiteral("请先结束签到"), Tost_Warning);
   }
   else if (mpMediaPlayWebView && mpMediaPlayWebView->isVisible()) {
      mpWebRtcLiveWdg->FadeOutTip(QStringLiteral("请先结束插播"), Tost_Warning);
   }
   else if (mpAnswerWebView && mpAnswerWebView->isVisible()) {
      mpWebRtcLiveWdg->FadeOutTip(QStringLiteral("请先结束答题"), Tost_Warning);
   }
   else if (mpWebRtcLiveWdg && mpWebRtcLiveWdg->IsTimerWdgVisable()) {
      mpWebRtcLiveWdg->FadeOutTip(QStringLiteral("请先结束倒计时"), Tost_Warning);
   }
   else if (globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL)
      && globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL)->isVisible()) {
      mpWebRtcLiveWdg->FadeOutTip(QStringLiteral("请先结束答题器"), Tost_Warning);
   }
   else {
      emit sig_StopClass();
   }
}

void LeftToolBarWdg::slot_ShowDoublePubStuChoiceUI() {
   if (mpDoublePubStuChoiceUI) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (mbRequestByManual && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         mbRequestByManual = false;
         int iRef = mpDoublePubStuChoiceUI->exec();
         if (QDialog::Rejected == iRef) {
            if (nullptr != mpWebRtcLiveWdg) {
               mpWebRtcLiveWdg->slot_showInitPage();
               if (mpDoublePubStuChoiceUI->iRowCount() <= 0) {
                  mpWebRtcLiveWdg->FadeOutTip(Str_NoVrtcStudent, Tost_Warning);
               }
            }
         }
      }
   }
}

void LeftToolBarWdg::slot_InvitaStateChanged(const InvitationStateWdg::eStateType& type)
{
   bool bEnable = false;
   if (InvitationStateWdg::eStateType_FrameGeting == type) {
      bEnable = false;
   }
   else {
      bEnable = true;
   }

   switch (mCurType)
   {
   case LeftToolBarWdg::OpType_Doc:
      break;
   case LeftToolBarWdg::OpType_Whiteboard:
      break;
   case LeftToolBarWdg::OpType_DesktopShar:
      break;
   case LeftToolBarWdg::OpType_SoftWareShar:
      break;
   case LeftToolBarWdg::OpType_MediaMenu:
      break;
   case LeftToolBarWdg::OpType_Anser:
      break;
   case LeftToolBarWdg::OpType_Interaction:
      break;
   case LeftToolBarWdg::OpType_TeacherDoublCamera:
   case LeftToolBarWdg::OpType_StuDoublCamera:
      SetDoublePushCameraEnabled(bEnable);
      break;
   case LeftToolBarWdg::OpType_StuDesktopShar:
      SetBtnDesktopSharingEnabled(bEnable);
      break;
   case LeftToolBarWdg::OpMediaType_database:
      break;
   case LeftToolBarWdg::OpMediaType_Local:
      break;
   case LeftToolBarWdg::InterActionOpType_SIGNIN:
      break;
   case LeftToolBarWdg::InterActionOpType_ANSWER:
      break;
   case LeftToolBarWdg::InterActionOpType_TIMER:
      break;
   case LeftToolBarWdg::OpType_NULL:
      break;
   default:
      break;
   }
}

void LeftToolBarWdg::slot_doublePush(const int& iOptype) {
   globalToolManager->GetDataManager()->WriteLog("%s  iOptype %d Enter ", __FUNCTION__, iOptype);
   SetDoublePushChecked(true);
   if (OpType_TeacherDoublCamera == iOptype)//讲师双推
   {
      //bool bIsPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);
      //ui.btnDoublePushCamera2->setChecked(true);
      //ui.btnDoublePushCamera->setChecked(true);
      emit sig_CameraDoublePush(true);
   }
   else//学员双推
   {
      mDoubleType = eDoubleType_StuDouble;
      mbRequestByManual = true;
      StuDoublePush();
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void LeftToolBarWdg::StuDoublePush()
{
   bool bIsSubScribeStream = globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_AVCapture);
   bool bIsExitRender = mpWebRtcLiveWdg->IsExitRender();
   if (bIsSubScribeStream || bIsExitRender) {
      mpWebRtcLiveWdg->FadeOutTip(Str_StudentMaybeCant_DoublePush, Tost_Warning);
      requestVrtcUsers(DoublePubStuChoiceUI::eChoiceType_Camera);
   }
   else {
      SetDoublePushChecked(false);
      //mpWebRtcLiveWdg->FadeOutTip(Str_NoVrtcStudent, Tost_Warning);
      mpWebRtcLiveWdg->FadeOutTip(Str_NoScribeStudent_DoublePush, Tost_Warning);
   }
}

bool LeftToolBarWdg::IsLocalMediaPlaying()
{
   return (mpVideoChoiceUi && mpVideoChoiceUi->isVisible());
}

void LeftToolBarWdg::SetBtnDocEnabled(bool checked /*= true*/) {
   ui.btnDoc->setEnabled(checked);
   ui.btnDoc2->setEnabled(checked);
}

void LeftToolBarWdg::SetBtnWhiteboardEnabled(bool checked /*= true*/) {
   ui.btnWhiteboard->setEnabled(checked);
   ui.btnWhiteboard2->setEnabled(checked);
}

void LeftToolBarWdg::SetBtnDesktopSharingEnabled(bool checked /*= true*/) {
   if (checked) {
      if (mpVideoChoiceUi && mpVideoChoiceUi->isVisible()) {
         checked = false;
      }
   }

   ui.btnDesktopSharing->setEnabled(checked);
   ui.btnDesktopSharing2->setEnabled(checked);
}

void LeftToolBarWdg::SetDoublePushCameraEnabled(bool checked /*= true*/) {
   if (checked) {
      if (mpVideoChoiceUi && mpVideoChoiceUi->isVisible()) {
         checked = false;
      }
   }

   ui.btnDoublePushCamera->setEnabled(checked);
   ui.btnDoublePushCamera2->setEnabled(checked);
}

void LeftToolBarWdg::SetBtnMediaEnabled(bool checked /*= true*/) {
   ui.btnMedia->setEnabled(checked);
   ui.btnMedia2->setEnabled(checked);
   slots_MediaMenuAboutToHide();
}

void LeftToolBarWdg::SetBtnAnswerEnabled(bool checked /*= true*/) {
   ui.btnAnswer->setEnabled(checked);
   ui.btnAnswer2->setEnabled(checked);
}

void LeftToolBarWdg::setAnserChecked(const bool& bChecked /*= true*/)
{
   ui.btnAnswer->setChecked(bChecked);
   ui.btnAnswer2->setChecked(bChecked);
}

void LeftToolBarWdg::StartSignin(const int& iTime, const QString& strSignInId)
{
   if (nullptr == mpSignInDlg)
   {
      CreateSignInDlg();
      if (nullptr != mpSignInDlg)
      {
         CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
         WebRtcLiveWdg* webmain = (WebRtcLiveWdg*)tips;

         mpSignInDlg->move((webmain->width() - mpSignInDlg->width()) / 2 + webmain->x(), (webmain->height() - mpSignInDlg->height()) / 2 + webmain->y());
         mpSignInDlg->show();
      }
   }
   mpSignInDlg->startSignIn(iTime, strSignInId);
   mpSignInDlg->AddonlineCount(mpWebRtcLiveWdg->GetOnlineTotal() - 1);
}

void LeftToolBarWdg::SomeOneSignin(const QString& strSigner, const QString& strNick)
{
   if (nullptr != mpSignInDlg)
   {
      mpSignInDlg->SomeOneSignin(strSigner, strNick);
   }
}

void LeftToolBarWdg::SetMuteSelf(const bool& bMute)
{
   CreateVideoChoiceUi();
   if (nullptr != mpVideoChoiceUi)
   {
      mpVideoChoiceUi->SetMuteSelf(bMute);
   }
}

void LeftToolBarWdg::SetMuteStudent(const bool& bMute)
{
   CreateVideoChoiceUi();
   if (nullptr != mpVideoChoiceUi)
   {
      mpVideoChoiceUi->SetMuteStudent(bMute);
   }
}
void LeftToolBarWdg::RecvSignUsers(std::list<std::string> signUsers, std::list<std::string> noSignUsers)
{
   CreateSignInDlg();
   if (nullptr != mpSignInDlg)
   {
      mpSignInDlg->clearList();
      mpSignInDlg->addSignUsers(SignInListItem::eSignInType_SignIn, signUsers);
      mpSignInDlg->addSignUsers(SignInListItem::eSignInType_UnSign, noSignUsers);
      mpSignInDlg->SetSignInfo(signUsers.size() + noSignUsers.size(), signUsers.size());
   }
}

bool LeftToolBarWdg::isMediaPlayExist()
{
   bool bExist = false;
   //if (nullptr != mpVideoChoiceUi)
   //{
   //	bExist |= (mpVideoChoiceUi->isActiveWindow() || mpVideoChoiceUi->isVisible());
   //}

   if (nullptr != mpMediaPlayWebView)
   {
      bExist |= (mpMediaPlayWebView->isActiveWindow() || mpMediaPlayWebView->isVisible());
   }

   return bExist;
}

bool LeftToolBarWdg::isVideoChoiceUiExist()
{
   bool bExist = false;
   //if (nullptr != mpVideoChoiceUi)
   //{
   //	bExist |= (mpVideoChoiceUi->isActiveWindow() || mpVideoChoiceUi->isVisible());
   //}

   if (nullptr != mpVideoChoiceUi)
   {
      bExist |= (mpVideoChoiceUi->isActiveWindow() || mpVideoChoiceUi->isVisible());
   }

   return bExist;
}

void LeftToolBarWdg::slots_MenuAboutToHide()
{
   //ui.btnDesktopSharing->setChecked(false);
   //ui.btnDesktopSharing2->setChecked(false);
   if (nullptr != mpsoftwareSourceDlg && mpsoftwareSourceDlg->isVisible()) {
      return;
   }
   SetDesktoSharingChecked(false);
}

void LeftToolBarWdg::slots_InteractionHide()
{
   ui.btnInteractionTool->setChecked(false);
   ui.btnInteractionTool2->setChecked(false);
}

void LeftToolBarWdg::slot_OverClass()
{
   if (nullptr != mpVideoChoiceUi && mpVideoChoiceUi->isVisible())
   {
      mpVideoChoiceUi->CloseWdg();
   }

   //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_MEDIA_PLAY_WEBVIEW);	

   if (mpMediaPlayWebView && mpMediaPlayWebView->isVisible()) {
      CSTDMapParam param;
      param[CLOSE_MEDIA_FILE] = "";
      mpMediaPlayWebView->Notice(param);
   }

}

void LeftToolBarWdg::slots_DesktopSharing(const int& iOptype)
{
   if (iOptype == OpDesktopSharType_DesktopShar)//桌面共享
   {
      SetShareType(eDestopType_Desktop);
   }
   else if (iOptype == OpDesktopSharType_SoftwareShar)//软件共享
   {
      SetShareType(eDestopType_Windows);
   }
   else if (iOptype == OpDesktopSharType_StuDesktopShar)//学员共享
   {
      mDoubleType = eDoubleType_StuScreen;
      SetShareType(eDestopType_StuDesktop);

   }
   slots_ScreenSharing();
}

QString LeftToolBarWdg::GetCurShareSoftware()
{
   return mstrCurShareSoftware;
}

void LeftToolBarWdg::SetBtnDesktopSharingText(const QString& str, const bool& bSorftWare /*= false*/)
{
   if (str.compare(DESKTOPSHAR) == 0) {
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_DesktopShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_SoftwareShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);

      //ui.btnDesktopSharing->setChecked(false);
      //ui.btnDesktopSharing2->setChecked(false);
      if (!bSorftWare) {
         SetDesktoSharingChecked(false);
      }

   }
   ui.btnDesktopSharing2->setText(str);
   ui.btnDesktopSharing2->setToolTip(str);
   ui.btnDesktopSharing->setToolTip(str);
}

void LeftToolBarWdg::SetDesktoSharingChecked(const bool& bChecked /*= true*/)
{
   ui.btnDesktopSharing->setChecked(bChecked);
   ui.btnDesktopSharing2->setChecked(bChecked);
   //if (!bChecked)
   //   int i = 0;
}

void LeftToolBarWdg::SetDoublePushChecked(const bool& bChecked /*= true*/)
{
   ui.btnDoublePushCamera->setChecked(bChecked);
   ui.btnDoublePushCamera2->setChecked(bChecked);
   if (!bChecked)
      int a = 0;
}

int LeftToolBarWdg::CurOptype()
{
   return mCurType;
}

void LeftToolBarWdg::SetOpType(OpType type)
{
   mCurType = type;
   //ui.btnDesktopSharing2->setText(DESKTOPSHAR);
   //ui.btnDesktopSharing->setToolTip(DESKTOPSHAR);
   switch (type)
   {
   case LeftToolBarWdg::OpType_Doc:  //文档
   {
      ui.btnDoublePushCamera2->setText(DOUBLECAMERA);
      SetDoubleCamera(false);
      ui.btnDoc->setChecked(true);
      ui.btnDoc2->setChecked(true);

      ui.btnWhiteboard->setChecked(false);
      ui.btnWhiteboard2->setChecked(false);

      globalToolManager->GetClassSDK()->ChangeDocMode(0);
      /*ui.btnDesktopSharing->setChecked(false);
      ui.btnDesktopSharing2->setChecked(false);*/
      SetDesktoSharingChecked(false);

      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_DesktopShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_SoftwareShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);

      /*    ui.btnDoublePushCamera->setChecked(false);
          ui.btnDoublePushCamera2->setChecked(false);*/
      SetDoublePushChecked(false);
      SetBtnDesktopSharingText(DESKTOPSHAR);
   }
   break;
   case LeftToolBarWdg::OpType_Whiteboard://白板
   {
      ui.btnDoublePushCamera2->setText(DOUBLECAMERA);
      SetDoubleCamera(false);
      ui.btnDoc->setChecked(false);
      ui.btnDoc2->setChecked(false);
      globalToolManager->GetClassSDK()->ChangeDocMode(1);
      ui.btnWhiteboard->setChecked(true);
      ui.btnWhiteboard2->setChecked(true);

      /*ui.btnDesktopSharing->setChecked(false);
      ui.btnDesktopSharing2->setChecked(false);*/
      SetDesktoSharingChecked(false);

      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_DesktopShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_SoftwareShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);

      /* ui.btnDoublePushCamera->setChecked(false);
       ui.btnDoublePushCamera2->setChecked(false);*/
      SetDoublePushChecked(false);
      SetBtnDesktopSharingText(DESKTOPSHAR);
   }
   break;
   case LeftToolBarWdg::OpType_DesktopShar: //桌面共享
   {
      ui.btnDoublePushCamera2->setText(DOUBLECAMERA);
      SetDoubleCamera(false);
      ui.btnDoc->setChecked(false);
      ui.btnDoc2->setChecked(false);

      ui.btnWhiteboard->setChecked(false);
      ui.btnWhiteboard2->setChecked(false);

      /*ui.btnDesktopSharing->setChecked(true);
      ui.btnDesktopSharing2->setChecked(true);*/
      SetDesktoSharingChecked(true);

      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_DesktopShar, true);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_SoftwareShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);

      //ui.btnDoublePushCamera->setChecked(false);
      //ui.btnDoublePushCamera2->setChecked(false);
      SetDoublePushChecked(false);
      SetBtnDesktopSharingText(DESKTOPSHAR);
   }
   break;
   case OpType_SoftWareShar://软件共享
   {

      ui.btnDoublePushCamera2->setText(DOUBLECAMERA);
      SetDoubleCamera(true);
      /*ui.btnDoc->setChecked(false);
      ui.btnDoc2->setChecked(false);

      ui.btnWhiteboard->setChecked(false);
      ui.btnWhiteboard2->setChecked(false);*/

      /*ui.btnDesktopSharing->setChecked(true);
      ui.btnDesktopSharing2->setChecked(true);*/


      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_DesktopShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_SoftwareShar, true);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);

      //ui.btnDoublePushCamera->setChecked(false);
      //ui.btnDoublePushCamera2->setChecked(false);
      SetDoublePushChecked(false);
      SetBtnDesktopSharingEnabled(true);
      //SetDesktoSharingChecked(true);
      //SetBtnDesktopSharingText(STOPSHAR, true);
      SetStuDesktopShare();
   }
   break;
   case OpType_TeacherDoublCamera:  //讲师双推
   case OpType_StuDoublCamera: {     //学员双推
      SetDoubleCamera(true);
      ui.btnDoc->setChecked(false);
      ui.btnDoc2->setChecked(false);

      ui.btnWhiteboard->setChecked(false);
      ui.btnWhiteboard2->setChecked(false);

      //ui.btnDoublePushCamera->setChecked(true);
      //ui.btnDoublePushCamera2->setChecked(true);
      SetDoublePushChecked(true);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);
      SetBtnDesktopSharingText(DESKTOPSHAR);
   }
                               break;
   case OpType_StuDesktopShar: {
      SetStuDesktopShare();
      ui.btnDoc->setChecked(false);
      ui.btnDoc2->setChecked(false);

      ui.btnWhiteboard->setChecked(false);
      ui.btnWhiteboard2->setChecked(false);

      //ui.btnDoublePushCamera->setChecked(true);
      //ui.btnDoublePushCamera2->setChecked(true);
      SetDoublePushChecked(true);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);
   }
                               break;
   case OpType_MediaMenu: break;
   default:
   {
      SetDoubleCamera(false);
      ClassRoomBaseInfo Roominfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      bool bdoc = (0 == Roominfo.mDocInfo.doc_mode);
      ui.btnDoc->setChecked(bdoc);
      ui.btnDoc2->setChecked(bdoc);

      ui.btnWhiteboard->setChecked(!bdoc);
      ui.btnWhiteboard2->setChecked(!bdoc);

      /*ui.btnDesktopSharing->setChecked(false);
      ui.btnDesktopSharing2->setChecked(false);*/
      SetDesktoSharingChecked(false);

      //ui.btnDoublePushCamera->setChecked(false);
      //ui.btnDoublePushCamera2->setChecked(false);
      SetDoublePushChecked(false);

      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_DesktopShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_SoftwareShar, false);
      mpDesktopSharWdg->SetItemChecked(OpDesktopSharType_StuDesktopShar, false);
      SetBtnDesktopSharingText(DESKTOPSHAR);
   }
   break;
   }
}

DoublePubStuChoiceUI::eChoiceType LeftToolBarWdg::GetDoublePushType()
{
   return mDoublePushType;
}

void LeftToolBarWdg::SetStuDesktopShare()
{
   //bool bEanble = !bDouble;
   SetBtnDocEnabled(false);
   SetBtnWhiteboardEnabled(false);

   //ui.btnDoublePushCamera->setEnabled(false);
   //ui.btnDoublePushCamera2->setEnabled(false);
   SetDoublePushCameraEnabled(false);
   SetBtnMediaEnabled(false);
   SetBtnAnswerEnabled(false);

   mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, false);
   mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, false);

   /*ui.btnDesktopSharing->setChecked(true);
   ui.btnDesktopSharing2->setChecked(true);*/
   SetDesktoSharingChecked(true);
   //ui.btnDesktopSharing2->setText(STOPSHAR);
   //ui.btnDesktopSharing2->setToolTip(STOPSHAR);
   //ui.btnDesktopSharing->setToolTip(STOPSHAR);

   SetBtnDesktopSharingText(STOPSHAR);
}

void LeftToolBarWdg::SetDoubleCamera(const bool bDouble)
{
   bool bEanble = !bDouble;
   SetBtnDocEnabled(bEanble);
   SetBtnWhiteboardEnabled(bEanble);
   SetBtnDesktopSharingEnabled(bEanble);

   SetBtnMediaEnabled(bEanble);
   SetBtnAnswerEnabled(bEanble);

   mpInteractionWdg->setItemEnabled(InterActionOpType_SIGNIN, bEanble);
   mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, bEanble);

   //ui.btnDoublePushCamera->setEnabled(true);
   //ui.btnDoublePushCamera2->setEnabled(true);
   SetDoublePushCameraEnabled(true);
   if (bEanble) {//关闭了双推
      //ui.btnDoublePushCamera->setChecked(false);
      //ui.btnDoublePushCamera2->setChecked(false);
      SetDoublePushChecked(false);
      ui.btnDoublePushCamera2->setText(DOUBLECAMERA);
      ui.btnDoublePushCamera->setToolTip(DOUBLECAMERA);
   }
   else {//开启了双推
      SetBtnDesktopSharingText(DESKTOPSHAR);
      //ui.btnDoublePushCamera->setChecked(true);
      //ui.btnDoublePushCamera2->setChecked(true);
      SetDoublePushChecked(true);
      ui.btnDoublePushCamera2->setText(CLOSEDOUBLECAMERA);
      ui.btnDoublePushCamera->setToolTip(CLOSEDOUBLECAMERA);
   }

}

MediaPlayWebView* LeftToolBarWdg::MediaPlayWebViewDlg()
{
   if (nullptr == mpMediaPlayWebView){
      mpMediaPlayWebView = new MediaPlayWebView(this);
   }

   return mpMediaPlayWebView;
}

void LeftToolBarWdg::DestoryMediaPlayWebViewDlg()
{
   if (nullptr != mpMediaPlayWebView && mpMediaPlayWebView->isVisible())
   {
      mpMediaPlayWebView->hide();
      mpMediaPlayWebView->SetLoadFinished(false);
      delete mpMediaPlayWebView;
      mpMediaPlayWebView = nullptr;
   }
}

AnswerWebView* LeftToolBarWdg::AnswerWebViewDlg()
{
   //if (nullptr == mpAnswerShadowBoxDlg) {
   //   mpAnswerShadowBoxDlg = new ShadowBoxDlg(this);
   //}

   if (nullptr == mpAnswerWebView){
      mpAnswerWebView = new AnswerWebView(this);
   }

   return mpAnswerWebView;
}

void LeftToolBarWdg::DestoryAnswerWebViewDlg()
{
   if (nullptr != mpAnswerWebView && mpAnswerWebView->isVisible())
   {

      mpAnswerWebView->hide();
      //mpAnswerShadowBoxDlg->hide();
      mpAnswerWebView->SetLoadFinished(false);

      delete mpAnswerWebView;
      mpAnswerWebView = nullptr;
   }
}

void LeftToolBarWdg::DestoryAnswerToolDlg() {
   CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);

   if (wnd && wnd->isVisible()) {
      wnd->hide();
      wnd->setParent(NULL);
      if (nullptr != mpInteractionWdg)
      {
         mpInteractionWdg->SetItemChecked(InterActionOpType_ANSWER, false);
         mpInteractionWdg->setItemEnabled(InterActionOpType_ANSWER, true);
      }
   }
}

void LeftToolBarWdg::DestoryTimerToolDlg() {
   if (nullptr != mpInteractionWdg)
   {
      mpInteractionWdg->SetItemChecked(InterActionOpType_TIMER, false);
      mpInteractionWdg->setItemEnabled(InterActionOpType_TIMER, true);
   }
}

void LeftToolBarWdg::DestoryTestPaperDlg()
{
	if (nullptr != mpInteractionWdg)
	{
		mpInteractionWdg->SetItemChecked(InterActionOpType_TESTPAPER, false);
		mpInteractionWdg->setItemEnabled(InterActionOpType_TESTPAPER, true);
	}
}

void LeftToolBarWdg::slots_MediaMenu(const int& iOptype)
{
   if (OpMediaType_Local == iOptype)//本地
   {
      localMediaPlay();
   }
   else//资源库
   {
      setMutexItemChecked(OpMediaType_database);
      //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_MEDIA_PLAY_WEBVIEW);
      MediaPlayWebViewDlg();
      if (mpMediaPlayWebView->isActiveWindow() || mpMediaPlayWebView->isVisible()) return;
      if (mpMediaPlayWebView) {
         CSTDMapParam param;
         mpMediaPlayWebView->Notice(param);
         CBaseWnd* mainwnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
         if (mainwnd)
         {

            int ix = (mainwnd->width() - mpMediaPlayWebView->width()) / 2 + mainwnd->x();
            int iy = (mainwnd->height() - mpMediaPlayWebView->height()) / 2 + mainwnd->y();
            mpMediaPlayWebView->move(ix, iy);
         }
         mpMediaPlayWebView->show();
      }
   }
   //SetOpType(OpType_MediaMenu);
}

void LeftToolBarWdg::StopPlayFile()
{
   //判断是否推流中 如是 则停止推流
   if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
   {
      globalToolManager->GetDataManager()->WriteLog("%s :StopPushMediaFileStream", __FUNCTION__);
      globalToolManager->GetClassSDK()->StopPushMediaFileStream();
   }

   //判断是否播放中 如是 则释放播放窗体
   if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_MediaFile))
   {
      globalToolManager->GetDataManager()->WriteLog("%s :StopMediaFileCapture", __FUNCTION__);
      globalToolManager->GetClassSDK()->StopMediaFileCapture();
   }
}

void LeftToolBarWdg::openMic(bool bOpen)
{
   if (!bOpen) {
      globalToolManager->GetClassSDK()->CloseMic();
   }
   else {
      globalToolManager->GetClassSDK()->OpenMic();
      globalToolManager->GetClassSDK()->SetCurrentMicVol(100);
   }
}

void LeftToolBarWdg::localMediaPlay() {
   //emit sig_Media();
   if (nullptr != mpVideoChoiceUi && mpVideoChoiceUi->isMinimized())
   {
      SetDoublePushCameraEnabled(false);
      SetBtnDesktopSharingEnabled(false);
      mpVideoChoiceUi->showNormal();
      return;
   }
   ui.btnMedia->setChecked(false);
   ui.btnMedia2->setChecked(false);
   globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
   bool bPlaying = globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_MediaFile);
   globalToolManager->GetDataManager()->WriteLog("%s :m_bIsMediaPlaying=%d", __FUNCTION__, bPlaying);
   if (bPlaying)
   {
      StopPlayFile();
   }
   else
   {
      if (nullptr != mpMediaWdg)
      {
         mpMediaWdg->SetItemChecked(OpMediaType_Local, true);
      }

      if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_Desktop) || globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Desktop)) {
         mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"桌面共享进行中……"), 0);
         return;
      }

      if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
      {
         mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"视频插播进行中……"), 0);
         return;
      }

      StopPlayFile();

      CreateVideoChoiceUi();
      if (nullptr != mpVideoChoiceUi)
      {
         mpVideoChoiceUi->UpdatePlayStates(false);
         CBaseWnd* mainwnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
         if (mainwnd)
         {
            mpVideoChoiceUi->move((mainwnd->width() - mpVideoChoiceUi->width()) / 2 + mainwnd->x(), (mainwnd->height() - mpVideoChoiceUi->height()) / 2 + mainwnd->y());
         }
         SetDoublePushCameraEnabled(false);
         SetBtnDesktopSharingEnabled(false);
         mpVideoChoiceUi->show();
      }
   }
}

void LeftToolBarWdg::CreateVideoChoiceUi()
{
   if (nullptr == mpVideoChoiceUi)
   {
      mpVideoChoiceUi = new VideoChoiceUI(this);
      connect(mpVideoChoiceUi, &VideoChoiceUI::sigMuteSelf, this, &LeftToolBarWdg::sigMuteSelf);
      connect(mpVideoChoiceUi, &VideoChoiceUI::sig_close, this, &LeftToolBarWdg::slot_VideoClose);

      CSTDMapParam param;
      param[INIT] = INIT;
      mpVideoChoiceUi->Notice(param);
   }
}

void LeftToolBarWdg::slot_VideoClose()
{
   if (nullptr != mpMediaWdg)
   {
      mpMediaWdg->SetItemChecked(OpMediaType_Local, false);
   }
   SetDoublePushCameraEnabled(true);
   SetBtnDesktopSharingEnabled(true);
}

void LeftToolBarWdg::DealTopplayChoiceUi()
{
   if (nullptr != mpVideoChoiceUi)
   {
      CSTDMapParam mapParam;
      mapParam[DEALSTOPPLAY] = "";
      mpVideoChoiceUi->Notice(mapParam);
   }
}

void LeftToolBarWdg::slots_ScreenSharing()
{
   if (eDestopType_Windows == miShareType)
   {
      //softwareSourceDlg dlg(this);
      if (nullptr == mpsoftwareSourceDlg) {
         mpsoftwareSourceDlg = new softwareSourceDlg(this);
      }

      if (nullptr != mpsoftwareSourceDlg && QDialog::Accepted == mpsoftwareSourceDlg->exec())
      {
         mstrCurShareSoftware = mpsoftwareSourceDlg->GetCurShareSoftware();
         int iResult = mpsoftwareSourceDlg->GetSelectResult();
         if (VhallLive_NO_OBJ == iResult || VhallLive_OK == iResult) {
            SetOpType(OpType_SoftWareShar);
         }
         else {
            SetOpType(mCurType);
         }

      }
      else
      {
         SetOpType(OpType_NULL);
      }
   }
   else if (eDestopType_Desktop == miShareType)
   {
      if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
      {
         mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"正在本地视频插播，请结束视频插播后重试！"), 0);
         return;
      }

      if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
         /*|| globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)*/)
      {
         mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"正在进行双推，请结束双推后重试！"), 0);
         return;
      }

      if (IsPlayMediaFile()) {
         CBaseWnd* wndLoading = globalToolManager->GetWndManager()->FindWnd(WND_ID_LOADING);
         if (wndLoading) {
            CSTDMapParam mapParam;
            mapParam["msg"] = ALREADY_PLAY_MEDIA_FILE;
            wndLoading->Notice(mapParam);
            wndLoading->exec();
         }
         return;
      }
      int iRec = QDialog::Rejected;

      if (nullptr == mpScreenChoiceDlg) {
         mpScreenChoiceDlg = new screenChoiceDlg(this);
         connect(mpScreenChoiceDlg, &screenChoiceDlg::sig_DesktopCapture, this, &LeftToolBarWdg::slot_DesktopCapture);
      }

      if (nullptr != mpScreenChoiceDlg) {
         mpScreenChoiceDlg->SetDesktopCapture(mpWebRtcLiveWdg->GetPtrVhallIALiveSettingDlg()->IsOpenDesktopCapture());
         mpScreenChoiceDlg->initList();
         iRec = mpScreenChoiceDlg->exec();
      }
      else {
         globalToolManager->GetDataManager()->WriteLog("mpScreenChoiceDlg is NULL");
      }

      if (QDialog::Rejected == iRec)
      {
         return;
      }
      else if (QDialog::Accepted == iRec)
      {
         SetOpType(OpType_DesktopShar);
         QDesktopWidget desktopWdg;
         //miIndex = mpScreenChoiceDlg->GetIndex();
         //miIndex = 1;
         int64_t iDesktoId = mpScreenChoiceDlg->GetDesktopIndex();
         miIndex = iDesktoId;
         QRect screenRect = desktopWdg.screenGeometry(iDesktoId);//0 ~1


         QString str = QString(" %1  Desk id xkk  %2").arg(__FUNCTION__).arg(iDesktoId);
         globalToolManager->GetDataManager()->WriteLog(str.toLatin1().data());

         QString msConfPath = globalToolManager->GetConfigPath();
         int iQualityId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, "1").toInt();
         globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, iQualityId);
         int iVioeProFile = 360;
         globalToolManager->GetVideoProfileByConfig(iQualityId, iVioeProFile, ToolManager::eVideoProFileTypeDesktop);
         VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(iVioeProFile, (int)ToolManager::eVideoProFileTypeDesktop);

         int iRef = globalToolManager->GetClassSDK()->StartDesktopCapture(iDesktoId, videoIndex);
         globalToolManager->GetDataManager()->WriteLog("%s iRef:%d\n", __FUNCTION__, iRef);

         if (VhallLive_OK != iRef)
         {
            mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"桌面共享错误，错误码：%1").arg(iRef), 3);
            return;
         }

         bool bIsLiving = globalToolManager->GetDataManager()->IsAlreadyPublished();
         globalToolManager->GetDataManager()->WriteLog("%s bIsLiving:%d\n", __FUNCTION__, iRef);
         if (bIsLiving) {
            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            QString strJson = "";
            if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
               QJsonObject json;
               json.insert("double_push", VHDoubleStreamType_Desktop);

               QJsonDocument document;
               document.setObject(json);
               QByteArray byteArray = document.toJson(QJsonDocument::Compact);
               strJson = QString(byteArray);
            }

            globalToolManager->GetClassSDK()->StartPushDesktopStream(strJson.toStdString());

         }
         else {

         }
      }
      else
      {
         SetOpType(OpType_NULL);
      }
   }
   else if (eDestopType_StuDesktop == miShareType) {//邀请学员共享
      if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
      {
         mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"正在本地视频插播，请结束视频插播后重试！"), 0);
         return;
      }

      if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
         /*|| globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)*/)
      {
         mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"正在进行双推，请结束双推后重试！"), 0);
         return;
      }

      if (mpWebRtcLiveWdg && mpWebRtcLiveWdg->IsDesktopShare()) {
         slots_MediaMenuAboutToHide();
         mpWebRtcLiveWdg->FadeOutTip(Str_PleaseStopDesktop, Tost_Warning);
         return;
      }
      mbRequestByManual = true;
      mpWebRtcLiveWdg->FadeOutTip(Str_StudentMaybeCant_Deskop, Tost_Warning);

      requestVrtcUsers(DoublePubStuChoiceUI::eChoiceType_Deskop);
   }
   else {}
}

void LeftToolBarWdg::SetscreenChoiceDlgState(bool baccept) {
	if (baccept && mpScreenChoiceDlg) {
		mpScreenChoiceDlg->Accept();
	}
}

void LeftToolBarWdg::slot_DesktopCapture(bool bDesktopCapture) {
   mpWebRtcLiveWdg->GetPtrVhallIALiveSettingDlg()->SetOpenDesktopCapture(bDesktopCapture);
}

void LeftToolBarWdg::setWebRtcLiveWdg(WebRtcLiveWdg* pwdg)
{
	mpWebRtcLiveWdg = pwdg;
}

void LeftToolBarWdg::initView()
{
   bool bIntShow = false;
   ToolConfig temp = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mToolConfig;
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();


   if (1 == temp.doc && baseInfo.mWebinarInfo.layout != CLASS_LAYOUT_ONLY_VIDEO)
   {
      ui.btnDoc->show();
      ui.btnDoc2->show();
      bIntShow = true;
   }
   else
   {
      ui.btnDoc2->hide();
      ui.btnDoc->hide();
   }


   if (1 == temp.board && baseInfo.mWebinarInfo.layout != CLASS_LAYOUT_ONLY_VIDEO)
   {
      ui.btnWhiteboard->show();
      ui.btnWhiteboard2->show();
      bIntShow = true;
   }
   else
   {
      ui.btnWhiteboard->hide();
      ui.btnWhiteboard2->hide();
   }



   if (1 == temp.desktop)
   {
      ui.btnDesktopSharing->show();
      ui.btnDesktopSharing2->show();
      bIntShow = true;
   }
   else
   {
      ui.btnDesktopSharing->hide();
      ui.btnDesktopSharing2->hide();
   }


   if (1 == temp.mediaplayer)
   {
      ui.btnMedia->show();
      ui.btnMedia2->show();
      bIntShow = true;
   }
   else
   {
      ui.btnMedia->hide();
      ui.btnMedia2->hide();
   }

   if (1 == temp.answer)
   {
      ui.btnAnswer->show();
      ui.btnAnswer2->show();
      bIntShow = true;
   }
   else
   {
      ui.btnAnswer->hide();
      ui.btnAnswer2->hide();
   }


   if (1 == temp.InteractionTool)
   {
      ui.btnInteractionTool->show();
      ui.btnInteractionTool2->show();
      bIntShow = true;
   }
   else
   {
      ui.btnInteractionTool->hide();
      ui.btnInteractionTool2->hide();
   }

   if (bIntShow)
      show();
   else
      hide();
}

void LeftToolBarWdg::setDocView(const bool bType/* = false*/)
{
   if (bType)
   {
      ui.btnDoc2->show();
      ui.btnDoc->show();
      ui.btnWhiteboard->show();
      ui.btnWhiteboard2->show();
      ui.horizontalSpacer_2->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
      ui.horizontalSpacer->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
      ui.horizontalSpacer_4->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
      ui.horizontalSpacer_3->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
   }
   else
   {
      ui.btnDoc2->hide();
      ui.btnDoc->hide();
      ui.btnWhiteboard->hide();
      ui.btnWhiteboard2->hide();
      ui.horizontalSpacer_2->changeSize(0, 0);
      ui.horizontalSpacer->changeSize(0, 0);
      ui.horizontalSpacer_4->changeSize(0, 0);
      ui.horizontalSpacer_3->changeSize(0, 0);
   }
}

bool LeftToolBarWdg::IsPlayMediaFile()
{
   bool bRef = false;
   bRef = globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_MediaFile);
   globalToolManager->GetDataManager()->WriteLog("%s :%d", __FUNCTION__, bRef);
   return bRef;
}

int LeftToolBarWdg::GetPlayFileState(signed long long& n64Pos, signed long long& n64MaxPos) {
   int nRet = -1;
   nRet = globalToolManager->GetClassSDK()->MediaGetPlayerState();
   n64Pos = globalToolManager->GetClassSDK()->MediaFileGetCurrentDuration();
   n64MaxPos = globalToolManager->GetClassSDK()->MediaFileGetMaxDuration();
   if (nRet == 7) {
      //MediaFilePlayFaild();
   }
   return nRet;
}

void LeftToolBarWdg::StopPlayMediaFile() {
   if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
   {
      globalToolManager->GetDataManager()->WriteLog("%s :StopPushMediaFileStream", __FUNCTION__);
      globalToolManager->GetClassSDK()->StopPushMediaFileStream();
   }

   //判断是否播放中 如是 则释放播放采集中
   if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_MediaFile))
   {
      globalToolManager->GetDataManager()->WriteLog("%s :StopMediaFileCapture", __FUNCTION__);
      globalToolManager->GetClassSDK()->StopMediaFileCapture();
   }

   StopShowMediaVideo();
}

void LeftToolBarWdg::StopShowMediaVideo() {
   globalToolManager->GetDataManager()->WriteLog("%s enter\n", __FUNCTION__);
   globalToolManager->GetDataManager()->WriteLog("%s leaveHandleConfigMediaFileMainView\n", __FUNCTION__);

}

void LeftToolBarWdg::DealStopPlay(void* apData, DWORD adwLen) {
   StopPlayMediaFile();
}

void LeftToolBarWdg::AddPlayFiles() {
   QStringList selectFileName;
   selectFileName = QFileDialog::getOpenFileNames(NULL, QObject::tr("打开文件"), "", QObject::tr("文件(*.mp3 *.wav *.mp4 *.flv *.mov *.rmvb );"), 0, 0);
   if (selectFileName.isEmpty()) {
      return;
   }


   foreach(QString filename, selectFileName) {
      //AddPlayList((char*)filename.toStdString().c_str());
   }
}

int LeftToolBarWdg::InitMediaStream(const char *szMediaFile) {
   QFile file(szMediaFile);
   if (!file.exists()) {

   }

   int iRef = globalToolManager->GetClassSDK()->StartPlayMediaFile(szMediaFile);
   if (VhallLive_OK == iRef)
   {
   }
   else
   {
      mpWebRtcLiveWdg->FadeOutTip(QString::fromWCharArray(L"插播视频错误，错误码：%1").arg(iRef), 3);
      return -1;
   }
   return 0;
}

void LeftToolBarWdg::CreateSignInDlg()
{
   if (nullptr == mpSignInDlg)
   {
      mpSignInDlg = new SignInDlg(this);
      connect(mpSignInDlg, &SignInDlg::sig_close, this, &LeftToolBarWdg::slot_MutexWdgClose);
   }
}