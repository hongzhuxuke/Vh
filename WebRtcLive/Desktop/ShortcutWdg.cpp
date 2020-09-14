#include "ShortcutWdg.h"
#include "../Unility/Unility.h"

extern ToolManager *globalToolManager;

ShortcutWdg::ShortcutWdg(QWidget *parent /*= Q_NULLPTR*/)
	:QDialog(parent)
{
	ui.setupUi(this);
	this->setFocusPolicy(Qt::StrongFocus);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
   JOIN_ROLE iRole = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;

   if (JOIN_ROLE_Teacher == iRole) {
      ui.But_member->show();
   }
   else if (JOIN_ROLE_Student == iRole) {
      ui.But_member->hide();
   }
}

ShortcutWdg::~ShortcutWdg()
{
}

void ShortcutWdg::on_But_chat_clicked()
{
	emit sig_click(OpMediaType_ChatShare);
}

void ShortcutWdg::on_But_member_clicked()
{
	emit sig_click(OpMediaType_Member);
}

void ShortcutWdg::on_But_Testpaper_clicked()
{
   emit sig_click(OpMediaType_Testpaper);
}

void ShortcutWdg::on_But_Signin_clicked()
{
   emit sig_click(OpMediaType_Signin);
}

void ShortcutWdg::on_But_answer_clicked()
{
   emit sig_click(OpMediaType_answer);
}

void ShortcutWdg::on_But_time_clicked()
{
   emit sig_click(OpMediaType_time);
}

void ShortcutWdg::showEvent(QShowEvent *e) {
	setFocus();
	QDialog::showEvent(e);
}

void ShortcutWdg::enterEvent(QEvent *e) {
	QDialog::enterEvent(e);
	m_bEnter = true;
}
void ShortcutWdg::leaveEvent(QEvent *e) {
	QDialog::leaveEvent(e);
	m_bEnter = false;
}

void ShortcutWdg::focusOutEvent(QFocusEvent *e) {
	if (!m_bEnter)
	{
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