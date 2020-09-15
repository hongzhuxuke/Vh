#include "LibManager.h"
#include <windows.h>
#include <QCoreApplication>

LibManager::LibManager(ToolManager* toolManager, QObject *parent)
    : QObject(parent),
   mToolManager(toolManager)
{
}

LibManager::~LibManager()
{
}
static wchar_t * char2wchar(const char* cchar)
{
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}



bool LibManager::InitDll() {
	QString dir = QCoreApplication::applicationDirPath() + "/WebRtcLive.dll";
	if (!RegisterDll(char2wchar(dir.toStdString().c_str()))) {
		return false;
	}

   //dir = QCoreApplication::applicationDirPath() + "/Examination.dll";
   //if (!RegisterDll(char2wchar(dir.toStdString().c_str()))) {
   //   return false;
   //}
	//dir = QCoreApplication::applicationDirPath();
	//QString main_thread_dll = dir + "/VHMainThread.dll";
	//if (!RegisterDll(char2wchar(main_thread_dll.toStdString().c_str()))) {
	//	return false;
	//}
	return true;
}

bool LibManager::RegisterDll(const wchar_t* dll) {
   HINSTANCE dllInterface = LoadLibrary(dll);
   if (dllInterface == NULL)
   {
       int err = GetLastError();
      return false;
   }



   void *Register = GetProcAddress(dllInterface, "RegisterGlobalManager");
   if (Register != NULL)
   {
      bool ret = ((bool(*)(void*))Register)(mToolManager);
   }
   else {
      FreeLibrary(dllInterface);
      dllInterface = NULL;
      return false;
   }
   return true;
}
