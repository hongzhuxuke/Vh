#ifndef H_CPAINTSTATEBTN_H
#define H_CPAINTSTATEBTN_H

#include <QPushButton>
#include "priConst.h"

class QPainter;


class CPaintStateBtn : public QPushButton
{
	QOBJECT_H
public:
	CPaintStateBtn(QWidget * parent);
	CPaintStateBtn(const QString &, QWidget *);
	CPaintStateBtn(const QIcon &, const QString &, QWidget *);
	~CPaintStateBtn();
	void SetHandsUpState(eHandsUpState  eState);


protected:
	void paintEvent(QPaintEvent *);
private:
	eHandsUpState mState;
	
};

#endif//H_CPAINTSTATEBTN_H
