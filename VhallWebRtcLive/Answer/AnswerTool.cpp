#include "AnswerTool.h"
#include "../Unility/CustomEventType.h"
#include "../Unility/Unility.h"
#include "AnswerStatistics.h"
#include "StudentStat.h"
#include "TipsWdg.h"
#include <QDateTime>

#define LABEL_STARTRIGHT   QStringLiteral("QLabel{color:rgb(85, 170, 127);font: 12pt \"微软雅黑\";}")
#define LABEL_STARTWEONG   QStringLiteral("QLabel{color:rgb(255, 0, 0);font: 12pt \"微软雅黑\";}")
#define LABEL_STARTANSWERRIGHT   QStringLiteral("QLabel{color:rgb(85, 170, 127);font: 12pt \"微软雅黑\";}")
#define LABEL_STARTANSWERWEONG   QStringLiteral("QLabel{color:rgb(252,86,89);font: 12pt \"微软雅黑\";}")

#define NO_SELECT_STYLE     QString("QPushButton{border-top-left-radius : 30px; \
border-top-right-radius:30px; \
border-bottom-left-radius:30px; \
border-bottom-right-radius : 30px;\
background-color:rgb(244, 244, 244); \
font: 20px \"\345\276\256\350\275\257\351\233\205\351\273\221\";\
color: rgb(68,68,68);}\
QPushButton:hover{\
border-top-right-radius:30px;\
border-bottom-left-radius:30px;\
border-bottom-left-radius:30px;\
border-bottom-right-radius:30px;\
background-color: rgb(244, 244, 244);\
font: 20px \"\345\276\256\350\275\257\351\233\205\351\273\221\";\
color: rgb(82, 204, 144); }")

#define SELECT_STYLE    QString("QPushButton{border-top-left-radius : 30px; \
border-top-right-radius:30px; \
border-bottom-left-radius:30px;\
border-bottom-right-radius:30px;\
background-color:rgb(82, 204, 144);\
font: 20px \"\345\276\256\350\275\257\351\233\205\351\273\221\";\
color:rgb(255,255,255);}")
//QPushButton:hover{\
//border-top-right-radius:30px;\
//border-bottom-left-radius:30px;\
//border-bottom-left-radius:30px;\
//border-bottom-right-radius:30px;\
//background-color: rgb(244, 244, 244);\
//font: 20px \"\345\276\256\350\275\257\351\233\205\351\273\221\";\
//color: rgb(82, 204, 144); }")

#define ANSWER_STYLE    QString("border-top-left-radius : 30px; \
border-top-right-radius:30px; \
border-bottom-left-radius:30px;\
border-bottom-right-radius:30px;\
background-color:rgb(252,86,89);\
font: 20px \"\345\276\256\350\275\257\351\233\205\351\273\221\";\
color:rgb(255,255,255);")
#define BTN_ADD_STSRT_START   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         border-image:url(:/button/img/button/AnswerAdd.png);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}\
                                         QPushButton:hover{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         border-image:url(:/button/img/button/AnswerAdd_hover.png);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}")  
#define BTN_ADD_STSRT_START_ENADLE   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         border-image:url(:/button/img/button/Answer_ add_disable.png);\
                                         font: 10 pt  \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}") 
#define BTN_DEL_STSRT_START   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         border-image:url(:/button/img/button/Answer_minus.png);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}\
                                         QPushButton:hover{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         border-image:url(:/button/img/button/Answer_minus_hover.png);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}")  
#define BTN_DEL_STSRT_START_ENADLE   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         border-image:url(:/button/img/button/Answer_ minus_disable.png);\
                                         font: 10 pt  \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}") 

#define BTN_STSRT_START   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         background-color: rgb(82, 204, 144);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}\
                                         QPushButton:hover{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         background-color: rgb(99,224,162);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}")  
#define BTN_STSRT_START_ENADLE   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         background-color: rgb(170,230,200);\
                                         font: 10 pt  \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}") 

#define TOTAL(x)  QStringLiteral("当前在线  : <font color=#52CC90>") \
	+ QString::number(x) +  QStringLiteral(" </font>人")

#define SUBMIT(x)  QStringLiteral("已回答  : <font color=#52CC90>") \
	+ QString::number(x) +  QStringLiteral(" </font>人")

#define STARTTIME(x)  QStringLiteral("开始答题 : <font color=#52CC90>") \
	+ (x)

#define STARTEDTIME(x)  QStringLiteral("已开始 : <font color=#52CC90>") \
	+ (x)

#define StrTeacherCommitAnswer QString::fromWCharArray(L"公布答案")
#define StrTeacherWatchDetail  QString::fromWCharArray(L"查看详情")
#define StrTeacherCloseDetail  QString::fromWCharArray(L"收起详情")
#define StrStudentStayAnswer  QString::fromWCharArray(L"已提交，等待公布答案")
#define StrAnswerRight  QString::fromWCharArray(L"回答正确")
#define StrAnswerWrong QString::fromWCharArray(L"回答错误")
#define StrAnswerTimeOut QString::fromWCharArray(L"回答超时")

