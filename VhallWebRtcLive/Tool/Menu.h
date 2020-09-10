#ifndef MENU_H
#define MENU_H
 
#include <QMenu>
#include <QCommonStyle>
 
class Menu : public QMenu
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = 0);
    explicit Menu(const QString & title);
    void init();
 
protected:
    virtual bool event(QEvent *event);
 
signals:
 
public slots:
 
};


	class CustomIconStyle : public QCommonStyle
	{
		Q_OBJECT

	public:
		CustomIconStyle();
		~CustomIconStyle();

		void SetCustomSize(int);

		//protected:
		virtual int pixelMetric(PixelMetric metric, const QStyleOption * option, const QWidget * widget) const;

	private:
		int mSize;
	};



 
#endif // MENU_H
