#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "../Unility/Unility.h"
#include "ui_LoginWdg.h"

class QGraphicsDropShadowEffect;

#define STATUS_LOGO_WIDTH   68
#define STATUS_LOGO_MIN_WIDTH   56
#define STATUS_LOGO_HEIGHT  24

enum LOGIN_ERR_CODE {
    LOGIN_ERR_CODE_PARAM = 400,             // �����������
    LOGIN_ERR_CODE_CLASS_NOT_EXIST = 1005,  // �γ���Ϣ������
    LOGIN_ERR_CODE_PWD_ERR = 1002,          // �������
    LOGIN_ERR_CODE_JOIN_ERR = 1009,         // �γ̽���ʧ��
    LOGIN_ERR_CODE_NO_JOIN_PER = 1004,	    // ���û�����Ȩ�޽���
    LOGIN_ERR_CODE_NO_SOURCE = 1007,	    // ����Ȩ�޽��룬��鿴�˻�������ݽ�ɫ
    LOGIN_ERR_CODE_KICK_OUT = 1001,	        // ���û��ѱ��߳�
};

class CAboutDlg;
class LoginWdg : public CBaseWnd
{
    Q_OBJECT

public:
    LoginWdg(QWidget *parent = Q_NULLPTR);
    ~LoginWdg();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);

public slots:
	void show();
public Q_SLOTS:
	void hide();
	bool close();

protected:
    virtual void customEvent(QEvent *);
    void closeEvent(QCloseEvent*);
    void HandleGetBaseInfo(QEvent* event);
    void HandleJoinClass(QEvent* event);

    void keyPressEvent(QKeyEvent *event);
	void LoginIdentity(bool bSpecial);
private slots:
	void slot_JoinRoom();
	void slot_GetClassInfo();
	void slot_Setting();
	void slot_Close();
	void slot_Min();
	void on_btnAbout_clicked(bool checked = false);
	void slot_LineEditRoomIdFocusIn();
	void slot_PwdEditRoomIdFocusIn();
	void slot_NickNameEditRoomIdFocusIn();
	void slot_TurnToRoomIdPage();
	void slot_ChangeUser();
private:
    void ReSizeStatusSize(int width, int height);
    void ChangeLoginPage(int pageIndex);

private:
    Ui::LoginWdg ui;
    std::list<std::string> sendlist_;
	//CAboutDlg* mpAboutDlg = NULL;
	QGraphicsDropShadowEffect* mpEffect = nullptr;
	int mLoginMode = -1;
	bool mLoginIdentity = false;   //��¼ѡ����� falseѧԱ true��ʦ�α�����
};
