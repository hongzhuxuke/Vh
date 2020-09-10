#include "NoticeShowWdg.h"

NoticeShowWdg::NoticeShowWdg(bool bShowTool, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	if (bShowTool){
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
	}
    
 //   m_curIndex = 0;//��ǰ����
 //   m_cutStart = 0;
 //   mbIsPaintStartPos = false;
 //   mPaintStartPos = 0;
 //   m_charWidth = fontMetrics().width("a");//ÿ���ַ��Ŀ��
	//connect(&mShowMsgTimer, &QTimer::timeout, this, &NoticeShowWdg::updateIndex);

    setAttribute(Qt::WA_TranslucentBackground, true);
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_close()));
}

NoticeShowWdg::~NoticeShowWdg()
{

}

void NoticeShowWdg::ShowNoticeMsg(QString msg) {
	ui.widget_show->ShowNoticeMsg(msg);
	//msg = msg.replace(QRegExp("[\r\n]"), "");
	//if (mShowMsgTimer.isActive())
	//	mShowMsgTimer.stop();

	////if (msg.isEmpty())
	////{
	////	hide();
	////}
	////else
	////{
	//	//show();
	//	m_showText = msg;
	//	m_curIndex = 0;
	//	int msgLen = m_showText.length();
	//	int wndWidth = fontMetrics().width(m_showText);

	//	//mShowMsgTimer.start(200);
	//	if (wndWidth >= width()) {
	//		mShowMsgTimer.start(200);
	//	}
	//	else {
	//		update();
	//	}
	//}
}

void NoticeShowWdg::paintEvent(QPaintEvent *event)
{
    //__super::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(this->rect(), QColor(248, 234, 228));
    painter.setPen(QColor(236, 132, 85));

	QWidget::paintEvent(event);
    //int showMsgWidth = fontMetrics().width(m_showText);
    //if (showMsgWidth < this->width()) {
    //    painter.drawText((this->width()-showMsgWidth)/2, 20, m_showText);
    //}
    //else {
    //    painter.drawText(mPaintStartPos, 20, m_showText.mid(m_cutStart, m_curIndex));
    //}
}

//void NoticeShowWdg::updateIndex()
//{
//    update();
//    //���һ����ȡ�ַ�����λ�á�
//    m_curIndex++;
//    QString showMsg = m_showText.mid(m_cutStart, m_curIndex);
//    QString totalShowMsg = m_showText.mid(0, m_curIndex);
//    int totalShowMsgLen = fontMetrics().width(totalShowMsg);
//    int showMsgWidth = fontMetrics().width(showMsg);
//    //һ���ܵĽ�ȡ�����ַ����ȣ��ȴ��ڴ�С������ô��ʼλ�ô�0��ʼ������֮����ַ�������һ���ַ���ʼ��ȡ
//    if (totalShowMsgLen >= this->width()) {
//        mPaintStartPos = 0;
//        m_cutStart++;
//    }
//    else {
//        mPaintStartPos = this->width()-showMsgWidth;
//    }
//
//    if (m_curIndex == m_showText.length() * 2) {
//        m_cutStart = 0;
//        m_curIndex = 0;
//    }  
//}

void NoticeShowWdg::slot_close() {
    //mShowMsgTimer.stop();
	ui.widget_show->StopTimer();
    hide();
}