#define QLABEL_ICON_QNSWER_RIGHT    QStringLiteral("#label_icon{border-image:url(:/idention/img/idention/answerRight.png);}")
#define QLABEL_ICON_QNSWER_WRONG    QStringLiteral("#label_icon{border-image:url(:/idention/img/idention/answerWrong.png);}")
#define QLABEL_ICON_QNSWER_TIMEOUT    QStringLiteral("#label_icon{border-image:url(:/idention/img/idention/answerTimeout.png);}")
#define QLABEL_ICON_QNSWER_COMMIT    QStringLiteral("#label_icon{border-image:url(:/idention/img/idention/answerCommit.png);}")

#define WIDGET_START_RIGHT   QStringLiteral("#widget_stuTimerCount{background-color: rgb(237,249,243);border:1px solid #52CC90;}")
#define WIDGET_START_WEONG   QStringLiteral("#widget_stuTimerCount{background-color: rgb(254,238,238);border:1px solid #FC5659;}")
#define WIDGET_START_TIMEOUT   QStringLiteral("#widget_stuTimerCount{background-color: rgb(255,244,229);border:1px solid #FF9800;}")

#define LABEL_ANSWER_RIGHT   QStringLiteral("QLabel{color:#52CC90;font: 9pt \"微软雅黑\";}")
#define LABEL_ANSWER_WEONG   QStringLiteral("QLabel{color:#FC5659;font: 9pt \"微软雅黑\";}")
#define LABEL_ANSWER_TIMEOUT   QStringLiteral("QLabel{color:#FF9800;font: 9pt \"微软雅黑\";}")

enum SELECT_INDEX {
    BTN_A = 0,
    BTN_B,
    BTN_C,
    BTN_D,
    BTN_E,
    BTN_F,
    BTN_G,
    BTN_H,
};

#define MAX_BTN_COUNT   8
extern ToolManager *globalToolManager;


CREATE_WND_FUNCTION(AnswerTool);
AnswerTool::AnswerTool(QWidget *parent)
    : CBaseWnd(parent)
    , mCount(4)
{

    ui.setupUi(this);
	//setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);
    ui.widget_bottomSelect->hide();
    connect(ui.pushButton_A, SIGNAL(clicked()), this, SLOT(slot_ClickedA()));
    connect(ui.pushButton_B, SIGNAL(clicked()), this, SLOT(slot_ClickedB()));
    connect(ui.pushButton_C, SIGNAL(clicked()), this, SLOT(slot_ClickedC()));
    connect(ui.pushButton_D, SIGNAL(clicked()), this, SLOT(slot_ClickedD()));
    connect(ui.pushButton_E, SIGNAL(clicked()), this, SLOT(slot_ClickedE()));
    connect(ui.pushButton_F, SIGNAL(clicked()), this, SLOT(slot_ClickedF()));
    connect(ui.pushButton_G, SIGNAL(clicked()), this, SLOT(slot_ClickedG()));
    connect(ui.pushButton_H, SIGNAL(clicked()), this, SLOT(slot_ClickedH()));

    connect(ui.pushButton_ADD, SIGNAL(clicked()), this, SLOT(slot_ClickedAdd()));
    connect(ui.pushButton_DEL, SIGNAL(clicked()), this, SLOT(slot_ClickedDel()));
    connect(ui.pushButton_start, SIGNAL(clicked()), this, SLOT(slot_ClickedStart()));

    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_ClickedClose()));     
    connect(ui.pushButton_restartanswer, SIGNAL(clicked()), this, SLOT(slot_ReAnswer()));

    connect(ui.pushButton_studentCommit, SIGNAL(clicked()), this, SLOT(slot_ClickedStudentCommit()));
    connect(ui.pushButton_teacherCommitAllAnswer, SIGNAL(clicked()), this, SLOT(slot_ClickedTeacherCommitAnswer()));
    connect(ui.pushButton_over, SIGNAL(clicked()), this, SLOT(slot_ClickedClose()));

    mpStartTimer = new QTimer(this);
    if (mpStartTimer) {
        connect(mpStartTimer, SIGNAL(timeout()), this, SLOT(slot_ClickedStartTime()));
    }

    mSelectBtnMap["A"] = ui.pushButton_A;
    mSelectBtnMap["B"] = ui.pushButton_B;
    mSelectBtnMap["C"] = ui.pushButton_C;
    mSelectBtnMap["D"] = ui.pushButton_D;
    mSelectBtnMap["E"] = ui.pushButton_E;
    mSelectBtnMap["F"] = ui.pushButton_F;
    mSelectBtnMap["G"] = ui.pushButton_G;
    mSelectBtnMap["H"] = ui.pushButton_H;
	 ui.pushButton_C->hide();
	 ui.pushButton_D->hide();
    ui.pushButton_E->hide();
    ui.pushButton_F->hide();
    ui.pushButton_G->hide();
    ui.pushButton_H->hide();
    //SelectBtn("B");
	 ui.pushButton_start->setStyleSheet(BTN_STSRT_START_ENADLE);
	 ui.pushButton_start->setEnabled(false);

	 ui.pushButton_studentCommit->setStyleSheet(BTN_STSRT_START_ENADLE);
	 ui.pushButton_studentCommit->setEnabled(false);
    ui.stackedWidget->setCurrentIndex(0);

    ui.label_stuCommitNotice->hide();
    ui.label_stu_commit_answer->hide();
	 ui.label_omit->hide();
    ui.label_teacherCommitNotice->hide();
    ui.label_teacherCommitAnswer->hide();
    ui.pushButton_restartanswer->hide();
	 ui.label_icon->hide();
	 //ui.listWidget_answerStat->setItemAlignment(Qt::AlignHCenter);

	 QFont font("Microsoft YaHei", 14, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
	 ui.label_accuracy->setFont(font);
	 ui.label_right_count->setFont(font);
	 ui.label_err_count->setFont(font);

    ui.label_bottom->hide();
    ui.label_left->hide();
    ui.label_right->hide();
    ui.label_top->hide();

	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
		ui.pushButton_close->hide();
	}

   
   setFixedSize(456, 548);

}

