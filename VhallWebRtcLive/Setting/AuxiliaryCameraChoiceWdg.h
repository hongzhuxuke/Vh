#ifndef  H_AUXILIARYCAMERACHOICEWDG_H
#define H_AUXILIARYCAMERACHOICEWDG_H
#include <QWidget>
#include "ui_AuxiliaryCameraChoiceWdg.h"
#include "../Unility/Unility.h"
#include "VhallUI_define.h"
#include "LiveEventInterface.h"
#include "VhallIALiveSettingDlg.h"

class AuxiliaryCameraChoiceWdg : public CBaseWnd
{
    Q_OBJECT

public:
    AuxiliaryCameraChoiceWdg(QWidget *parent = Q_NULLPTR);
    ~AuxiliaryCameraChoiceWdg();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);
    virtual void Notice(const CSTDMapParamPtr &);
    void PreviewAuxiliaryCamera(QEvent*event);
    void EnableUI();
signals:
   void sigRealse();
protected:
    //virtual void customEvent(QEvent*);

private slots:
    void slot_OnClickedApply();
    void slot_OnClickedClose();
    void on_cmbcamera_currentIndexChanged(int index);
private:
   void slotOvertime();
   void StartLocalAuxiliaryCapture(std::string cameraId);
   void stopTimer();

   Ui::CameraCaptureAuxiliary ui;
   bool mbCaptureSuccess = false;
   QTimer* mpOvertimeTimer = nullptr;
};

#endif //H_AUXILIARYCAMERACHOICEWDG_H