#pragma once

#include <QDialog>
#include "ui_ShortcutWdg.h"

class ShortcutWdg : public QDialog
{
	Q_OBJECT

public:
	ShortcutWdg(QWidget *parent = Q_NULLPTR);
	~ShortcutWdg();

   enum OpMediaType
   {
      OpMediaType_ChatShare,
      OpMediaType_Member,

      OpMediaType_Testpaper,
      OpMediaType_Signin,
      OpMediaType_answer,
      OpMediaType_time,

      OpMediaType_NULL
   };

private slots:
	void on_But_chat_clicked();
	void on_But_member_clicked();
   void on_But_Testpaper_clicked();
   void on_But_Signin_clicked();
   void on_But_answer_clicked();
   void on_But_time_clicked();

protected:
	void showEvent(QShowEvent *);
	virtual void focusOutEvent(QFocusEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);

signals:
	void sig_click(const int&);

private:
	Ui::ShortcutWdg ui;

	bool m_bEnter = false;
};