AnswerTool::~AnswerTool()
{
}

void AnswerTool::SelectBtn(QString selectIndex) {
    std::map<QString, QPushButton*>::iterator iter = mSelectBtnMap.find(selectIndex);
    while (iter != mSelectBtnMap.end()) {
        if (iter->second->styleSheet() != SELECT_STYLE) {
            iter->second->setStyleSheet(SELECT_STYLE);
				mSelectCount++;
				if (mbIsTeacher) {
					ui.pushButton_start->setStyleSheet(BTN_STSRT_START);
					ui.pushButton_start->setEnabled(true);
				}
				else {
					ui.pushButton_studentCommit->setStyleSheet(BTN_STSRT_START);
					ui.pushButton_studentCommit->setEnabled(true);
				}
        }
        else {
            iter->second->setStyleSheet(NO_SELECT_STYLE);
				mSelectCount--;
				if (mSelectCount == 0) {
					if (mbIsTeacher) {
						ui.pushButton_start->setStyleSheet(BTN_STSRT_START_ENADLE);
						ui.pushButton_start->setEnabled(false);
					}
					else {
						ui.pushButton_studentCommit->setStyleSheet(BTN_STSRT_START_ENADLE);
						ui.pushButton_studentCommit->setEnabled(false);
					}				
				}
        }
        break;
    }
}

