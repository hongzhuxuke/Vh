#ifndef CACTION_H
#define CACTION_H
 
#include <QAction>
 
class CAction: public QAction
{
    Q_OBJECT
public:
	CAction(QObject *parent = nullptr);
	CAction(const QString &text, QObject *parent = nullptr);
	CAction(const QIcon &icon, const QString &text, QObject *parent = nullptr);
	~CAction();

	void setstrIcon(QString strFile);
	void setstrHoverIcon(QString strFile);

	void showIcon();
signals:
	void sig_Hover(CAction*);
public slots:
	void hover();
protected:
	bool eventFilter(QObject *watched, QEvent *event);

private:
	QString mstrIcon;
	QString mstrHoverIcon;
};
 
#endif // CACTION_H
