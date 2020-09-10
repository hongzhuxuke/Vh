#include "LoginWdg.h"
#include "pathmanager.h"
#include "ConfigSetting.h"

#include <QCryptographicHash> 
#include <QRegExpValidator>
#include <QUrl>
#include <QDesktopServices>
#include "RelevantDlg/CAboutDlg.h"
#include "class.pub.Const.h"
#include "../Unility/Unility.h"
#include <QGraphicsDropShadowEffect> 
#include <strstream>
#include "TipsWdg.h"
#include <ctime>

//ToolManager *globalToolManager = nullptr;

//extern ToolManager *globalToolManager;
extern std::wstring gCurrentVersion;
extern std::string gCurStreamID;

static int CLASS_ID_PAGE = 0;
static int CLASS_PWD_PAGE = 1;
static int MAX_ROOM_ID_LEN = 12;

static int MAX_PWD_LEN = 6;
static int MAX_PHONRE_LEN = 11;


#define NORMAL_INPUT_QSS    "QLineEdit{border:1px solid rgb(196, 196, 196); border-radius: 4px; background: white; padding-left:10px; color: rgb(90, 90, 90);} "\
 "QLineEdit:hover{ border: 1px solid rgb(153, 153, 153); border-radius: 4px; background: white; color: rgb(254, 99, 106); }" \
 "QLineEdit:focus{ border: 1px solid rgb(153, 153, 153); border-radius: 4px; background: white; padding-left: 10px; color: rgb(90, 90, 90);}"
#define ERR_INPUT_QSS       "QLineEdit{border:1px solid rgb(255, 0, 0); border-radius: 4px; background: white; padding-left: 10px;color: rgb(255, 0, 0);}"

#define LOGIN_BACK_IMAGE    QStringLiteral("QWidget#widget_3{border-image:url(:/Login/img/Login/login_page_1.png);}")
#define LOGIN_BACK_NO_LOGO_IMAGE  QStringLiteral("QWidget#widget_3{border-image:url(:/Login/img/Login/login_page_1_nologo.png);}")
#define LOGIN_BACK_NO_IMAGE    QStringLiteral("QWidget#widget_3{background-color: rgb(255, 255, 255);border:1px solid rgb(26, 26, 26, 51);border-radius:4px;}")

#define SETTING_BTN_IN_ROOM_PAGE    QStringLiteral("QPushButton#pushButton_setting{border-image:url(:/Login/img/Login/set_green_Normal.png);}\
                                                    QPushButton#pushButton_setting:hover{border-image:url(:/Login/img/Login/set_green_Click.png);}")
#define SETTING_BTN_IN_JOIN_PAGE    QStringLiteral("QPushButton#pushButton_setting{border-image:url(:/Login/img/Login/set_black_Normal.png);}\
                                                    QPushButton#pushButton_setting:hover{border-image:url(:/Login/img/Login/set_black_Click.png);}")

#define MIN_BTN_IN_ROOM_PAGE    QStringLiteral("QPushButton#pushButton_min{border-image:url(:/Login/img/Login/min_green_Normal.png);}\
                                                    QPushButton#pushButton_min:hover{border-image:url(:/Login/img/Login/mic_green_Click.png);}")
#define MIN_BTN_IN_JOIN_PAGE    QStringLiteral("QPushButton#pushButton_min{border-image:url(:/Login/img/Login/min_black_Normal.png);}\
                                                    QPushButton#pushButton_min:hover{border-image:url(:/Login/img/Login/mic_black_Click.png);}")

#define CLOSE_BTN_IN_ROOM_PAGE    QStringLiteral("QPushButton#pushButton_close{border-image:url(:/Login/img/Login/close_green_Normal.png);}\
                                                    QPushButton#pushButton_close:hover{border-image:url(:/Login/img/Login/close_green_Click.png);}")
#define CLOSE_BTN_IN_JOIN_PAGE    QStringLiteral("QPushButton#pushButton_close{border-image:url(:/Login/img/Login/close_black_Normal.png);}\
                                                    QPushButton#pushButton_close:hover{border-image:url(:/Login/img/Login/close_black_Click.png);}")

