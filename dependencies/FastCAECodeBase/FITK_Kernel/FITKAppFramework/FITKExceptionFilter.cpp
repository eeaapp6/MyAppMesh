/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKExceptionFilter.h"
#include <QString>
#include "FITKAppFramework.h"

static QString DumpFileName ;

#ifdef Q_OS_WIN
#include <windows.h>
#include <DbgHelp.h>

LONG WINAPI FITKUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    // ASCII字符串
    QByteArray ba = DumpFileName.toLocal8Bit();
    const char* asciiString = ba.data();

    // 获取需要的缓冲区大小
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, asciiString, -1, NULL, 0);

    // 分配缓冲区
    wchar_t* wideString = new wchar_t[bufferSize];

    // 转换为Unicode字符串
    MultiByteToWideChar(CP_UTF8, 0, asciiString, -1, wideString, bufferSize);
    

    HANDLE lhDumpFile = CreateFileW(wideString, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;

    loExceptionInfo.ExceptionPointers = ExceptionInfo;

    loExceptionInfo.ThreadId = GetCurrentThreadId();

    loExceptionInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
 //   MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, MiniDumpWithFullMemory, &loExceptionInfo, NULL, NULL);

    CloseHandle(lhDumpFile);
    

    // 释放内存
    delete[] wideString;
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

AppFrame::FITKExceptionFilter::FITKExceptionFilter()
{

#ifdef Q_OS_WIN
    //dump文件路径
    DumpFileName = FITKAPP->applicationDirPath() + "/FITKAppDump.dmp";
    //注册函数到操作系统
    SetUnhandledExceptionFilter(FITKUnhandledExceptionFilter);
#endif

}

 
