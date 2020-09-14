#pragma once

#include <QWidget>
#include "ui_BigRenderCtrl.h"
#include "../Unility/Unility.h"
#include "CToolTips.h"
class BigRenderCtrl : public QWidget
{
    Q_OBJECT

public:
    BigRenderCtrl(QWidget *parent = Q_NULLPTR);
    ~BigRenderCtrl();
    void InitBtn(QString userId);

    void SetCameraState(bool open);
    void SetMicState(bool open);
    void SetAllMuteVideo(bool mute);
    void SetAllMuteAudio(bool mute); 

    void SetStudentCamera(bool open);
    void SetStudentMic(bool open);

    void OpenClassStudentPublish(bool start);

private slots:
    void slot_OnClickedCamera();
    void slot_OnClickedMic();
    void slot_OnClickedfullScreen();

    void slot_OnAllMuteVideo();
    void slot_OnAllMuteMic();
    void slot_OnAllUnPublish();
    void slot_OnChangeCamera();
    void slot_OnBigScreenChanged();

    void slot_OncloseStuCamera();
    void slot_OncloseStuMic();
    void slot_OnUnpublishStu();

protected:
    virtual bool eventFilter(QObject* receiver, QEvent * event);

signals:
    void sig_OnClickedCamera();
    void sig_OnClickedMic();
    void sig_OnClickedfullScreen();

    void sig_OnAllMuteVideo();
    void sig_OnAllMuteMic();
    void sig_OnAllUnPublish();  

    void sig_OnChangeCamera();
    void sig_OnBigScreenChanged();
    void sig_OncloseStuCamera();
    void sig_OncloseStuMic();
    void sig_OnUnpublishStu();

public:
    bool mbStudentCameraOpen = true;
    bool mbStudentMicOpen = true;
private:
    Ui::BigRenderCtrl ui;
    CToolTips* mToolTips;

};