LoginWdg::LoginWdg(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);

    connect(ui.pushButton_getClassInfo, SIGNAL(clicked()), this, SLOT(slot_GetClassInfo()));
    connect(ui.pushButton_ToLoginRoom, SIGNAL(clicked()), this, SLOT(slot_JoinRoom()));
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_Close()));
    connect(ui.pushButton_min, SIGNAL(clicked()), this, SLOT(slot_Min()));
    connect(ui.pushButton_setting, SIGNAL(clicked()), this, SLOT(slot_Setting()));
    connect(ui.pushButton_turnToRoomIdPage, SIGNAL(clicked()), this, SLOT(slot_TurnToRoomIdPage())); 
	connect(ui.pushButton_ChangeUser, SIGNAL(clicked()), this, SLOT(slot_ChangeUser()));

    ui.stackedWidget->setCurrentIndex(CLASS_ID_PAGE);
    //最多可输入12个字符（限英文、数字或下划线
    QRegExp regxRoomID("^[0-9a-zA-Z_]{1,}$");
    QValidator *validatorRoomId = new QRegExpValidator(regxRoomID, ui.lineEdit_room_id);
    ui.lineEdit_room_id->setValidator(validatorRoomId);
    ui.lineEdit_room_id->setMaxLength(MAX_ROOM_ID_LEN);
    ui.lineEdit_room_id->setStyleSheet(NORMAL_INPUT_QSS);

    //上课口令输入框最多可输入6个数字，限讲师和学员口令
    QRegExp regxPwd("^[0-9]{1,}$");
    QValidator *validatorPwd = new QRegExpValidator(regxPwd, ui.lineEdit_pwd);
    ui.lineEdit_pwd->setValidator(validatorPwd);
	
    //姓名输入框最多可输入20个字符
    ui.lineEdit_nickName->setMaxLength(MAX_NICKNAME_LEN);
    ui.pushButton_turnToRoomIdPage->hide();


    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);

    QString version = ToolManager::GetInstance()->GetDataManager()->GetVersion();
    if (!version.isEmpty()) {
       int iPos = version.lastIndexOf(".");
        QString midVersion = version.mid(0, iPos);//取版本前两位 1.0
        //QString class_conf = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
		QString class_conf = ToolManager::GetInstance()->GetToolConfigPath();
		int hideLogo = ConfigSetting::ReadInt(class_conf, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
        if (hideLogo) {
           ui.label_version_notice->setText(QStringLiteral("客户端V%1").arg(midVersion));
        }
        else {
           ui.label_version_notice->setText(QStringLiteral("客户端V%1").arg(midVersion));
        }
    }

    //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = ToolManager::GetInstance()->GetConfigPath();
    QString roomId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_ROOM_ID,  "");
    QString nickName = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_NICK_NAME,  "");
    QString pwd = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT,KEY_ROOM_PWD,  "");

    //QString class_conf = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
    
	QString class_conf = ToolManager::GetInstance()->GetToolConfigPath();
	int hideLogo = ConfigSetting::ReadInt(class_conf, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
    if (hideLogo == 1) {
       ui.widget_3->setStyleSheet(LOGIN_BACK_NO_LOGO_IMAGE);
    }
    else {
       ui.widget_3->setStyleSheet(LOGIN_BACK_IMAGE);
    }


    ui.lineEdit_room_id->setText(roomId);
    ui.lineEdit_nickName->setText(nickName);
    ui.lineEdit_pwd->setText(pwd);

    //每次登陆时取消设置桌面采集。需要用户自己在设置页面进行勾选。
    ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, 0);
    ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);

	mpEffect = new QGraphicsDropShadowEffect(this);
	mpEffect->setOffset(4, 6);
	mpEffect->setColor(QColor(0, 0, 0, 40));
	mpEffect->setBlurRadius(10);
	ui.widget_3->setGraphicsEffect(mpEffect);

	ui.pushButton_ChangeUser->setFocusPolicy(Qt::NoFocus);
}

LoginWdg::~LoginWdg()
{
	if (nullptr != mpEffect)
	{
		delete mpEffect;
		mpEffect = nullptr;
	}

    ui.lineEdit_room_id->removeEventFilter(this);
    //if (NULL != mpAboutDlg)
    //{
    //    delete mpAboutDlg;
    //    mpAboutDlg = NULL;
    //}
}