void AnswerTool::Notice(const CSTDMapParam &mapParam) {

	if (!mbFirstNotice) {
		setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint /*| Qt::WindowStaysOnTopHint*/);
		//setAttribute(Qt::WA_TranslucentBackground);
		//mbFirstNotice = true;
	}

	std::map<QString, QString>::const_iterator iter = mapParam.find("detail");
	if(iter != mapParam.end() && iter->second == "1"){
		iter = mapParam.find(Str_Function_AnswerToolItems);
		if (iter != mapParam.end()) {
			QJsonDocument doc = QJsonDocument::fromJson(iter->second.toUtf8());
			QJsonObject obj = doc.object();

			if (obj.contains("data") && obj["data"].isObject()) {

				QJsonObject objlist = obj["data"].toObject();
				if (objlist.contains("list") && objlist["list"].isArray()) {

					QJsonArray array = objlist["list"].toArray();
					QString answers;
					QString nick_name;
					int answer_time = 0;
					bool bright = false;
					int answer_right = 0;
					for (int i = 0;i < array.size(); i++) {
						if (array.at(i).isObject()) {
                     QJsonObject json_obj = array[i].toObject();
							answers = json_obj["answers"].toString();
							nick_name = json_obj["nick_name"].toString();
							answer_time = json_obj["answer_time"].toInt();
							if (json_obj["is_right"].toInt() == 1) {
								++answer_right;
								bright = true;
							}
							else {
								bright = false;
							}
						}

						StudentStat* answer = new StudentStat(ui.listWidget_stulist);
						QListWidgetItem *itemWidget = new QListWidgetItem(ui.listWidget_stulist);
						itemWidget->setSizeHint(QSize(0, 40));
						ui.listWidget_stulist->addItem(itemWidget);
						ui.listWidget_stulist->setItemWidget(itemWidget, answer);
						int min = answer_time / 60;
						answer_time = answer_time - min * 60;
						int hour = 0;
						if (min >= 60) {
							hour = min / 60;
						}
						QString mSecondTimeText;
						if (answer_time >= 10) {
							mSecondTimeText = QString::number(answer_time);
						}
						else {
							mSecondTimeText = QString("0%1").arg(answer_time);
						}

						QString mMinTimeText;
						if (min >= 10) {
							mMinTimeText = QString::number(min);
						}
						else {
							mMinTimeText = QString("0%1").arg(min);
						}

						QString mmHourTimeText;
						if (hour >= 10) {
							mmHourTimeText = QString::number(hour);
						}
						else {
							mmHourTimeText = QString("0%1").arg(hour);
						}
						QString timeText = QString::fromWCharArray(L"%1:%2:%3").arg(mmHourTimeText).arg(mMinTimeText).arg(mSecondTimeText);
						answer->SetStudentAnswerInfo(nick_name, answers, timeText, bright);
					}
					ui.label_right_count->setText(QString::number(answer_right) + QString::fromWCharArray(L"人"));
					ui.label_err_count->setText(QString::number(array.size() - answer_right) + QString::fromWCharArray(L"人"));
					QString accuracy;
					if (array.size() == 0) {
						accuracy = QString::fromWCharArray(L"0%");
					}
					else {
						accuracy = QString::number(answer_right * 100 / array.size()) + "%";
					}
					QFont font("Microsoft YaHei", 14, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
					ui.label_accuracy->setFont(font);
					ui.label_accuracy->setText(accuracy);
					ui.pushButton_teacherCommitAllAnswer->setText(StrTeacherCloseDetail);
					return;
		      }
	      }
		}
	}
	iter = mapParam.find("close");
	if (iter != mapParam.end() && iter->second == "1") {
		CloseExec();
	}
	iter = mapParam.find("create");
	if (iter != mapParam.end() && iter->second == "1") {
		iter = mapParam.find(Str_Function_AnswerToolItems);
		if (iter != mapParam.end()) {
			QJsonDocument doc = QJsonDocument::fromJson(iter->second.toUtf8());
			QJsonObject obj = doc.object();

			if (obj.contains("data") && obj["data"].isObject()) {

				QJsonObject quesId = obj["data"].toObject();
				if (quesId.contains("ques_id")) {
					mQuesId = QString::number(quesId["ques_id"].toInt());
				}
				iter = mapParam.find("online_num");
				if (iter != mapParam.end()) {
					mTotal = iter->second.toInt();
					if (mTotal <= 0) {
						ui.label_online->setText(TOTAL(0));
					}
					else {
						ui.label_online->setText(TOTAL(mTotal));
					}
					ui.label_submit->setText(SUBMIT(0));
				}
				return;
			}
		}
	}
	iter = mapParam.find("new_push_question");
	if (iter != mapParam.end() && iter->second == "1") {
		iter = mapParam.find("online_num");
		if (iter != mapParam.end()) {
			mTotal = iter->second.toInt();
			if (mTotal <= 0) {
				ui.label_online->setText(TOTAL(0));
			}
			else {
				ui.label_online->setText(TOTAL(mTotal));
			}
			ui.label_submit->setText(SUBMIT(0));
		}
		return;
	}
	iter = mapParam.find(Str_Function);
	if (iter != mapParam.end() && iter->second == Str_Function_AnswerToolTeacher) {
		ui.pushButton_C->show();
		ui.pushButton_D->show();
		iter = mapParam.find(Str_Function_AnswerToolItems);
		if (iter != mapParam.end()) {
			//解析json
			QString items = iter->second;
			QJsonDocument jsonDocument = QJsonDocument::fromJson(items.toUtf8());
			if (jsonDocument.isArray()) {
				iter = mapParam.find("online_num");
				if (iter != mapParam.end()) {
					mTotal = iter->second.toInt();
					if (mTotal <= 0) {
						ui.label_online->setText(TOTAL(0));
					}
					else {
						ui.label_online->setText(TOTAL(mTotal));
					}				
				}
				QJsonArray jsonArray = jsonDocument.array();
				for (int i = 0;i < jsonArray.size(); i++) {
					QJsonObject obj = jsonArray.at(i).toObject();
					if (obj.contains("subject")) {
						std::map<QString, AnswerStatistics*>::const_iterator ite = mAnswerIdMap.find(obj["subject"].toString());
						if (ite != mAnswerIdMap.end() && obj.contains("answer_num")) {
							ite->second->SetAnswerPerson(obj["answer_num"].toInt(), mTotal);
						}

					}
				}
			}
			iter = mapParam.find("answers");
			if (iter != mapParam.end()) {
				int answers = iter->second.toInt();
				if (answers <= 0) {
					ui.label_submit->setText(SUBMIT(0));
				}
				else {
					ui.label_submit->setText(SUBMIT(answers));
				}
			
			}
			//iter = mapParam.find("ques_id");
			//if (iter != mapParam.end()) {
			//	mQuesId = iter->second;
			//}
		}
		else {
			ui.widget_stuTimerCount->hide();
			ui.widget_stuCommitAnswer->hide();
			ui.label_left->setFixedWidth(15);
			ui.label_right->setFixedWidth(15);

			//this->setFixedWidth(452);
			//this->setFixedHeight(392);
         setFixedSize(452, 392);
			mbIsTeacher = true;
		}
	}
		else if (iter != mapParam.end() && iter->second == Str_Function_AnswerToolStudent) {
		   ui.widget_teacherStartAnswer->hide();
			ui.label_selectNotice->hide();
			//this->setFixedWidth(420);
			//this->setFixedHeight(350);
         setFixedSize(420, 370);

			ui.label_left->setFixedWidth(13);
			ui.label_right->setFixedWidth(13);
			mbIsTeacher = false;
			iter = mapParam.find(Str_Function_AnswerToolItems);
			if (iter != mapParam.end()) {
				//解析json
				QString items = iter->second;
				QJsonDocument jsonDocument = QJsonDocument::fromJson(items.toUtf8());
				if (jsonDocument.isArray()) {
					QJsonArray jsonArray = jsonDocument.array();
					bool bover = false;
					QString answer;
					for (int i = 0;i < jsonArray.size(); i++) {
						QJsonObject obj = jsonArray.at(i).toObject();
						if (obj.contains("is_answer")) {
							if (obj["is_answer"].toInt() == 1)
							{
								if (answer.isEmpty()) {
									answer = obj["subject"].toString();
								}
								else {
									answer += QString::fromWCharArray(L"、") + obj["subject"].toString();
								}
								std::map<QString, QPushButton*>::iterator iter = mSelectBtnMap.find(obj["subject"].toString());
								if (iter != mSelectBtnMap.end()) {
									iter->second->setStyleSheet(SELECT_STYLE);
									if (SelectBtnList.contains(iter->second)) {
										SelectBtnList.removeOne(iter->second);
									}
									
								}		
							}							
						}
						else{
							bover = true;
							if (obj.contains("subject") && obj.contains("id")) {
								mBtnIdMap[obj["subject"].toString()] = QString::number(obj["id"].toInt());
							}
						}
					}
					if (bover) {
						if (this->isVisible()) {
							ReLayout();
						}
						mCount = jsonArray.size();
						if (mCount >= 3) {
							ui.pushButton_C->show();
						}
						if (mCount >= 4) {
							ui.pushButton_D->show();
						}
						if (mCount >= 5) {
							ui.widget_bottomSelect->show();
							ui.pushButton_E->show();
						}
						if (mCount >= 6) {
							ui.pushButton_F->show();
						}
						if (mCount >= 7) {
							ui.pushButton_G->show();
						}
						if (mCount >= 8) {
							ui.pushButton_H->show();
						}
						if (mpStartTimer) {
							mpStartTimer->start(1000);
						}
					}
					else {		
						if (SelectBtnList.size() > 0) {
							for (int i = 0;i < SelectBtnList.size();i++) {
								SelectBtnList.at(i)->setStyleSheet(ANSWER_STYLE);
							}
							SelectBtnList.clear();
						}

						this->setFixedHeight(370);
						ui.label_teacherCommitNotice->show();
						ui.label_teacherCommitAnswer->show();
						ui.label_teacherCommitAnswer->setText(answer);
						//answer = answer.replace(QRegExp("[\r\n]"), "");

                  int wndWidth = fontMetrics().width(answer) * 3/2 + 10;
						ui.label_teacherCommitAnswer->setFixedWidth(wndWidth);
						int omit = 340 - ui.label_teacherCommitAnswer->width() - ui.label_teacherCommitNotice->width() - ui.label_stuCommitNotice->width();
						if (wndWidth > omit || fontMetrics().width(ui.label_stu_commit_answer->text()) * 3 / 2 > omit) {
							QString temp = ui.label_stu_commit_answer->text();
							//ui.label_stu_commit_answer->setFixedWidth();
							temp = temp.left(omit * 2 / 3 / 16);
							QString end = temp.at(temp.size() - 1);
							if (end == QString::fromLocal8Bit("、"))
							{
								temp = temp.left(temp.size() - 1);
							}
							ui.label_stu_commit_answer->setText(temp);
							ui.label_omit->show();
						}

						ui.label_icon->show();
						if (ui.label_stu_commit_answer->text() == answer) {
							ui.label_stu_commit_answer->setStyleSheet(LABEL_STARTANSWERRIGHT);
							ui.label_omit->setStyleSheet(LABEL_STARTANSWERRIGHT);
							ui.label_studentAnswerTimer->setText(StrAnswerRight);
							ui.label_icon->setStyleSheet(QLABEL_ICON_QNSWER_RIGHT);
							ui.widget_stuTimerCount->setStyleSheet(WIDGET_START_RIGHT);
							ui.label_studentAnswerTimer->setStyleSheet(LABEL_ANSWER_RIGHT);
						}															 
						else if(ui.label_stu_commit_answer->text() == ""){
							mpStartTimer->stop();
							ui.label_stu_commit_answer->setStyleSheet(LABEL_STARTANSWERWEONG);
							ui.label_omit->setStyleSheet(LABEL_STARTANSWERWEONG);
							ui.label_studentAnswerTimer->setText(StrAnswerTimeOut);
							ui.label_studentAnswerTimer->setStyleSheet(LABEL_ANSWER_TIMEOUT);
							ui.label_icon->setStyleSheet(QLABEL_ICON_QNSWER_TIMEOUT);
							ui.widget_stuTimerCount->setStyleSheet(WIDGET_START_TIMEOUT);
							ui.pushButton_studentCommit->hide();
							ui.label_omit->hide();
						}
						else if (ui.label_stu_commit_answer->text() != answer) {
							ui.label_stu_commit_answer->setStyleSheet(LABEL_STARTANSWERWEONG);
							ui.label_omit->setStyleSheet(LABEL_STARTANSWERWEONG);
							ui.label_studentAnswerTimer->setText(StrAnswerWrong);
							ui.label_icon->setStyleSheet(QLABEL_ICON_QNSWER_WRONG);
							ui.widget_stuTimerCount->setStyleSheet(WIDGET_START_WEONG);
							ui.label_studentAnswerTimer->setStyleSheet(LABEL_ANSWER_WEONG);
						}
						else {
							ui.label_studentAnswerTimer->setText(StrAnswerWrong);
							ui.label_studentAnswerTimer->setStyleSheet(LABEL_ANSWER_WEONG);
							ui.label_icon->setStyleSheet(QLABEL_ICON_QNSWER_WRONG);
							ui.widget_stuTimerCount->setStyleSheet(WIDGET_START_WEONG);
							ui.label_stu_commit_answer->setStyleSheet(LABEL_STARTANSWERRIGHT);
							ui.label_omit->setStyleSheet(LABEL_STARTANSWERRIGHT);
						}
					}

				}

			}
			iter = mapParam.find("ques_id");
			if (iter != mapParam.end()) {
				mQuesId = iter->second;
			}
		}
	
}
void AnswerTool::Notice(const CSTDMapParamPtr &) {

}

