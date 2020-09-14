#pragma once

#include <QWidget>
#include "ui_DeviceTestingWdg.h"
#include "../Unility/Unility.h"
#include "VhallUI_define.h"
#include "LiveEventInterface.h"
#include "VhallIALiveSettingDlg.h"

enum DEVICE_PAGE_INDEX {
	DEVICE_TESTING = 0,
	DEVICE_CAMERA ,
	DEVICE_MIC,
	DEVICE_SPEAKER,
	DEVICE_RESULT,
};

enum CAMERA_PAGE_INDEX {
	CAMERA_RENDER = 0,
	CAMERA_USING,
	CAMERA_CHANGE,
	CAMERA_FAILED,
};
class DeviceTestingWdg : public CBaseWnd
{
	Q_OBJECT

public:
	DeviceTestingWdg(QWidget *parent = Q_NULLPTR);
	~DeviceTestingWdg();
	CREATE_WND();

	virtual void Notice(const CSTDMapParam &);
	virtual void Notice(const CSTDMapParamPtr &);

	void StartPreView();
   void HandelCustomEvent(QEvent* event);
private slots:
	void slot_Testing();
	void on_cmbcamera_currentIndexChanged(int index);
	void slot_TestScuess();
	void slot_TestFailed();
	void slot_TestPlay();
	void slot_CurrentMicSelectChanged(int);
	void slot_CurrentPlayerSelectChanged(int);
	void slot_ReDevTesting();
	void slot_GoClass();
	void slot_Help();
	void slot_DesktopCaptureValueChanged(int value);
	void slot_PlayOver();
	void slot_MicHasAudio();
protected:
	void customEvent(QEvent* event);
private:
	Ui::DeviceTestingWdg ui;

	bool mbTestCamera = false;
	bool mbTestMic = false;
	bool mbTestSpeaker = false;
	bool mbDevTested = true;
	QTimer* mPlayTimer = nullptr;
	std::list<vhall::AudioDevProperty> micList;
};
