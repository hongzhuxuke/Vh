#include "MoveNoticeWdg.h"

MoveNoticeWdg::MoveNoticeWdg(QWidget *parent)
    : QWidget(parent)
{
    m_curIndex = 0;//��ǰ����
    m_cutStart = 0;
    mbIsPaintStartPos = false;
    mPaintStartPos = 0;
    m_charWidth = fontMetrics().width("a");//ÿ���ַ��Ŀ��
    setAttribute(Qt::WA_TranslucentBackground, true);
    connect(&mShowMsgTimer, &QTimer::timeout, this, &MoveNoticeWdg::updateIndex);
}

MoveNoticeWdg::~MoveNoticeWdg()
{

}

void MoveNoticeWdg::ShowNoticeMsg(QString msg) {
	msg = msg.replace(QRegExp("[\r\n]"), "");
	if (mShowMsgTimer.isActive())
		mShowMsgTimer.stop();

	m_showText = msg;
	m_curIndex = 0;
	int msgLen = m_showText.length();
	int wndWidth = fontMetrics().width(m_showText);


	if (wndWidth >= width()) {
		mShowMsgTimer.start(200);
	}
	else {
		update();
	}

}

void MoveNoticeWdg::StopTimer()
{
	if (mShowMsgTimer.isActive())
		mShowMsgTimer.stop();
}

void MoveNoticeWdg::paintEvent(QPaintEvent *event)
{
    //__super::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(this->rect(), QColor(248, 234, 228));
    painter.setPen(QColor(236, 132, 85));
    int showMsgWidth = fontMetrics().width(m_showText);
    if (showMsgWidth < this->width()) {
        painter.drawText((this->width()-showMsgWidth)/2, 20, m_showText);
    }
    else {
        painter.drawText(mPaintStartPos, 20, m_showText.mid(m_cutStart, m_curIndex));
    }
	QWidget::paintEvent(event);
}

void MoveNoticeWdg::updateIndex()
{
    update();
    //���һ����ȡ�ַ�����λ�á�
    m_curIndex++;
    QString showMsg = m_showText.mid(m_cutStart, m_curIndex);
    QString totalShowMsg = m_showText.mid(0, m_curIndex);
    int totalShowMsgLen = fontMetrics().width(totalShowMsg);
    int showMsgWidth = fontMetrics().width(showMsg);
    //һ���ܵĽ�ȡ�����ַ����ȣ��ȴ��ڴ�С������ô��ʼλ�ô�0��ʼ������֮����ַ�������һ���ַ���ʼ��ȡ
    if (totalShowMsgLen >= this->width()) {
        mPaintStartPos = 0;
        m_cutStart++;
    }
    else {
        mPaintStartPos = this->width()-showMsgWidth;
    }

    if (m_curIndex == m_showText.length() * 2) {
        m_cutStart = 0;
        m_curIndex = 0;
    }  
}