void AnswerTool::setFixedSize(int w, int h)
{
   ui.widget->setFixedSize(w-2, h-2);
   CBaseWnd::setFixedSize(w, h);
}

void AnswerTool::slot_ClickedA() {
    SelectBtn("A");
}

void AnswerTool::slot_ClickedB() {
    SelectBtn("B");
}

void AnswerTool::slot_ClickedC() {
    SelectBtn("C");
}

void AnswerTool::slot_ClickedD() {
    SelectBtn("D");
}

void AnswerTool::slot_ClickedE() {
    SelectBtn("E");
}

void AnswerTool::slot_ClickedF() {
    SelectBtn("F");
}

void AnswerTool::slot_ClickedG() {
    SelectBtn("G");
}

void AnswerTool::slot_ClickedH() {
    SelectBtn("H");
}

void AnswerTool::slot_ClickedAdd() {
    if (mCount >= MAX_BTN_COUNT) {
        return;
    }
	 ui.pushButton_DEL->setStyleSheet(BTN_DEL_STSRT_START);
    mCount++;
    if (mCount == 3) {
        ui.pushButton_C->show();
    }
    else if (mCount == 4) {
        ui.pushButton_D->show();
    }
    if (mCount >= 5) {
        ui.widget_bottomSelect->show();
        if (mCount == 5) {
            ui.pushButton_E->show();
        }
        else if (mCount == 6) {
            ui.pushButton_F->show();
        }
        else if (mCount == 7) {
            ui.pushButton_G->show();
        }
        else if (mCount == 8) {
            ui.pushButton_H->show();
				ui.pushButton_ADD->setStyleSheet(BTN_ADD_STSRT_START_ENADLE);
        }
    }
}

