#include "NoticeItem.h"
#include <QDate>

NoticeItem::NoticeItem(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

NoticeItem::~NoticeItem()
{
}

int NoticeItem::InsertNoticeMsg(QString msg) {
    QString text = QStringLiteral("[课堂公告] %1").arg(msg);
    ui.textEdit->setText(text);

    QFont font;
    font.setFamily(QStringLiteral("微软雅黑"));
    font.setPointSize(9);
    QFontMetrics fm(font);
    QRect rec = fm.boundingRect(text);
    //字符串所占的像素宽度,高度
    int textWidth = rec.width();
    int textHeight = rec.height();

    int count = textWidth / 278;
    if (count == 0) {
        count = 1;
    }
    else if (textWidth > count * 278) {
        count += 1;
    }

    int newHeight = 48 + count * 22;
    this->setFixedHeight(newHeight);
    ui.textEdit->setFixedHeight(count * 22);
	ui.label_flag->setFixedHeight(25+count * 22);
	if (count > 1)
	{
		ui.label_Flag->setFixedHeight(8);
	}
    QDateTime local(QDateTime::currentDateTime());
    QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");
    ui.label_data->setText(localTime);
    return newHeight;
}
