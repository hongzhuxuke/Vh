#ifndef CORSERSETBUTTON_H
#define CORSERSETBUTTON_H
#include <QPushButton>


class CorserSetButton : public QPushButton
{

    Q_OBJECT

public:
    CorserSetButton(QWidget *parent = 0);
    ~CorserSetButton();


protected:
	void enterEvent(QEvent *event);
private:
 
};

#endif // CORSERSETBUTTON_H