void AnswerTool::slot_ClickedDel() {
    if (mCount <= 2) {
        return;
    }
	 ui.pushButton_ADD->setStyleSheet(BTN_ADD_STSRT_START);
    mCount--;
    if (mCount == 2) {
        ui.pushButton_C->hide();
        if (ui.pushButton_C->styleSheet() == SELECT_STYLE) {
			  ui.pushButton_C->setStyleSheet(NO_SELECT_STYLE);
            --mSelectCount;
        }
		  ui.pushButton_DEL->setStyleSheet(BTN_DEL_STSRT_START_ENADLE);
    }
    else if (mCount == 3) {
        ui.pushButton_D->hide();
		  if (ui.pushButton_D->styleSheet() == SELECT_STYLE) {
			  ui.pushButton_D->setStyleSheet(NO_SELECT_STYLE);
			  --mSelectCount;
		  }
    }
    if (mCount >= 4) {
        if (mCount == 4) {
            ui.pushButton_E->hide();
            ui.widget_bottomSelect->hide();
				if (ui.pushButton_E->styleSheet() == SELECT_STYLE) {
					ui.pushButton_E->setStyleSheet(NO_SELECT_STYLE);
					--mSelectCount;
				}
        }
        else if (mCount == 5) {
            ui.pushButton_F->hide();
				if (ui.pushButton_F->styleSheet() == SELECT_STYLE) {
					ui.pushButton_F->setStyleSheet(NO_SELECT_STYLE);
					--mSelectCount;
				}
        }
        else if (mCount == 6) {
            ui.pushButton_G->hide();
				if (ui.pushButton_G->styleSheet() == SELECT_STYLE) {
					ui.pushButton_G->setStyleSheet(NO_SELECT_STYLE);
					--mSelectCount;
				}
        }
        else if (mCount == 7) {
            ui.pushButton_H->hide();
				if (ui.pushButton_H->styleSheet() == SELECT_STYLE) {
					ui.pushButton_H->setStyleSheet(NO_SELECT_STYLE);
					--mSelectCount;
				}
        }
    }
	 if (mSelectCount == 0) {
		 ui.pushButton_start->setStyleSheet(BTN_STSRT_START_ENADLE);
		 ui.pushButton_start->setEnabled(false);
	 }
}

void AnswerTool::slot_ClickedStart() {

    if (mpStartTimer) {
        mpStartTimer->start(1000);
    }

    //this->setFixedWidth(596);
    //this->setFixedHeight(520);
    setFixedSize(596, 520);
	ui.label_left->setFixedWidth(18);
	ui.label_right->setFixedWidth(18);
	ui.label_name->setFixedWidth(160);
	ui.label_answer->setFixedWidth(100);
	ui.label_time->setFixedWidth(125);
	ui.label_isright->setFixedWidth(55);

	char icon = 'A';
	QString temp = icon;
	for (int i = 0; i < mCount; i++) {
		AnswerStatistics* answer = new AnswerStatistics(ui.listWidget_answerStat);
		mAnswerIdMap[temp] = answer;
		QListWidgetItem *itemWidget = new QListWidgetItem(ui.listWidget_answerStat);
		answer->SetAnswerIndex(temp);
		//ui.listWidget_answerStat->setItemAlignment(Qt::AlignCenter);
		temp = ++icon;
		//answer->SetAnswerPerson(i, 8);
		//if (i == 4) {
		//	answer->SetAnswerCorrect(true);
		//}
		//else {
		//	answer->SetAnswerCorrect(false);
		//}
		answer->SetAnswerCorrect(false);
		itemWidget->setSizeHint(QSize(0, 60));
		ui.listWidget_answerStat->addItem(itemWidget);
		ui.listWidget_answerStat->setItemWidget(itemWidget, answer);
	}

	QJsonArray array_options;
	QJsonObject obj_options;
	
    QStringList selectIndex;
    std::map<QString, QPushButton*>::iterator iter = mSelectBtnMap.begin();
	 int count = 0;
    while (iter != mSelectBtnMap.end() && count++ < mCount) {
		  obj_options.insert("subject", iter->first);
		  obj_options.insert("key", iter->first);
        if (iter->second->styleSheet() == SELECT_STYLE && !iter->second->isHidden()) {
            selectIndex.push_back(iter->first);
				obj_options.insert("is_answer",1);
				std::map<QString, AnswerStatistics*>::iterator iterAnswer = mAnswerIdMap.find(iter->first);
				if (iterAnswer != mAnswerIdMap.end()) {
					iterAnswer->second->SetAnswerCorrect(true);
				}
        }
		  else {
			  obj_options.insert("is_answer", 0);
		  }
		  array_options.append(obj_options);
        iter++;
    }
    QString text;
    for (int i = 0; i < selectIndex.count(); i++) {
        if (i == 0) {
            text += selectIndex.at(i);
        }
        if (i > 0) {
            text += QString::fromWCharArray(L"、") + selectIndex.at(i);
        }
    }
    ui.label_answer_num->setText(text);
	  
	 //发起答题器
	 QJsonObject obj_sub;
	 QDateTime time = QDateTime::currentDateTime();
	 QString sub = QString::fromLocal8Bit("答题器") + QString::number(time.toTime_t());
	 obj_sub.insert("subject", sub);
	 obj_sub.insert("options", array_options);
	 obj_sub.insert("type", "scantron");

	 globalToolManager->GetClassSDK()->StartAnswer(QString(QJsonDocument(obj_sub).toJson()).toStdString());


    ui.stackedWidget->setCurrentIndex(1);
    ui.stackedWidget_stulist->setCurrentIndex(0);
    ui.widget_stuPerCount->hide();
}