void LoginWdg::slot_GetClassInfo() {
	bool exist = ToolManager::GetInstance()->GetWndManager()->IsWndExist(WND_ID_TIMER_TOOL);
	if (exist) {
		return;
	}

	ToolManager::GetInstance()->GetDataManager()->WriteLog("%s", __FUNCTION__);
	QString live_room_id = ui.lineEdit_room_id->text();
	if (live_room_id.isEmpty()) {
		ui.lineEdit_room_id->clear();
		ui.lineEdit_room_id->clearFocus();
		ui.lineEdit_room_id->setStyleSheet(ERR_INPUT_QSS);
		ui.lineEdit_room_id->setPlaceholderText(QStringLiteral("请输入课程ID"));
		connect(ui.lineEdit_room_id, SIGNAL(sig_focusInEvent()), this, SLOT(slot_LineEditRoomIdFocusIn()));
		return;
	}

	ToolManager::GetInstance()->GetDataManager()->SetLiveRoomId(live_room_id.trimmed());
	gCurStreamID = live_room_id.toStdString();
	ToolManager::GetInstance()->GetClassSDK()->GetWebinarDetailInfo(live_room_id.trimmed().toStdString());
	ui.pushButton_getClassInfo->setEnabled(false);
}

void LoginWdg::slot_JoinRoom() {
    ToolManager::GetInstance()->GetDataManager()->WriteLog("%s", __FUNCTION__);
	CourseInfo baseCourseInfo = ToolManager::GetInstance()->GetClassSDK()->GetCourseInfo();
	//
	int viewing_conditions = -1;
	if (mLoginIdentity) {//如果是特殊身份 以口令方式登录
		viewing_conditions = ToolManager::eLoginMode_Command;
	}
	else {
		viewing_conditions = baseCourseInfo.viewing_conditions;
	}
	switch (viewing_conditions)
	{
	case ToolManager::eLoginMode_Command:
	{
		QString nickName = ui.lineEdit_nickName->text();
		nickName = nickName.trimmed();
		if (nickName.isEmpty()) {
			ui.lineEdit_nickName->clear();
			ui.lineEdit_nickName->clearFocus();
			ui.lineEdit_nickName->setStyleSheet(ERR_INPUT_QSS);
			ui.lineEdit_nickName->setPlaceholderText(QStringLiteral("请输入姓名"));
			connect(ui.lineEdit_nickName, SIGNAL(sig_focusInEvent()), this, SLOT(slot_NickNameEditRoomIdFocusIn()));
			return;
		}

		QString pwd = ui.lineEdit_pwd->text();
		if (pwd.isEmpty()) {
			ui.lineEdit_pwd->clear();
			ui.lineEdit_pwd->clearFocus();
			ui.lineEdit_pwd->setStyleSheet(ERR_INPUT_QSS);
			ui.lineEdit_pwd->setPlaceholderText(QStringLiteral("请输入上课口令"));
			connect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
			return;
		}
		ToolManager::GetInstance()->GetClassSDK()->JoinClassRoom(ui.lineEdit_nickName->text().toStdString(), pwd.toStdString(),"");
		break;
	}
	case ToolManager::eLoginMode_WhiteList: {
		QString pwd = ui.lineEdit_pwd->text();
		if (pwd.isEmpty()) {
			ui.lineEdit_pwd->clear();
			ui.lineEdit_pwd->clearFocus();
			ui.lineEdit_pwd->setStyleSheet(ERR_INPUT_QSS);
			ui.lineEdit_pwd->setPlaceholderText(QStringLiteral("请输入手机号"));
			connect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
			return;
		}
		ToolManager::GetInstance()->GetClassSDK()->JoinClassRoom("","" , pwd.toStdString());
		break;
	}
	case ToolManager::eLoginMode_Free: {
		QString nickName = ui.lineEdit_nickName->text();
		//nickName = nickName.trimmed();
		if (nickName.isEmpty()) {
			ui.lineEdit_nickName->clear();
			ui.lineEdit_nickName->clearFocus();
			ui.lineEdit_nickName->setStyleSheet(ERR_INPUT_QSS);
			ui.lineEdit_nickName->setPlaceholderText(QStringLiteral("请输入姓名"));
			connect(ui.lineEdit_nickName, SIGNAL(sig_focusInEvent()), this, SLOT(slot_NickNameEditRoomIdFocusIn()));
			return;
		}
		ToolManager::GetInstance()->GetClassSDK()->JoinClassRoom(ui.lineEdit_nickName->text().toStdString(), "", "");
		break;
	}
	}

    ui.pushButton_ToLoginRoom->setDisabled(true);
}

