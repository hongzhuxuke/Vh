#ifndef H_INVITATIONSTATEWDG_H
#define H_INVITATIONSTATEWDG_H
#include <QObject>

#include "ui_InvitationStateWdg.h"
#include "../Unility/Unility.h"
#include "CWidget.h"
#include "DoublePubStuChoiceUI.h"

class InvitationStateWdg : public CWidget {
   Q_OBJECT   

public:
   explicit InvitationStateWdg(QWidget *parent = 0);
   ~InvitationStateWdg();
   void SetStuId(const QString& stuId);
   void SetType(const DoublePubStuChoiceUI::eChoiceType type);

   enum eStateType
   {
      eStateType_FrameGeting = 0,//画面获取
      eStateType_DesktopSharing, //桌面共享中
      eStateType_ReleasWheat,     //双推学员下麦
		eStateType_ReleasVrtc,     //桌面共享学员下麦
      eStateType_OverTime,             //获取超时
      eStateType_Null
   };

    void SetStateType(const eStateType type);
    eStateType StateType();
   void stopTimer();
signals:
   void sig_inviteDoubleVrtc(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType);
   void sig_ReSelection(const DoublePubStuChoiceUI::eChoiceType& eType);
   void sig_Close();
   void sig_StateChanged(const eStateType& type);
public slots:
   void slotChangeTargetId(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType);
private slots:
   void on_btnRetry_clicked(bool checked = false);
   void on_btnReSelection_clicked(bool checked = false);
   void on_btnClose_clicked(bool checked = false);
   void slotOvertime();
private:
   
private:
	Ui::InvitationStateWdg ui;

   QString mstrCurId = "";
   DoublePubStuChoiceUI::eChoiceType mType;
   QTimer* mpOvertimeTimer = nullptr;
   eStateType mStateType = eStateType_Null;
};


#endif//H_CTRLDOUBLEPUBSTUWDG_H
