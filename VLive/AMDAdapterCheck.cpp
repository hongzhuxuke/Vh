#include "stdafx.h"
#include "AMDAdapterCheck.h"

AMDAdapterCheck::AMDAdapterCheck(QString name,QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setAutoFillBackground(true);
    connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(accept()));
    QString notice = QStringLiteral("�Կ�����:%1\n\n��⵽��ǰ�Կ������汾���ͣ�����Ӱ��\n��������ʹ�ã�������������µ��Կ���\n���汾֮�����ԡ�").arg(name);
    ui.label->setText(notice);
}

AMDAdapterCheck::~AMDAdapterCheck()
{
}


void AMDAdapterCheck::keyPressEvent(QKeyEvent *event) {
    if (event) {
        switch (event->key()) {
        case Qt::Key_Escape:
            break;
        default:
            QDialog::keyPressEvent(event);
        }
    }
}