void LoginWdg::slot_Close() {
    //CBaseWnd* tips = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_TIPS_WND, "close");
	TipsWdg* tips = new TipsWdg(this);
    if (tips) {
        CSTDMapParam param;
        param[TipsMsg] = Str_IsExitVhallClassApp;
        param[ShowCancelButton] = PARAM_TRUE;
        tips->Notice(param);
        int accept = tips->exec();
        if (accept == QDialog::Accepted) {
            ToolManager::GetInstance()->GetDataManager()->WriteLog("%s", __FUNCTION__);
            ToolManager::GetInstance()->SafeToExitApp();
			
        }
    }
}

void LoginWdg::slot_Setting() {
	ToolManager::GetInstance()->GetClassSDK()->InitWebRtcSDK();
    CBaseWnd* wnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
    if (wnd) {
        CSTDMapParam mapParam;
        mapParam[Str_Function] = Str_Function_FromLogin;
        wnd->Notice(mapParam);
        wnd->exec();
    }
}

void LoginWdg::slot_ChangeUser() {
	mLoginIdentity = !mLoginIdentity;
	if (mLoginIdentity) {
		ui.label_User->setText(QString::fromLocal8Bit("讲师登录"));
		ui.pushButton_ChangeUser->setText(QString::fromLocal8Bit("返回学员登录"));
	}
	else {
		ui.label_User->setText(QString::fromLocal8Bit("学员登录"));
		ui.pushButton_ChangeUser->setText(QString::fromLocal8Bit("以讲师身份登录"));
	}
	LoginIdentity(mLoginIdentity);
}

