#pragma once

#include <QWidget>
#include "ui_TimerWdg.h"
#include "../Unility/Unility.h"
#include "VhallUI_define.h"
#include <QTimer>
class QMediaPlayer;

class TimerWdg : public CBaseWnd
{
    Q_OBJECT

public:
    TimerWdg(QWidget *parent = Q_NULLPTR);
    ~TimerWdg();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);
    virtual void Notice(const CSTDMapParamPtr &);
	void SetState(const QString& strState );
	void StartCountDown(const QString& strId, const int& miPlay, const int& iCount);

	void  showWdg(const int& roleType);
private slots:
    void slot_ClickedClose();
    void slot_ClickedStart();
    void slot_RecordTimeout();
	void slot_initState();
private:
	void calcDisplayTime();
	void showEdit(const bool & enable = false);
	void enableEdit(const bool & enable = false);
	void requestStartTimer();
	void requestStopTimer();


	enum ePlayEndSound
	{
		ePlayEndSound_No = 0,
		ePlayEndSound_play = 1,
	};

    Ui::TimerWdg ui;
    bool mbIsTeacher;
    QTimer *mpRecordTimerPtr;

	bool mbIsPlayEndSound = false;
	int miTimeCount;
	bool mbClicked = false;
	QMediaPlayer * mpPlayer = nullptr;
};
