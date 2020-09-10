#pragma once

#include <QObject>
#include "vhmainthread_global.h"

class VHMAINTHREAD_EXPORT MainThreadEventProcessor : public QObject
{

public:
   MainThreadEventProcessor(QObject *parent = nullptr);
   ~MainThreadEventProcessor();

protected:
	virtual void customEvent(QEvent *);
private:
	void HandleDestoryWnd(QEvent* event);
};