void LoginWdg::slot_TurnToRoomIdPage() {
    ui.pushButton_turnToRoomIdPage->hide();
    //QString class_conf = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
	mLoginIdentity = false;
	QString class_conf = ToolManager::GetInstance()->GetToolConfigPath();
	int hideLogo = ConfigSetting::ReadInt(class_conf, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
    if (hideLogo == 1) {
       ui.widget_3->setStyleSheet(LOGIN_BACK_NO_LOGO_IMAGE);
    }
    else {
       ui.widget_3->setStyleSheet(LOGIN_BACK_IMAGE);
    }
    ui.stackedWidget->setCurrentIndex(CLASS_ID_PAGE);
    ChangeLoginPage(CLASS_ID_PAGE);
}

void LoginWdg::slot_Min() {
    showMinimized();
}

void LoginWdg::on_btnAbout_clicked(bool checked /*= false*/)
{
	//if (NULL == mpAboutDlg)
	//{
	//	mpAboutDlg = new CAboutDlg();
	//}
	//
	//if (NULL != mpAboutDlg)
	//	mpAboutDlg->show();
}

void LoginWdg::Notice(const CSTDMapParam &mapParam) {
    std::list<std::string>::iterator iterlist = sendlist_.begin();
    if (iterlist != sendlist_.end()) {
        *iterlist = std::string("123");
    }
} 

void LoginWdg::show()
{
	int iStartMode = ToolManager::GetInstance()->GetStartMode();
	ToolManager::GetInstance()->GetDataManager()->WriteLog("%s iStartMode %d", __FUNCTION__, iStartMode);
	switch(iStartMode)
	{
	case ToolManager::eStartMode_desktop:
		ToolManager::GetInstance()->GetDataManager()->WriteLog("%s -- CBaseWnd::show()", __FUNCTION__);
		CBaseWnd::show();
	break;
	case ToolManager::eStartMode_private:
	{
		ui.pushButton_turnToRoomIdPage->show();
		ui.stackedWidget->setCurrentIndex(CLASS_PWD_PAGE);
		ChangeLoginPage(CLASS_PWD_PAGE);

		ui.lineEdit_nickName->setEnabled(false);
		ui.lineEdit_pwd->setEnabled(false);
		ui.pushButton_ToLoginRoom->setEnabled(false);

		CBaseWnd::show();
		ui.widget_3->setStyleSheet(LOGIN_BACK_NO_IMAGE);
		ToolManager::GetInstance()->GetDataManager()->WriteLog("%s -- ToolManager::eStartMode_private", __FUNCTION__);
 		QString strStartInfo = ToolManager::GetInstance()->GetPrivateStartInfo();
		ToolManager::GetInstance()->GetClassSDK()->JoinClassRoom(strStartInfo.toStdString());
	}
	break;
	default:
		ToolManager::GetInstance()->GetDataManager()->WriteLog("%s -- default ", __FUNCTION__);
		CBaseWnd::show();
		break;
	}
}

void LoginWdg::hide()
{
	CBaseWnd::hide();
}

bool LoginWdg::close()
{
	return CBaseWnd::close();
}

CBaseWnd* LoginWdg::CreateFunction() {
   return new LoginWdg();
}

void LoginWdg::keyPressEvent(QKeyEvent *event) {

}

void LoginWdg::customEvent(QEvent *event) {
    if (event == nullptr) {
        return;
    }
    int type = event->type();
    switch (type)
    {
        case CustomEvent_GetWebinarDetailInfo: {
            HandleGetBaseInfo(event);
        }
        break;
        case CustomEvent_JoinClassRoom: {
            HandleJoinClass(event);
            break;
        }
        default:
            break;
    }
}

void LoginWdg::closeEvent(QCloseEvent* e) {
    e->ignore();
    QWidget::closeEvent(e);
    slot_Close();
}

void LoginWdg::HandleGetBaseInfo(QEvent* event) {
    ui.pushButton_getClassInfo->setEnabled(true);
    QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
    if (roomEvent) {
        ToolManager::GetInstance()->GetDataManager()->WriteLog("%s roomEvent->mRespCode:%d roomEvent->mRespMsg:%s", __FUNCTION__, roomEvent->mRespCode, roomEvent->mRespMsg.c_str());
        if (roomEvent->mRespCode == 200) {
            CourseInfo baseCourseInfo = ToolManager::GetInstance()->GetClassSDK()->GetCourseInfo();
            ui.label_class_id->setText(QString::fromStdString(baseCourseInfo.webinar_id));
            QString webinar_subject = QString::fromStdString(baseCourseInfo.webinar_subject);
            if (webinar_subject.length() > 16) {
                webinar_subject = webinar_subject.insert(16, "\n");
            }
            ui.label_class_name->setText(webinar_subject);
            switch (baseCourseInfo.webinar_status)// 1:上课中 2 : 预告 3 : 回放 4：转播 5 : 已下课
            {
            case CLASS_STATUS_IN_CLASS: {// 1:上课中 
                ui.label_class_status->setStyleSheet(QStringLiteral("border-image: url(:/Login/img/Login/class_starting.png);"));
                ReSizeStatusSize(STATUS_LOGO_WIDTH, STATUS_LOGO_HEIGHT);
                break;
            }
            case CLASS_STATUS_PRE: {// 2 : 预告 
                ui.label_class_status->setStyleSheet(QStringLiteral("border-image: url(:/Login/img/Login/notice.png);"));
                ReSizeStatusSize(STATUS_LOGO_MIN_WIDTH, STATUS_LOGO_HEIGHT);
                break;
            }
            case CLASS_STATUS_PLAY_BACK: {// 3 : 回放 
                ui.label_class_status->setStyleSheet(QStringLiteral("border-image: url(:/Login/img/Login/review.png);"));
                ReSizeStatusSize(STATUS_LOGO_MIN_WIDTH, STATUS_LOGO_HEIGHT);
                break;
            }
            case CLASS_STATUS_RELAY: {//4：转播
                ui.label_class_status->setStyleSheet(QStringLiteral("border-image: url(:/Login/img/Login/transport.png);"));
                ReSizeStatusSize(STATUS_LOGO_MIN_WIDTH, STATUS_LOGO_HEIGHT);
                break;
            }
            case CLASS_STATUS_OVER: {//5 : 已下课
                ui.label_class_status->setStyleSheet(QStringLiteral("border-image: url(:/Login/img/Login/over.png);"));
                ReSizeStatusSize(STATUS_LOGO_WIDTH, STATUS_LOGO_HEIGHT);
                break;
            }
            default:
                break;
            }
            ui.widget_3->setStyleSheet(LOGIN_BACK_NO_IMAGE);
            ui.pushButton_turnToRoomIdPage->show();
			//判断观看条件登陆方式----------------------
            ui.stackedWidget->setCurrentIndex(CLASS_PWD_PAGE);
            ChangeLoginPage(CLASS_PWD_PAGE);
			//CourseInfo baseCourseInfo = ToolManager::GetInstance()->GetClassSDK()->GetCourseInfo();
			mLoginIdentity = true;
			slot_ChangeUser();
			

            ui.pushButton_ToLoginRoom->setFocus();
        }
        else {
            QString msg = QString::fromStdString(roomEvent->mRespMsg);
            if (msg.isEmpty()) {
                msg = QStringLiteral("请求失败,请重试");
            }
            else {
                msg = QStringLiteral("%1").arg(msg.replace("http request err", Str_ServerTimeOut));
            }
            ui.lineEdit_room_id->clear();
            ui.lineEdit_room_id->clearFocus();
            ui.lineEdit_room_id->setStyleSheet(ERR_INPUT_QSS);
            ui.lineEdit_room_id->setPlaceholderText(msg);
            connect(ui.lineEdit_room_id, SIGNAL(sig_focusInEvent()), this, SLOT(slot_LineEditRoomIdFocusIn()));
        }
    }
}

void LoginWdg::LoginIdentity(bool bSpecial) {
	CourseInfo baseCourseInfo = ToolManager::GetInstance()->GetClassSDK()->GetCourseInfo();
	QString msConfPath = ToolManager::GetInstance()->GetConfigPath();

	QString nickName = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_NICK_NAME, "");
	QString pwd = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_ROOM_PWD, "");
	QString phone = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_ROOM_PHONE, "");
	if (bSpecial) {
		if (baseCourseInfo.viewing_conditions == ToolManager::eLoginMode_Command) {
			ui.label_User->hide();
			ui.pushButton_ChangeUser->hide();
		}

		ui.lineEdit_nickName->setText(nickName);
		ui.lineEdit_pwd->setText(pwd);

		ui.lineEdit_nickName->show();
		ui.lineEdit_pwd->show();
		//ui.lineEdit_nickName->clear();
		ui.lineEdit_nickName->clearFocus();
		ui.lineEdit_nickName->setStyleSheet(NORMAL_INPUT_QSS);
		ui.lineEdit_nickName->setPlaceholderText(QStringLiteral("请输入姓名"));
		connect(ui.lineEdit_nickName, SIGNAL(sig_focusInEvent()), this, SLOT(slot_NickNameEditRoomIdFocusIn()));
		//ui.lineEdit_pwd->clear();
		ui.lineEdit_pwd->clearFocus();
		ui.lineEdit_pwd->setStyleSheet(NORMAL_INPUT_QSS);
		ui.lineEdit_pwd->setMaxLength(MAX_PWD_LEN);
		ui.lineEdit_pwd->setPlaceholderText(QStringLiteral("请输入上课口令"));
		connect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
	}
	else {
		switch (baseCourseInfo.viewing_conditions)
		{
			case ToolManager::eLoginMode_Command:
			{
				ui.label_User->hide();
				ui.pushButton_ChangeUser->hide();

				ui.lineEdit_nickName->setText(nickName);
				ui.lineEdit_pwd->setMaxLength(MAX_PWD_LEN);
				ui.lineEdit_pwd->setText(pwd);

				ui.lineEdit_nickName->show();
				ui.lineEdit_pwd->show();
				//ui.lineEdit_nickName->clear();
				ui.lineEdit_nickName->clearFocus();
				ui.lineEdit_nickName->setStyleSheet(NORMAL_INPUT_QSS);
				ui.lineEdit_nickName->setPlaceholderText(QStringLiteral("请输入姓名"));
				connect(ui.lineEdit_nickName, SIGNAL(sig_focusInEvent()), this, SLOT(slot_NickNameEditRoomIdFocusIn()));
				//ui.lineEdit_pwd->clear();
				ui.lineEdit_pwd->clearFocus();
				ui.lineEdit_pwd->setStyleSheet(NORMAL_INPUT_QSS);
				ui.lineEdit_pwd->setPlaceholderText(QStringLiteral("请输入上课口令"));
				connect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
				break;
			}
			case ToolManager::eLoginMode_WhiteList: {
				ui.label_User->show();
				ui.pushButton_ChangeUser->show();

				ui.lineEdit_pwd->setMaxLength(MAX_PHONRE_LEN);
				ui.lineEdit_pwd->setText(phone);

				ui.lineEdit_nickName->hide();
				ui.lineEdit_pwd->show();
				//ui.lineEdit_pwd->clear();
				ui.lineEdit_pwd->clearFocus();
				ui.lineEdit_pwd->setStyleSheet(NORMAL_INPUT_QSS);
				ui.lineEdit_pwd->setPlaceholderText(QStringLiteral("请输入手机号"));
				connect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
				break;
			}
			case ToolManager::eLoginMode_Free: {
				ui.label_User->show();
				ui.pushButton_ChangeUser->show();
				ui.lineEdit_nickName->setText(nickName);

				ui.lineEdit_pwd->hide();
				ui.lineEdit_nickName->show();
				//ui.lineEdit_nickName->clear();
				ui.lineEdit_nickName->clearFocus();
				ui.lineEdit_nickName->setStyleSheet(NORMAL_INPUT_QSS);
				ui.lineEdit_nickName->setPlaceholderText(QStringLiteral("请输入姓名"));
				connect(ui.lineEdit_nickName, SIGNAL(sig_focusInEvent()), this, SLOT(slot_NickNameEditRoomIdFocusIn()));
				break;
			}
		}
	}
	
}

