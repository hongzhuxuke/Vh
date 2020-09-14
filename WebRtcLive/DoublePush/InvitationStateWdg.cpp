#include "stdafx.h"

#include "InvitationStateWdg.h"

extern ToolManager *globalToolManager;
using namespace std;


InvitationStateWdg::InvitationStateWdg(QWidget *parent) :
   CWidget(parent)  {
	ui.setupUi(this);
   setFocusPolicy(Qt::NoFocus);

}

InvitationStateWdg::~InvitationStateWdg() {
   stopTimer();
   if (nullptr != mpOvertimeTimer) {
      delete mpOvertimeTimer;
      mpOvertimeTimer = nullptr;
   }
}

void InvitationStateWdg::SetStuId(const QString& stuId)
{
   mstrCurId = stuId;
}

void InvitationStateWdg::SetType(const DoublePubStuChoiceUI::eChoiceType type)
{
   mType = type;
}

void InvitationStateWdg::slotChangeTargetId(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType)
{
   mstrCurId = strId;
   mType = eType;
}

void InvitationStateWdg::on_btnRetry_clicked(bool checked /*= false*/)
{
   emit sig_inviteDoubleVrtc(mstrCurId, mType);
}

void InvitationStateWdg::on_btnReSelection_clicked(bool checked/* = false*/)
{
   emit sig_ReSelection(mType);
}

void InvitationStateWdg::on_btnClose_clicked(bool checked/* = false*/)
{
   emit sig_Close();
}

void InvitationStateWdg::slotOvertime()
{
   SetStateType(eStateType_OverTime);
}

void InvitationStateWdg::stopTimer()
{
   if (nullptr != mpOvertimeTimer && mpOvertimeTimer->isActive()) {
      mpOvertimeTimer->stop();
   }
}

InvitationStateWdg::eStateType InvitationStateWdg::StateType()
{
   return mStateType;
}

void InvitationStateWdg::SetStateType(const eStateType type)
{
   mStateType = type;
   emit sig_StateChanged(mStateType);
   switch (type)
   {
   case eStateType_FrameGeting: {
      ui.labTitle->setText(QStringLiteral("画面获取中，请稍后~"));
      ui.labImage->setStyleSheet("border-image: url(:/widget/img/widget/FrameGeting.png);");
      ui.btnRetry->hide();
      ui.btnReSelection->hide();
      ui.btnClose->hide();
      if (nullptr == mpOvertimeTimer) {
         mpOvertimeTimer = new QTimer(this);
         connect(mpOvertimeTimer, &QTimer::timeout, this, &InvitationStateWdg::slotOvertime);
      }
      if (nullptr != mpOvertimeTimer) {
         mpOvertimeTimer->start(10000);
      }
   }
      break;
   case eStateType_DesktopSharing:
      ui.labTitle->setText(QStringLiteral("桌面共享中~"));
      ui.labImage->setStyleSheet("border-image: url(:/widget/img/widget/desktopSharing.png);");

      ui.btnRetry->hide();
      ui.btnReSelection->hide();
      ui.btnClose->hide();
      break;
   case eStateType_ReleasWheat:
      ui.labTitle->setText(QStringLiteral("双推学员已下麦，请重新选择学员"));
      ui.labImage->setStyleSheet("border-image: url(:/widget/img/widget/StuReleaseWheat.png);");
      ui.btnRetry->hide();
      ui.btnReSelection->show();
      ui.btnClose->show();
      break;
	case eStateType_ReleasVrtc:
		ui.labTitle->setText(QStringLiteral("桌面共享学员已下麦，请重新选择学员"));
		ui.labImage->setStyleSheet("border-image: url(:/widget/img/widget/StuReleaseWheat.png);");
		ui.btnRetry->hide();
		ui.btnReSelection->show();
		ui.btnClose->show();
		break;
   case eStateType_OverTime:
      stopTimer();
      ui.labTitle->setText(QStringLiteral("画面获取超时~"));
      ui.labImage->setStyleSheet("border-image: url(:/widget/img/widget/overTime.png);");
      ui.btnRetry->show();
      ui.btnReSelection->hide();
      ui.btnClose->hide();
      break;
   default:
      break;
   }
}