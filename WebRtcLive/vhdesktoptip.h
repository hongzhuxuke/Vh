#ifndef VHDESKTOPTIP_H
#define VHDESKTOPTIP_H

#include <QWidget>
#include <QTimer>

class QGraphicsDropShadowEffect;

namespace Ui {
class VHDesktopTip;
}

class VHDesktopTip : public QWidget
{
    Q_OBJECT

public:
    explicit VHDesktopTip(QWidget *parent = 0);
    ~VHDesktopTip();
    void Tip(QString);
    void Stop();
    void SetPixmap(QString);
    void CenterWindow(QWidget * parent);
signals:
    void SigStop();
public slots:
    void timeout();
    void SlotStop();
protected:
    void paintEvent(QPaintEvent *);
private:
    Ui::VHDesktopTip *ui;
    QPixmap pixmap;
    QTimer timer;
    int timerCount=0;

	QGraphicsDropShadowEffect* mpEffect = nullptr;
	QGraphicsDropShadowEffect* mpEffectLeft = nullptr;
};

#endif // VHDESKTOPTIP_H
