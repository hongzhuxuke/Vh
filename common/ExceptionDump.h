#ifndef __EXCEPTION_DUMP_H_
#define __EXCEPTION_DUMP_H_

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

#include <fstream>
#include <time.h>

using namespace std;
void add_log(const char *logfile, const char *fmt, ...);

class CExceptionDump
{
public:
    CExceptionDump(void);
    ~CExceptionDump(void);

private:
    //初始化
    void Init();

    //反初始化
    void UnInit();

private:
    LPTOP_LEVEL_EXCEPTION_FILTER m_pSystemFilter;
};

#endif //__EXCEPTION_DUMP_H_