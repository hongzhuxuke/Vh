#ifndef _VHDIALOG_H_
#define _VHDIALOG_H_
#include <QDialog>
#include <QKeyEvent>

class CBaseDlg : public QDialog
{
    Q_OBJECT

public:
    CBaseDlg(QWidget *parent = NULL) {};
    ~CBaseDlg() {};

protected:
    virtual void keyPressEvent(QKeyEvent *event) {
        if (event) {
            switch (event->key()) {
            case Qt::Key_Escape:
                break;
            default:
                QDialog::keyPressEvent(event);
            }
        }
    }

private:

};


class VHDialog : public CBaseDlg {
   Q_OBJECT

public:
   VHDialog(QWidget *parent = 0);
   ~VHDialog();
   static void WillClose();
   static void WillShow(void *);
   static int VHDialogShowCount(); 
   static void Raise();
   
protected:
   virtual void showEvent(QShowEvent *);
   virtual void hideEvent(QHideEvent *);
   virtual void keyPressEvent(QKeyEvent *);
   virtual void OnSave();
private:
   static int mShowCount;
   static VHDialog *mCurrent;
};
#endif