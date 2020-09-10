#include "MoreSettingWdg.h"
#include "../Unility/Unility.h"
#include "LanguageSetDlg.h"
#include "pathmanager.h"
#include "TipsWdg.h"
#include "WebRtcLiveWdg.h"
#include "VhallIALiveSettingDlg.h"

#include <QPainter>
extern ToolManager *globalToolManager;


MoreSettingWdg::MoreSettingWdg(QWidget *parent /*= Q_NULLPTR*/)
	:QDialog(parent)
{
	ui.setupUi(this);
	this->setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen|Qt::WindowStaysOnTopHint);

	ui.trophyWg->hideLabAdd();
	ui.trophyWg->SetStyleSheet("#widget \
		{ \
		background-color: rgba(0, 151, 104, 142);\
		border-radius : 14px; \
		}");

	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (JOIN_ROLE_Student == baseInfo.mUserInfo.role_type && CLASS_TYPE_SMALL == baseInfo.mWebinarInfo.course_type)
	{
		ui.trophyWg->show();
		ui.trophyWg->initTrophyRewardNum(baseInfo.mUserInfo.iRewardNum < 0 ? 0 : baseInfo.mUserInfo.iRewardNum);
	}
	else
	{
		ui.trophyWg->hide();
	}
	
	
	//隐藏语言设置
	ui.btnLaguage->hide();
	ui.labLaguage->hide();
}

MoreSettingWdg::~MoreSettingWdg()
{

}

void MoreSettingWdg::hideMediaSet()
{
	ui.btnMedia->setEnabled(false);
	ui.labMedia->setEnabled(false);
}

void MoreSettingWdg::AddTrophyCount(const int& iAdd)
{
	ui.trophyWg->setTrophyAdd(iAdd);
}

int MoreSettingWdg::iTrophyCount()
{
	return ui.trophyWg->iTrophyCount();
}
void MoreSettingWdg::SetRoleType(const int& iRolType) {
	switch (iRolType)
	{
	case JOIN_ROLE_Teacher:// 1 : 老师
		ui.labRol->setText(QStringLiteral("讲师"));
		break;
	case JOIN_ROLE_Student://  2 ; 学员
		ui.labRol->setText(QStringLiteral("学员"));
		break;
	case JOIN_ROLE_Assistant:// 3 : 助教
		ui.labRol->setText(QStringLiteral("助教"));
		break;
	case JOIN_ROLE_Guest://4 : 嘉宾
		ui.labRol->setText(QStringLiteral("嘉宾"));
		break;
	case JOIN_ROLE_Supervise://5 : 监课
		ui.labRol->setText(QStringLiteral("监课"));
		break;
	case JOIN_ROLE_Admin://6 : 管理员
		ui.labRol->setText(QStringLiteral("管理员"));
		break;
	}
}
void MoreSettingWdg::SetNickName(const QString& strNickName) {
	ui.labNick->setText(CPathManager::GetString(strNickName, 14));
	ui.labNick->setToolTip(strNickName);
}

void MoreSettingWdg::show(int x, int y)
{
	move(x-222, y);
	setFocus(); 	
    QDialog::show();
}

//void MoreSettingWdg::paintEvent(QPaintEvent *e) {
//	QPainter painter(this);
//	
//	//bool bd = mPixmap.load("E:\\workSpace\\VhallClassRoom\\VLive\\img\\widget\\morechouse.png");
//
//	painter.drawPixmap(rect(), mPixmap.copy(0, 0, mPixmap.width(), mPixmap.height()));
//	QDialog::paintEvent(e);
//}

void MoreSettingWdg::showEvent(QShowEvent *e) {
	setFocus();
	QDialog::showEvent(e);
}

void MoreSettingWdg::enterEvent(QEvent *e) {
	QDialog::enterEvent(e);
	m_bEnter = true;
}
void MoreSettingWdg::leaveEvent(QEvent *e) {
	QDialog::leaveEvent(e);
	m_bEnter = false;
}

void MoreSettingWdg::focusOutEvent(QFocusEvent *e) {
	if (!m_bEnter) {
		hide();
		this->close();
		return;
	}

	QWidget *focusWidget = this->focusWidget();
	bool focusThis = false;

	while (focusWidget) {
		if (focusWidget == this) {
			focusThis = true;
			break;
		}

		focusWidget = focusWidget->parentWidget();
	}

	if (!focusThis) {
		this->close();
		m_bEnter = false;
	}
	else {
		this->setFocus();
	}
}
void MoreSettingWdg::on_btnMedia_clicked(bool checked /*= false*/) {
    hide();

    VhallIALiveSettingDlg* wnd = nullptr;
	CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	if (pWdg)
	{
		WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
		wnd = pMainWdg->GetPtrVhallIALiveSettingDlg();
	}

	//CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
	if (wnd) {
		CSTDMapParam param;
		wnd->Notice(param);
		wnd->exec();
	}
}

void MoreSettingWdg::on_btnLaguage_clicked(bool checked /*= false*/) {
	LanguageSetDlg languageDlg(this);
	languageDlg.exec();
}

void MoreSettingWdg::on_btnRolQuit_clicked(bool checked /*= false*/) {
    hide();
    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
        //CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_TIPS_WND);
		TipsWdg* tips = new TipsWdg(this);
        if (tips) {
            CSTDMapParam param;
            param[TipsMsg] = QStringLiteral("您正在上课，退出后将中断课程，是否继续操作?");
            param[ShowCancelButton] = PARAM_TRUE;
            tips->Notice(param);
            int accept = tips->exec();
            if (accept == QDialog::Accepted) {
                globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
                globalToolManager->GetClassSDK()->StopClass(0);
                globalToolManager->LogOut();
            }
        }
    }
    else {
        //CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_TIPS_WND);
		TipsWdg* tips = new TipsWdg(this);
        if (tips) {
            CSTDMapParam param;
            param[TipsMsg] = QStringLiteral("是否确认角色退出?");
            param[ShowCancelButton] = PARAM_TRUE;
            int createDefaultRecord = 0;
            tips->Notice(param);
            int accept = tips->exec();
            if (accept == QDialog::Accepted) {
                globalToolManager->LogOut();
            }
        }
    }
}