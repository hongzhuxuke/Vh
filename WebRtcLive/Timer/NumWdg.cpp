#include "NumWdg.h"

NumWdg::NumWdg(QWidget *parent)
    : QWidget(parent)
    , mNum(0)
{
    ui.setupUi(this);
    connect(ui.pushButton_up, SIGNAL(clicked()), this, SLOT(slot_ClickedUp()));
    connect(ui.pushButton_down, SIGNAL(clicked()), this, SLOT(slot_ClickedDown()));    
}

NumWdg::~NumWdg()
{
}

void NumWdg::SetMaxSize(int maxNum) {
    mMaxNum = maxNum;
}

void NumWdg::slot_ClickedUp() {
    if (mNum >= mMaxNum) {
        mNum = mMaxNum;
    }
    else {
        mNum++;
    }
    ui.label->setText(QString::number(mNum));
}

void NumWdg::slot_ClickedDown() {
    if (mNum <= 0) {
        mNum = 0;
    }
    else {
        mNum--;
    }
    ui.label->setText(QString::number(mNum));
}

void NumWdg::SetNum(int num) {
	mNum = num;
    ui.label->setText(QString::number(mNum));
}

int NumWdg::GetNum() {
    return mNum;
}

void NumWdg::enableEdit(bool recording) {
	ui.pushButton_up->setEnabled(recording);
	ui.pushButton_down->setEnabled(recording);
}

void NumWdg::showEdit(const bool& bEnable/* = true*/)
{
	if (bEnable) {
		ui.pushButton_up->show();
		ui.pushButton_down->show();
	}
	else {
		ui.pushButton_up->hide();
		ui.pushButton_down->hide();
	}
}