#pragma once

#include <QDialog>
#include "ui_blackMenuWdg.h"
#include <QMap>
#include <QList>
#include <QPushButton>

class CMapButton;

class CPushButton : public  QPushButton
{
	Q_OBJECT
public: 

	CPushButton(QWidget *parent = Q_NULLPTR);
	CPushButton(const QString &text, QWidget *parent = Q_NULLPTR);
	CPushButton(const QIcon &icon, const QString &text, QWidget *parent = Q_NULLPTR);
	~CPushButton();
	void SetOpType(const int& iOp);
signals:
	void sigClicked(const int& iOptype);
private slots:
	void slot_Clicked();
private:
	int miOpType = -1;
};

class BlackMenuWdg : public QDialog
{
	Q_OBJECT

public:
	BlackMenuWdg(QWidget *parent = Q_NULLPTR);
	~BlackMenuWdg();
	void show(int x, int y);
	//void SetShowItemCount(const int& iCount);

	void addItem(const int& iOpType,
		const QString& strStyleSheet, const bool& bCheckedable = true);
	void setItemEnabled(const int& iOpType, const bool& bEnabled = true);
	void SetItemShow(const int& iOpType, const bool& bShow = true);
	void SetItemChecked(const int& iOpType, const bool& bChecked = false);
   void addTrigger(CMapButton* btnMap);
signals:
	void sigClicked(const int& iOpType);
	void sigClose();
protected:
	void showEvent(QShowEvent *);
	virtual void focusOutEvent(QFocusEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
private slots:
   void slotFoucusOutEvent();
private:
	Ui::blackMenuWdg ui;
	bool m_bEnter = false;
	QMap<int, CPushButton*> mlistMenuItem;
	int miFixHeight = 18;
   QList<CMapButton*> mlistMapBtn;
};
