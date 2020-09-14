#pragma once
#include <QPushButton>
#include <QTimer>

class CTimeButton : public QPushButton
{
	Q_OBJECT

public:
	CTimeButton(QWidget *parent = Q_NULLPTR);
	~CTimeButton();
	void startClassTime();
	void stopClassTime();
	void setHoverDisplayText(const QString& strText);
    long GetClassTime();
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
private slots:
	void slot_UpdateClassTime();
private:

	void dispalyTime();

	int miHour;
	int miBranch;
	int miSecond;

	QTimer* mpClassTime = nullptr;//�Ͽ�ʱ���ʱ��

	long miClassingTime = 0;

	bool mbMouseHoverOn = false;
	QString mstrDisplayText = "";
};

class CCancelCountDownBtn : public QPushButton
{
	Q_OBJECT

public:
	CCancelCountDownBtn(QWidget *parent = Q_NULLPTR);
	~CCancelCountDownBtn();


	void SetNormalText(const QString& str);
	void SetCountDownText(const QString& str);
	void SetCountDownTime(const int& iTime);

	void StartCountDown(const bool& bCountDown);
	void StopCountDown();
signals:
	void sigCliecked(bool state);
protected:

private slots:
	void slotCountDonw();
	void slotclicked();
private:

	void StartTimer();
	QTimer* mpCountDownTimer = nullptr;//�Ͽ�ʱ���ʱ��
	QString mstrNormal;
	QString mstrCountDown;
	int miCountDown;
	int miInitCount;
	bool mbCountDownState = false; //true ����ʱ�У� false������ʾ��������ʱ
};