void AnswerTool::slot_ClickedClose() {
    //CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_TIPS_WND);
	TipsWdg* tips = new TipsWdg(this);
    if (tips) {
        CSTDMapParam param;
        param[TipsMsg] = QStringLiteral("是否结束答题?");
        param[ShowCancelButton] = PARAM_TRUE;
        tips->Notice(param);
        int accept = tips->exec();
        if (accept == QDialog::Accepted) {
			//发起停止答题广播
			
			globalToolManager->GetClassSDK()->StopAnswer(mQuesId.toStdString());

         globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN,new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_ANSWER_TOOL));
			globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_ANSWER_TOOL));
			this->setParent(NULL);
			mbFirstNotice = false;
        }
    }
}

void AnswerTool::slot_ReAnswer() {
	ui.stackedWidget->setCurrentIndex(0);
	ui.pushButton_teacherCommitAllAnswer->setText(StrTeacherCommitAnswer);
	ui.listWidget_answerStat->clear();
	ui.listWidget_stulist->clear();
	ui.label_left->setFixedWidth(15);
	ui.label_right->setFixedWidth(15);
	ui.label_stu_commit_answer->setText("");
	//this->setFixedWidth(452);
	//this->setFixedHeight(392);

   setFixedSize(452, 392);
	ui.label_submit->setText(SUBMIT(0));
	ui.label_8->show();
	ui.label_startTimeCount->show();
	SelectBtnList.clear();
	ui.label_stu_commit_answer->setStyleSheet(LABEL_STARTANSWERRIGHT);
	ui.label_omit->setStyleSheet(LABEL_STARTANSWERRIGHT);
	mCount = 4;
	if (mpStartTimer && mpStartTimer->isActive()) {
		mpStartTimer->stop();
	}
	ui.pushButton_start->setStyleSheet(BTN_STSRT_START_ENADLE);
	ui.pushButton_start->setEnabled(false);
	ui.pushButton_ADD->setStyleSheet(BTN_ADD_STSRT_START);
	ui.pushButton_DEL->setStyleSheet(BTN_DEL_STSRT_START);
	ReLayout();
}

void AnswerTool::slot_ClickedStartTime() {

    if (mSecond == 60) {
        mSecond = 0;
        mMin++;
    }
    else {
        mSecond++;
    }
    if (mMin == 60) {
        mMin = 0;
        mHour++;
    }

    if (mHour == 60) {
        mHour = 0;
    }

    QString mSecondTimeText;
    if (mSecond >= 10) {
        mSecondTimeText = QString::number(mSecond);
    }
    else {
        mSecondTimeText = QString("0%1").arg(mSecond);
    }

    QString mMinTimeText;
    if (mMin >= 10) {
        mMinTimeText = QString::number(mMin);
    }
    else {
        mMinTimeText = QString("0%1").arg(mMin);
    }

    QString mmHourTimeText;
    if (mHour >= 10) {
        mmHourTimeText = QString::number(mHour);
    }
    else {
        mmHourTimeText = QString("0%1").arg(mHour);
    }


    if (mbIsTeacher) {
        if (mHour == 0) {
            QString timeText = QString::fromWCharArray(L"%1:%2").arg(mMinTimeText).arg(mSecondTimeText);
            ui.label_startTimeCount->setText(STARTEDTIME(timeText));
        }
        else {
            QString timeText = QString::fromWCharArray(L"%1:%2:%3").arg(mmHourTimeText).arg(mMinTimeText).arg(mSecondTimeText);
            ui.label_startTimeCount->setText(STARTEDTIME(timeText));
        }
    }
    else {
        QString timeText = QString::fromWCharArray(L"%1:%2:%3").arg(mmHourTimeText).arg(mMinTimeText).arg(mSecondTimeText);
        ui.label_studentAnswerTimer->setText(STARTTIME(timeText));
    }
}


