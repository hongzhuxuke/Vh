#include "WndManager.h"
#include "Unility.h"
#include <QCoreApplication>
#include <QMouseEvent>
#include <QDesktopWidget> 
#include <QApplication>

CBaseWnd::CBaseWnd(QWidget *parent) :
    QDialog(parent) {
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
}

CBaseWnd::~CBaseWnd() {

}

void CBaseWnd::showEvent(QShowEvent *event) {
    this->setAttribute(Qt::WA_Mapped);
    QWidget::showEvent(event);
}

void CBaseWnd::mousePressEvent(QMouseEvent *event) {
    if (event) {
        mPressPoint = this->pos()-event->globalPos();
        mIsMoved = true;
    }
}

void CBaseWnd::mouseMoveEvent(QMouseEvent *event) {
    if (event && (event->buttons() == Qt::LeftButton) && mIsMoved  && m_bIsEnableMove) {
        this->move(event->globalPos() + mPressPoint);
		this->raise();
    }
}

void CBaseWnd::mouseReleaseEvent(QMouseEvent *) {
    if (mIsMoved) {
        mIsMoved = false;

        QPoint pos = this->pos();
        QRect rect = QApplication::desktop()->availableGeometry(pos);
        QRect wnd_rect = this->frameGeometry();

        if (pos.y() > rect.height() - 150) {
           pos.setY(rect.height() - 150);
           this->move(pos);
        }
        else if (pos.y() < rect.y()) {
           this->move(pos + QPoint(0, +100));
        }
        else if (wnd_rect.x() >= rect.x() + rect.width() - 100) {
           this->move(QPoint(wnd_rect.x(), wnd_rect.y()) + QPoint(-100, 0));
        }
        else if (wnd_rect.x() + this->width() < rect.x() + 100) {
           this->move(QPoint(wnd_rect.x(), wnd_rect.y()) + QPoint(100, 0));
        }

    }
}

void CBaseWnd::keyPressEvent(QKeyEvent *event) {
    switch (event->key())
    {
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}


WndManager::WndManager(QObject *parent)
    : QObject(parent)
{
}

WndManager::~WndManager()
{

}

void WndManager::RegisterWndFactory(QString wndId, CreateFunction fun) {
   std::unique_lock<std::mutex> lock(mWndMutex);
   std::unordered_map<QString, CreateFunction>::iterator iter = mWndFactory.find(wndId);
   if (iter == mWndFactory.end()) {
      mWndFactory.insert(std::map<QString, CreateFunction>::value_type(wndId, fun));
	  ToolManager::GetInstance()->GetDataManager()->WriteLog("%s  %s", __FUNCTION__, wndId.toStdString().c_str());
   }
   else {
	   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s  Wnd find error%s", __FUNCTION__, wndId.toStdString().c_str());
   }
}

CBaseWnd* WndManager::FindWnd(QString wndId ,QString strWndIndex) {
   CBaseWnd* wnd = nullptr;
   std::unique_lock<std::mutex> lock(mWndMutex);
   QString createId = wndId + strWndIndex;
   std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(createId);
   if (iterWndMap != mWndMap.end()) {
      return iterWndMap->second;
   }

    std::unordered_map<QString, CreateFunction>::iterator iter = mWndFactory.find(wndId);
    if (iter != mWndFactory.end()) {
        wnd = (CBaseWnd*)(iter->second)();
    }
    else {
		ToolManager::GetInstance()->GetDataManager()->WriteLog("%s  Wnd find error%s", __FUNCTION__, wndId.toStdString().c_str());
        return nullptr;
    }
    std::unordered_map<QString, CBaseWnd*>::iterator iterWndMapInsert = mWndMap.find(createId);
    if (iterWndMapInsert == mWndMap.end()) {
        mWndMap.insert(std::map<QString, CBaseWnd*>::value_type(createId, wnd));
    }

   return wnd;
}

bool WndManager::IsWndExist(QString wndId) {
    CBaseWnd* wnd = nullptr;
    std::unique_lock<std::mutex> lock(mWndMutex);
    QString createId = wndId;
    std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(createId);
    while (iterWndMap != mWndMap.end()) {
        if (iterWndMap->first.contains(createId)) {
			CBaseWnd* pWnd = iterWndMap->second;
			if (NULL != pWnd)
			{
				return pWnd->isActiveWindow() || pWnd->isVisible();
			}
        }
    }
    return false;
}

bool WndManager::HideWnd(QString wndId) {
    std::unique_lock<std::mutex> lock(mWndMutex);
    std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(wndId);
    if (iterWndMap != mWndMap.end()) {
        iterWndMap->second->hide();
        return true;
    }
    return false;
}

bool WndManager::ShowWnd(QString wndId) {
    std::unique_lock<std::mutex> lock(mWndMutex);
    std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(wndId);
    if (iterWndMap != mWndMap.end()) {
        iterWndMap->second->show();
        return true;
    }
    return false;
}

bool WndManager::DestoryWnd(QString wndId) {
   std::unique_lock<std::mutex> lock(mWndMutex);
   std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(wndId);
   if (iterWndMap != mWndMap.end()) {
      delete iterWndMap->second;
      mWndMap.erase(iterWndMap);
   }
   return false;
}


bool WndManager::DestoryExistWnd(QString wndId) {
    std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.begin();
    while (iterWndMap != mWndMap.end()) {
        if (wndId != iterWndMap->first) {
            delete iterWndMap->second;
            iterWndMap = mWndMap.erase(iterWndMap);
        }
        else {
            iterWndMap++;
        }
    }
    return true;
}

bool WndManager::PostEventToWnd(QString wndId, QEvent* event) {
    std::unique_lock<std::mutex> lock(mWndMutex);
    std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(wndId);
    if (iterWndMap != mWndMap.end()) {
        QCoreApplication::postEvent(iterWndMap->second, event);
        return true;
    }
    return false;
}

void WndManager::DestoryAllWnd() {
   std::unordered_map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.begin();
   while (iterWndMap != mWndMap.end()) {
       delete iterWndMap->second;
      iterWndMap = mWndMap.erase(iterWndMap);
   }
}


