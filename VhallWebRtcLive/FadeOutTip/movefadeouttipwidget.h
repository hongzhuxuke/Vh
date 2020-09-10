#ifndef MOVEFADEOUTTIPWIDGET_H
#define MOVEFADEOUTTIPWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <QLabel>
#include <QLabel>


class MoveFadeoutTipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MoveFadeoutTipWidget(QWidget *parent = 0);
    ~MoveFadeoutTipWidget();
 //   void Tip(QWidget* mainWdg,QPoint, QString);
	//void rePos(QWidget* mainWdg);
	void StartRoll();
	void SetTilte(const QString& str);
	void SetTilteSheetStyle(const QString& str);
public slots:

protected:
   void paintEvent(QPaintEvent *);
   void mousePressEvent(QMouseEvent *event);
private slots:
	void slot_moveText();
private:

	QLabel* mpTipText = nullptr;
	QPoint mPoint;
	QSize m_globalSize;
	QTimer *mPMoveTimer = nullptr;
	QWidget *m_pParentWdg = NULL;
};

#endif // MOVEFADEOUTTIPWIDGET_H
