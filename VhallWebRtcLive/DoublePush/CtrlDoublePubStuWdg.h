#ifndef H_CTRLDOUBLEPUBSTUWDG_H
#define H_CTRLDOUBLEPUBSTUWDG_H
#include <QObject>

#include "ui_CtrlDoublePubStuWdg.h"
#include "../Unility/Unility.h"
#include "CWidget.h"
#include "DoublePubStuChoiceUI.h"

class CtrlDoublePubStuWdg : public CWidget {
   Q_OBJECT   

public:
   explicit CtrlDoublePubStuWdg(QWidget *parent = 0);
   ~CtrlDoublePubStuWdg();
   void initItem(const DoublePubStuChoiceUI::eChoiceType& mType);
   void setCurrentDoubleStu(const QString& strId);
   void AddUser(const CDoubleVrtcUsers& vrtcUser);
   QString CurDoubleStuId();
   bool isInitDoublePushComb();
   void setEnabledSig(const bool& bEnabled);
   void setCurComboBoxById(const QString& strId);
	void ChangeUser();
	void stopPush(const bool bAsk = true);
   void SetType(const DoublePubStuChoiceUI::eChoiceType& Type);
	QString GetCurrentStrId();
	const DoublePubStuChoiceUI::eChoiceType&  GetCurrentType();
   bool isChangeUser();
   //void StopDoublePush();
signals:
   //void sig_close();
   void sig_CameraDoublePush(bool start);
	void sig_StopDoublePush();
   void sig_inviteDoubleVrtc(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType);

public slots:
   void slot_DeleteItem(const QString& strID);
   void slot_cmbUserList_currentIndexChanged(int index);
   void on_btnPre_clicked(bool checked = false);
   void on_btnNext_clicked(bool checked = false);
   void slot_StopPush();
   void slot_StopDesktopSharing();
   void slot_StartStuDoublePush(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType);
   void on_btnClose_clicked(bool checked /*= false*/);
public Q_SLOTS:
   void show();
   void hide();
private:
	bool isExitStu(const QString& strId);

	Ui::CtrlDoublePubStuWdg ui;

   QString mCurrDoubleStu = "";
   DoublePubStuChoiceUI::eChoiceType mType = DoublePubStuChoiceUI::eChoiceType::eChoiceType_Invalid;
	bool mbChange = false; //当前是否切换桌面共享学员
	QString mCurrentStrId;   //当前共享学员ID
};


#endif//H_CTRLDOUBLEPUBSTUWDG_H
