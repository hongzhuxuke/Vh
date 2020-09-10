#pragma once

#include <QWidget>
#include "ui_MemberCtrlMaskWdg.h"
#include "CToolTips.h"

#define MIC_OPEN_STATE_CSS "QPushButton:pressed{border-image:url(:/vhallActive/img/vhallactive/mic_click.png);}\
QPushButton:hover{border-image:url(:/vhallActive/img/vhallactive/mic_hover.png);}\
QPushButton{border-image:url(:/vhallActive/img/vhallactive/mic_normal.png);}"

#define MIC_CLOSE_STATE_CSS "QPushButton:pressed{border-image:url(:/vhallActive/img/vhallactive/mic_mute_click.png);}\
QPushButton:hover{border-image:url(:/vhallActive/img/vhallactive/mic_mute_hover.png);}\
QPushButton{border-image:url(:/vhallActive/img/vhallactive/mic_mute.png);}"


#define CAMERA_OPEN_STATE_CSS "QPushButton{border-image:url(:/vhallActive/img/vhallactive/camera_normal.png);}\
QPushButton:hover{border-image:url(:/vhallActive/img/vhallactive/camera_hover.png);}\
QPushButton:pressed{border-image:url(:/vhallActive/img/vhallactive/camera_click.png);}"

#define CAMERA_CLOSE_STATE_CSS "QPushButton{border-image:url(:/vhallActive/img/vhallactive/camera_mute.png);}\
QPushButton:hover{border-image:url(:/vhallActive/img/vhallactive/camera_mute_hover.png);}\
QPushButton:pressed{border-image:url(:/vhallActive/img/vhallactive/camera_mute_click.png);}"

#define USER_TOSPEAK_STATE_CSS "QPushButton{border-image:url(:/vhallActive/img/vhallactive/speak.png);}\
QPushButton:hover{border-image:url(:/vhallActive/img/vhallactive/speak_hover.png);}\
QPushButton:pressed{border-image:url(:/vhallActive/img/vhallactive/speak_click.png);}"

#define USER_KICKOUT_STATE_CSS "QPushButton{border-image:url(:/vhallActive/img/vhallactive/off_speak.png);}\
QPushButton:hover{border-image:url(:/vhallActive/img/vhallactive/off_speak_hover.png);}\
QPushButton:pressed{border-image:url(:/vhallActive/img/vhallactive/off_speak_click.png);}"


class MemberCtrlMaskWdg : public QWidget
{
	Q_OBJECT

public:
	MemberCtrlMaskWdg(QWidget *parent = Q_NULLPTR);
	~MemberCtrlMaskWdg();

    void InitCtrlBtn(bool teacher, bool bIsSelf);
	//������˷�״̬
	void SetMicState(bool open);
	//��������ͷ״̬
	void SetCameraState(bool open);

public slots:
	void hide();

private slots:
	void slot_OnClickedCamera();
	void slot_OnClickedMic();
	void slot_OnClickedSetPen();
	void slot_OnClickedKickOutPublish();
	void slot_OnClickedSetToSpeaker();

   void slot_OnClickedCamerafocusIn();
   void slot_OnClickedSetMainViewfocusIn();
   void slot_OnClickedMicfocusIn();
   void slot_OnClickedSetToSpeakerfocusIn();
   void slot_OnClickedKickOffRoomfocusIn();

   void slot_OnClickedCamerafocusOut();
   void slot_OnClickedSetMainViewfocusOut();
   void slot_OnClickedMicfocusOut();
   void slot_OnClickedSetToSpeakerfocusOut();
   void slot_OnClickedKickOffRoomfocusOut();

   void slot_OnClickedFloatRender();

protected:
	virtual void leaveEvent(QEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);

	void OnEnteCamera();
	void OnEnteMainView();
	void OnEnteMic();
	void OnEnteToSpeaker();
	void OnEnteKickOffRoom();
signals:
	void sig_clickedCamera(bool);
	void sig_clickedMic(bool);
	void sig_clickedSetMainView(); //����Ϊ������
	void sig_clickedKickOutPublish(); //����
	void sig_clickedSetToSpeaker();//��Ϊ������
    void sig_clickedFloatRender();

private:
	Ui::MemberCtrlMaskWdg ui; 
	bool mbEnableCtrl = false;

	bool mbMicOpen = false;
	bool mbCameraOpen = false;
	CToolTips* mpTips = NULL;
};
