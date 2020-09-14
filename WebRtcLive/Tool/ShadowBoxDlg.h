#ifndef SHADOWBOXDLG_H
#define SHADOWBOXDLG_H

#include "ui_ShadowBoxDlg.h"
#include <QDialog>

class ShadowBoxDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ShadowBoxDlg(QWidget *parent = 0);
    ~ShadowBoxDlg();
    void SetTitleChanged(const QString& strTitle);
    void setLefRightBoerderWidth(int iW);
    int GetChildGlobalX();
    int GetChildGlobalY();

    int GetChildX();
    int GetChildY();
signals:

public slots:

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
private slots:

private:
   
	void moveXY();


	Ui::ShadowBoxDlg ui;
	//bool mbIsMaxSize = false;
};

#endif // ShadowBoxDlg_H