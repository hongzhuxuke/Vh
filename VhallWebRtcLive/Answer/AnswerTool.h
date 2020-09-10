#pragma once

#include <QWidget>
#include "ui_AnswerTool.h"
#include "../Unility/Unility.h"
#include "VhallUI_define.h"
#include <QTimer>


class AnswerStatistics;
class AnswerTool : public CBaseWnd
{
    Q_OBJECT

public:
    AnswerTool(QWidget *parent = Q_NULLPTR);
    ~AnswerTool();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);
    virtual void Notice(const CSTDMapParamPtr &);
    void setFixedSize(int w, int h);
private slots:
    void slot_ClickedA();
    void slot_ClickedB();
    void slot_ClickedC();
    void slot_ClickedD();
    void slot_ClickedE();
    void slot_ClickedF();
    void slot_ClickedG();
    void slot_ClickedH();

    void slot_ClickedAdd();
    void slot_ClickedDel();
    void slot_ClickedStart();
    void slot_ClickedClose();
	 void slot_ReAnswer();

    void slot_ClickedStartTime();
    void slot_ClickedStudentCommit();
    void slot_ClickedTeacherCommitAnswer();
    void slot_ClickedTeacherOver();

private:
    void SelectBtn(QString selectIndex);
	 void CloseExec();
	 void ReLayout();
private:

    Ui::AnswerTool ui;

    std::map<QString,QPushButton*> mSelectBtnMap;
	 QList<QPushButton*> SelectBtnList;
	 std::map<QString, QString> mBtnIdMap;
	 std::map<QString, AnswerStatistics*> mAnswerIdMap;
    int mCount = 0;
    int mSelectIndex = 0;
    QTimer *mpStartTimer = nullptr;
    int mHour = 0;
    int mMin = 0;
    int mSecond = 0;
	int mTotal = 0;
    bool mbIsTeacher = false;
	 QString mQuesId;
	 int mSelectCount = 0;
	 bool mbDetails = false;
	 bool mbFirstNotice = false;
};
