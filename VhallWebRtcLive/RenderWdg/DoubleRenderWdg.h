#pragma once

#include <QWidget>
#include "ui_DoubleRenderWdg.h"
#include "../Unility/Unility.h"

#include "VhallRenderMaskWdg.h"
#include "MemberCtrlMaskWdg.h"

class DoubleRenderWdg : public QWidget
{
    Q_OBJECT

public:
    DoubleRenderWdg(QWidget *parent = Q_NULLPTR);
    ~DoubleRenderWdg();

    void StartRenderTeacherView(QString uid);
    void StartRenderStudentView(QString uid);

    void SetTeacherJoinId(QString join_id);
    void SetStudendJoinId(QString join_id);

    void SetTeacherCameraStreamId(QString stream_id);
    void SetStudentCameraStreamId(QString stream_id);

    void SetTeacherNickName(QString name);
    void SetStudentNickName(QString name);

private:
    Ui::DoubleRenderWdg ui;  

    //VhallRenderMaskWdg *mVhallRenderMaskWdg;
    //MemberCtrlMaskWdg *mMemberCtrlMaskWdg;

    QString mTeacherJoinId;
    QString mStudendJoinId;
    QString mTeacherCameraStreamId;
    QString mStudendCameraStreamId;
    QString mTeacherNickName;
    QString mStudendNickName;
};
