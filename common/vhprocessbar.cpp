#include "vhprocessbar.h"
#include "ui_vhprocessbar.h"
#include <QTimer>
#include <QPainter>
#include "pathmanager.h"
#include "AlertDlg.h"

VHProcessBar::VHProcessBar(QWidget *parent) :
m_pTimer(NULL),
QWidget(parent),
ui(new Ui::VHProcessBar) {
   ui->setupUi(this);
   this->installEventFilter(this);
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Dialog);
   setAttribute(Qt::WA_TranslucentBackground);
   setAutoFillBackground(false);

   QString vhprocessbarback = CPathManager::GetVhprocessbarbackImagePath();
   this->pixmap = QPixmap(vhprocessbarback);
   this->setFixedSize(pixmap.size());
}

VHProcessBar::~VHProcessBar() {
   if (NULL != m_pTimer) {
      if (m_pTimer->isActive()) {
         m_pTimer->stop();
      }
      delete m_pTimer;
      m_pTimer = NULL;
   }
   delete ui;
}

void VHProcessBar::SetValue(int v) {
   ui->progressBar->setValue(v);
   ui->labPerc->setText(QString("%1%").arg(v));
}

bool VHProcessBar::GetSuspension()
{
	return bSuspension;
}

void VHProcessBar::setForceUpdate(const bool& bForce) {
	bForce ? ui->btnClose->setEnabled(false): ui->btnClose->setEnabled(true);
}

bool VHProcessBar::eventFilter(QObject *o, QEvent *e) {
   if (o == this) {
      if (e->type() == QEvent::MouseButtonPress) {
         this->pressPoint = this->cursor().pos();
         this->startPoint = this->pos();
      } else if (e->type() == QEvent::MouseMove) {
         int dx = this->cursor().pos().x() - this->pressPoint.x();
         int dy = this->cursor().pos().y() - this->pressPoint.y();
         this->move(this->startPoint.x() + dx, this->startPoint.y() + dy);
      }
   }
   return QWidget::eventFilter(o, e);
}
void VHProcessBar::hideEvent(QHideEvent *e) {
   QWidget::hideEvent(e);
   emit this->SigHide();
}

void VHProcessBar::show() {
   if (NULL == m_pTimer) {
      m_pTimer = new QTimer;
      connect(m_pTimer, &QTimer::timeout, this, &VHProcessBar::slotTimeOut);
   }
   m_pTimer->start(800);
   m_iIndex = 0;
   bSuspension = false;
   QWidget::show();
}

void VHProcessBar::hide() {
   if (NULL != m_pTimer && m_pTimer->isActive()) {
      m_pTimer->stop();
   }
   QWidget::hide();
}

//QLabel{\n  font: 13px "宋体";\n  text-align: left;\n  color: #bbbbbb;\n}\n
void VHProcessBar::slotTimeOut() {
   switch (m_iIndex) {
   case 0:
      m_iIndex++;
      ui->labAnimal->setText(QStringLiteral("   正在更新   "));
      break;
   case 1:
      m_iIndex++;
	  ui->labAnimal->setText(QStringLiteral("   正在更新.  "));
      break;
   case 2:
      m_iIndex++;
	  ui->labAnimal->setText(QStringLiteral("   正在更新.. "));
      break;
   default:
      m_iIndex = 0;
	  ui->labAnimal->setText(QStringLiteral("   正在更新..."));
      break;
   }
}

void VHProcessBar::on_btnClose_clicked(bool checked/* = false*/)
{
	AlertDlg alertDlg(QString::fromWCharArray(L"正在下载新版助手，确定要退出吗？"), true, NULL);
	alertDlg.SetYesBtnText(QString::fromWCharArray(L"确定"));
	alertDlg.SetNoBtnText(QString::fromWCharArray(L"取消"));
	alertDlg.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	if (QDialog::Accepted == alertDlg.exec())
	{
		bSuspension = true;
		//中间关闭
		if (NULL != m_pTimer && m_pTimer->isActive()) {
			m_pTimer->stop();
		}

		emit SigSuspensionLoad();
	}
}

void VHProcessBar::paintEvent(QPaintEvent *) {
   QPainter painter(this);
   painter.drawPixmap(rect(), this->pixmap);
}

