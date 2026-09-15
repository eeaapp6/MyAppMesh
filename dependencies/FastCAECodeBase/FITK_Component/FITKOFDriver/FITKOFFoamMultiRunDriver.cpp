/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFFoamMultiRunDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"

#include <QDebug>
namespace FoamDriver
{
    int FITKOFFoamMultiRunDriver::getProgramType()
    {
        return 2;
    }

    QString FITKOFFoamMultiRunDriver::getProgramName()
    {
        return "foamMultiRun";
    }

    void FITKOFFoamMultiRunDriver::start()
    {
        //输入参数判空
        if (!_inputInfo) return;

        // 获取blockMesh启动参数
        QStringList formMultiRunArguments = _inputInfo->args();

        qDebug() << "formMultiRunArguments:" << formMultiRunArguments;

        // 获取监控的路径
        QStringList monitorPaths = _inputInfo->getMonitorPath();

        qDebug() << "monitorPaths:" << monitorPaths;

        QString foamMultiRunCommond = "foamMultiRun " + formMultiRunArguments.join(" ");

        qDebug() << "foamMultiRunCommond:" << foamMultiRunCommond;


        // 判断监控路径是否存在
        if (_commandRunner->isExistDictionary(monitorPaths))
        {
            // 执行命令
            connect(_commandRunner, SIGNAL(taskFinishedSig(FITKThreadTask*)), this, SLOT(threadFinishedSlot()), Qt::UniqueConnection);
            _commandRunner->setExecuteCommand(foamMultiRunCommond);
            _commandRunner->push2ThreadPool();
        }
    }

    void FITKOFFoamMultiRunDriver::stop()
    {
        _commandRunner->killProcess(_commandRunner->getID());
    }
}