void LoginWdg::ReSizeStatusSize(int width, int height) {
    QSize size(width, height);
    ui.label_class_status->setMaximumSize(size);
    ui.label_class_status->setMinimumSize(size);
}

void LoginWdg::ChangeLoginPage(int pageIndex) {
    if (pageIndex == CLASS_ID_PAGE) {
        ui.pushButton_close->setStyleSheet(CLOSE_BTN_IN_ROOM_PAGE);
        ui.pushButton_min->setStyleSheet(MIN_BTN_IN_ROOM_PAGE);
        ui.pushButton_setting->setStyleSheet(SETTING_BTN_IN_ROOM_PAGE);
        //ui.lineEdit_nickName->clear();
        //ui.lineEdit_pwd->clear();
    }
    else {
        ui.pushButton_close->setStyleSheet(CLOSE_BTN_IN_JOIN_PAGE);
        ui.pushButton_min->setStyleSheet(MIN_BTN_IN_JOIN_PAGE);
        ui.pushButton_setting->setStyleSheet(SETTING_BTN_IN_JOIN_PAGE);
    }
}


void LoginWdg::HandleJoinClass(QEvent* event) {
	ToolManager::GetInstance()->GetDataManager()->WriteLog("%s START ",  __FUNCTION__);
    ui.pushButton_ToLoginRoom->setDisabled(false);
    QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
    if (roomEvent) {
		ToolManager::GetInstance()->GetDataManager()->WriteLog("%s  roomEvent is not NULL", __FUNCTION__);
        if (roomEvent->mRespCode != 200) {
            QString msg = QString::fromStdString(roomEvent->mRespMsg);
            if (msg.isEmpty()) {
                msg = QStringLiteral("传入参数错误");
				ToolManager::GetInstance()->GetDataManager()->WriteLog("%s  mRespCode is error", __FUNCTION__);
            }
            else {
               msg = msg.replace("http request err", Str_ServerTimeOut);
            }
            ui.lineEdit_pwd->clear();
            ui.lineEdit_pwd->clearFocus();
            ui.lineEdit_pwd->setStyleSheet(ERR_INPUT_QSS);
            ui.lineEdit_pwd->setPlaceholderText(msg);
            connect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
        }
        else {
			//QString confPath = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
			
			QString confPath = ToolManager::GetInstance()->GetToolConfigPath();
			QString strLogReportUrl = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_LOGREPORT_URL, defaultLogReportURL);
			QString strVer = ToolManager::GetInstance()->GetDataManager()->GetVersion();
	
         UserJoinRoomBaseInfo info = ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo;
         CourseInfo corse_info = ToolManager::GetInstance()->GetClassSDK()->GetCourseInfo();
         QString strParam = QString("%1?key=class_join&k=%2&id=%3&nick=%4&role=%5&devid=%6&ver=%7&uid=%8")
            .arg(strLogReportUrl)
            .arg(eClassLogRePortK_StartStream)
            .arg(info.join_id.c_str())
            .arg(info.nick_name.c_str())
            .arg(info.role_type)
            .arg(ToolManager::GetInstance()->GetClassSDK()->GetDevId().c_str())
            .arg(strVer)
            .arg(corse_info.uid.c_str());

         ToolManager::GetInstance()->GetClassSDK()->UpLoadLog(strParam.toStdString());

		 ToolManager::GetInstance()->GetClassSDK()->SetWhiteListPhone(ui.lineEdit_pwd->text().toStdString());
            //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
			QString msConfPath = ToolManager::GetInstance()->GetConfigPath();
            QString roomId = ui.lineEdit_room_id->text();
            QString nickName = ui.lineEdit_nickName->text();
            QString pwd = ui.lineEdit_pwd->text();
            ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_ROOM_ID, roomId);
            ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_NICK_NAME, nickName);
			if (ToolManager::GetInstance()->GetClassSDK()->GetCourseInfo().viewing_conditions == ToolManager::eLoginMode_WhiteList && !mLoginIdentity) {
				ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_ROOM_PHONE, pwd);
			}
			else {
				ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_ROOM_PWD, pwd);
			}


            if (ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher ||
                ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Student)
			{
            ClassRoomBaseInfo baseInfo = ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo();
            ToolManager::GetInstance()->GetClassSDK()->InitSDK(baseInfo.play_type);
				ToolManager::GetInstance()->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_LOGIN));

				CBaseWnd* mainWnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
				if (mainWnd) {
					CSTDMapParam param;
					param[START_WEB_RTC_LIVE] = START_WEB_RTC_LIVE;
               
					mainWnd->Notice(param);
               mainWnd->show();
               mainWnd->raise();
               mainWnd->activateWindow();

               CSTDMapParam paramDevTest;
               paramDevTest[DEV_TEST] = DEV_TEST;
               mainWnd->Notice(paramDevTest);


					if (ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.countdown_starttime > 0
						&& ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.countdown_time > 0)
					{
						std::time_t mCurrenttime;
						std::time(&mCurrenttime);
						int residuetime = ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.countdown_time - (mCurrenttime - ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.countdown_starttime);
						if (residuetime > 0)
						{
							ToolManager::GetInstance()->OnRecvStartCountdown(ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.user_id, 0, residuetime);
						}
					}

					if (ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mStruSign.irest_time > 0
						&& ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mStruSign.strSignStatus == "ongoing")
					{
						ToolManager::GetInstance()->OnRecvStartSign(ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.id,
							ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mStruSign.strSignId,
							ToolManager::GetInstance()->GetClassSDK()->GetClassRoomBaseInfo().mStruSign.irest_time);

					}
				}
				else {
					ToolManager::GetInstance()->GetDataManager()->WriteLog("%s  mainWnd is NULL", __FUNCTION__);
				}


            }
            else {
                QString msg = QStringLiteral("您的身份不支持从客户端登录，请使用网页端");
                ui.lineEdit_pwd->clear();
                ui.lineEdit_pwd->clearFocus();
                ui.lineEdit_pwd->setStyleSheet(ERR_INPUT_QSS);
                ui.lineEdit_pwd->setPlaceholderText(msg);
                connect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
				show();
            }
        }
    }
	ToolManager::GetInstance()->GetDataManager()->WriteLog("%s END ", __FUNCTION__);
}

