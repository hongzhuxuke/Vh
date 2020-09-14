#include "StudentStat.h"


#define StrRight    QString::fromWCharArray(L"ÕýÈ·")
#define StrErr      QString::fromWCharArray(L"´íÎó")
#define StrErrSheet QString::fromWCharArray(L"font:10pt \"Î¢ÈíÑÅºÚ\";color:rgb(254, 99, 106);")

StudentStat::StudentStat(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

StudentStat::~StudentStat()
{
}


void StudentStat::SetStudentAnswerInfo(QString nickName, QString answer, QString answerTime, bool right) {
    ui.label_name->setText(nickName);
    ui.label_answer->setText(answer);
    ui.label_time->setText(answerTime);
    if (right) {
        ui.label_right->setText(StrRight);
    }
    else {
        ui.label_right->setText(StrErr);
        ui.label_right->setStyleSheet(StrErrSheet);
    }
}