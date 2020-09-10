#pragma once

#include <QWidget>
#include "ui_StudentStat.h"

class StudentStat : public QWidget
{
    Q_OBJECT

public:
    StudentStat(QWidget *parent = Q_NULLPTR);
    ~StudentStat();

    void SetStudentAnswerInfo(QString nickName, QString answer, QString answerTime, bool right);

private:
    Ui::StudentStat ui;
};
