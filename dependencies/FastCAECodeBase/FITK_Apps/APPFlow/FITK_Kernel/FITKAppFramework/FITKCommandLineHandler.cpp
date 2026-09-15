/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCommandLineHandler.h"
#include "FITKAppFramework.h"
#include "FITKCore/FITKLibInfo.h"
#include <QCoreApplication>
#include <QDebug>

namespace AppFrame
{
    FITKCommandLineHandler::FITKCommandLineHandler()
    {
        //获取全部参数 mpi运行时，会出现错误导致崩溃，原因未知
        bool runInMPI = FITKAPP->getRunInMPI();
        if(runInMPI) return;
        QCoreApplication* ins =  QCoreApplication::instance();
         if(ins != nullptr)
             _args = ins->arguments();
    }

    void FITKCommandLineHandler::exec()
    {
        if (this->isHelp())
        {
            this->printHelp();
            QCoreApplication::exit();
        }

        if(this->isPrintLibInfo())
            FITKLIBINFO->printAllLibraryInfo();
    }

    bool FITKCommandLineHandler::isNoGUI()
    {
        //遍历全部的参数
        for (QString s : _args)
        {
            //移除-号
            if (s.remove("-").toLower() == "nogui")
                return true;
        }
        return false;
    }

    bool FITKCommandLineHandler::isPrintLibInfo()
    {
        //遍历全部的参数
        for (QString s : _args)
        {
            //移除-号
            if (s.remove("-").toLower() == "libinfo")
                return true;
        }
        //没有找到参数
        return false;
    }

    bool FITKCommandLineHandler::isHelp()
    {
        //遍历全部的参数
        for (QString s : _args)
        {
            //移除-号
            if (s.remove("-").toLower() == "help")
                return true;
        }
        //没有找到参数
        return false;
    }

    QString FITKCommandLineHandler::argValue(const int index)
    {
        //错误值返回空值
        if (index < 0 || index >= _args.size())
            return QString();
        return _args.value(index);
    }

    bool FITKCommandLineHandler::containsValue(QString v)
    {
        //遍历全部的参数
        for (QString s : _args)
        {
            if (s.toLower() == v.toLower())
                return true;
        }
        return false;
    }

    void FITKCommandLineHandler::printHelp()
    {
        qDebug() << "FITKAppFramework Options:";
        qDebug() << "  -nogui       Run without GUI";
        qDebug() << "  -libinfo     Print library version and information";
        qDebug() << "  -help        Show this help message";
        qDebug() << "  ";
        
        QCoreApplication::exit(0);

    }
}