void LoginWdg::slot_LineEditRoomIdFocusIn() {
    disconnect(ui.lineEdit_room_id, SIGNAL(sig_focusInEvent()), this, SLOT(slot_LineEditRoomIdFocusIn()));
    ui.lineEdit_room_id->setStyleSheet(NORMAL_INPUT_QSS);
    //ui.lineEdit_room_id->setPlaceholderText(QStringLiteral("请输入课程ID"));
}

void LoginWdg::slot_PwdEditRoomIdFocusIn() {
    disconnect(ui.lineEdit_pwd, SIGNAL(sig_focusInEvent()), this, SLOT(slot_PwdEditRoomIdFocusIn()));
    ui.lineEdit_pwd->setStyleSheet(NORMAL_INPUT_QSS);
    //ui.lineEdit_pwd->setPlaceholderText(QStringLiteral("输入上课口令"));
}

void LoginWdg::slot_NickNameEditRoomIdFocusIn() {
    disconnect(ui.lineEdit_nickName, SIGNAL(sig_focusInEvent()), this, SLOT(slot_NickNameEditRoomIdFocusIn()));
    ui.lineEdit_nickName->setStyleSheet(NORMAL_INPUT_QSS);
    //ui.lineEdit_nickName->setPlaceholderText(QStringLiteral("输入姓名"));
}
