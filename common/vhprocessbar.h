#ifndef VHPROCESSBAR_H
#define VHPROCESSBAR_H

#include <QWidget>

namespace Ui {
class VHProcessBar;
}

class VHProcessBar : public QWidget
{
    Q_OBJECT

public:
    explicit VHProcessBar(QWidget *parent = 0);
    ~VHProcessBar();
    void SetValue(int);
	bool GetSuspension();
	void setForceUpdate(const bool& bForce);
signals:
    void SigHide();  
	void SigSuspensionLoad();
public slots:
	void show();
	void hide();
protected:
    bool eventFilter(QObject *, QEvent *);
    void hideEvent(QHideEvent *);
	 void paintEvent(QPaintEvent *);
private slots:
	 void slotTimeOut();
	 void on_btnClose_clicked(bool checked = false);
private:
    Ui::VHProcessBar *ui;
    QPoint pressPoint;
    QPoint startPoint;
	 QTimer* m_pTimer = NULL;
	 int m_iIndex;
	 QPixmap pixmap;
	 bool bSuspension = false;
};

#endif // VHPROCESSBAR_H
