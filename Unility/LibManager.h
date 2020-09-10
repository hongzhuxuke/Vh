#pragma once

#include <QObject>

class ToolManager;
class LibManager : public QObject
{
   Q_OBJECT

public:
    LibManager(ToolManager* toolManager,QObject *parent = nullptr);
    ~LibManager();

    bool InitDll();

	
private:

   bool RegisterDll(const wchar_t* dll);
private:
   ToolManager * mToolManager = nullptr;
};