void AnswerTool::slot_ClickedStudentCommit() {

	std::map<QString, QPushButton*>::iterator iter = mSelectBtnMap.begin();
	int count = 0;
	int textcount = 0;
	QString text;
	QString answer;
	char icon = 'A';
	QString temp = icon;


	while (iter != mSelectBtnMap.end() && count++ < mCount) {		
		if (iter->second->styleSheet() == SELECT_STYLE && !iter->second->isHidden()) {
			if (textcount == 0) {
				text += temp;
				answer = mBtnIdMap[temp];
			}
			if (textcount > 0) {
				text += QString::fromWCharArray(L"、") + temp;
				answer = answer + "," + mBtnIdMap[temp];
			}
			SelectBtnList << iter->second;
			textcount++;
		}
		else {
			
		}
		temp = ++icon;
		iter++;
	
	}
    ui.pushButton_studentCommit->hide();
	 ui.label_stuCommitNotice->show();
	 ui.listWidget_answerStat->setItemAlignment(Qt::AlignHCenter);


	 text = text.replace(QRegExp("[\r\n]"), "");
    int wndWidth = fontMetrics().width(text);
	 ui.label_stu_commit_answer->setText(text);
	 ui.label_stu_commit_answer->show();

	 //提交答案
	 globalToolManager->GetClassSDK()->AnswerIn(answer.toStdString(), mQuesId.toStdString());

	 mpStartTimer->stop();
	 ui.label_studentAnswerTimer->setText(StrStudentStayAnswer);
	 ui.label_studentAnswerTimer->setStyleSheet(LABEL_ANSWER_RIGHT);
	 ui.label_icon->show();
	 ui.label_icon->setStyleSheet(QLABEL_ICON_QNSWER_COMMIT);
	 ui.widget_stuTimerCount->setStyleSheet(WIDGET_START_RIGHT);
}

void AnswerTool::slot_ClickedTeacherCommitAnswer() {
    if (mpStartTimer) {
        mpStartTimer->stop();
    }

    ui.label_8->hide();
    ui.label_startTimeCount->hide();
    QString btnText = ui.pushButton_teacherCommitAllAnswer->text();
    if (btnText == StrTeacherCommitAnswer) {
        ui.pushButton_teacherCommitAllAnswer->setText(StrTeacherWatchDetail);
		  //公布答案
		  globalToolManager->GetClassSDK()->AnnounceAnswer(mQuesId.toStdString());
		  mbDetails = false;
    }
	 else if (btnText == StrTeacherCloseDetail) {
		 ui.stackedWidget_stulist->setCurrentIndex(0);
		 ui.pushButton_teacherCommitAllAnswer->setText(StrTeacherWatchDetail);
		 ui.widget_right->hide();
	 }
	 else{
		 //请求详情
		 if (!mbDetails) {
			 globalToolManager->GetClassSDK()->AnswerDetail(mQuesId.toStdString());
			 mbDetails = true;
			 ui.stackedWidget_stulist->setCurrentIndex(1);
			 ui.widget_stuPerCount->show();
			 ui.pushButton_restartanswer->show();
			 ui.widget_right->show();
			 //ui.pushButton_teacherCommitAllAnswer->hide();
			 //ui.pushButton_over->hide();
		 }
		 else {
			 ui.stackedWidget_stulist->setCurrentIndex(1);
			 ui.widget_right->hide();
			 ui.pushButton_teacherCommitAllAnswer->setText(StrTeacherCloseDetail);
		 }
    }
}


void AnswerTool::slot_ClickedTeacherOver() {

}

void AnswerTool::ReLayout() {
	mHour = 0;
	mMin = 0;
	mSecond = 0;
	ui.pushButton_studentCommit->show();
	ui.label_stuCommitNotice->hide();
	ui.label_stu_commit_answer->hide();
	ui.label_omit->hide();
	ui.label_stu_commit_answer->setText("");
	//this->setFixedHeight(350);
	ui.label_teacherCommitNotice->hide();
	ui.label_teacherCommitAnswer->hide();
	ui.pushButton_A->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_B->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_C->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_D->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_E->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_F->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_G->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_H->setStyleSheet(NO_SELECT_STYLE);
	ui.pushButton_A->show();
	ui.pushButton_B->show();
	ui.pushButton_C->show();
	ui.pushButton_D->show();
	ui.pushButton_E->hide();
	ui.pushButton_F->hide();
	ui.pushButton_G->hide();
	ui.pushButton_H->hide();
	//SelectBtn("B");
	ui.label_icon->hide();
	ui.widget_stuTimerCount->setStyleSheet("background-color: rgb(255, 247, 233);border:1px solid rgba(250, 206, 116, 1);");
	ui.label_studentAnswerTimer->setStyleSheet("color: rgb(68, 68, 68);border:none;");
	ui.label_stu_commit_answer->setStyleSheet(LABEL_STARTANSWERRIGHT);
	ui.label_omit->setStyleSheet(LABEL_STARTANSWERRIGHT);
	ui.pushButton_studentCommit->setStyleSheet(BTN_STSRT_START_ENADLE);
	ui.pushButton_studentCommit->setEnabled(false);
}

void AnswerTool::CloseExec() {

	if (!mbIsTeacher) {
		ReLayout();
	}
	setParent(NULL);
	mbFirstNotice = false;
	hide();
}