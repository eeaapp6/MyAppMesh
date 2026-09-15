/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFBlockMeshDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"
// QT
#include <qdebug.h>

namespace FoamDriver
{
    int FITKOFBlockMeshDriver::getProgramType()
    {
        return 1;
    }

    QString FITKOFBlockMeshDriver::getProgramName()
    {
        return "blockMesh";
    }

    void FITKOFBlockMeshDriver::start()
    {
        //输入参数判空
        if (!_inputInfo) return;

        // 获取blockMesh启动参数
        QStringList blockMeshArguments = _inputInfo->args();
        qDebug() << "blockMeshArguments:" << blockMeshArguments;

        // 获取监控的路径
        QStringList monitorPaths = _inputInfo->getMonitorPath();
        qDebug() << "monitorPaths:" << monitorPaths;

        QString blockMeshCommond = "blockMesh " + blockMeshArguments.join(" ");
        qDebug() << "blockMeshCommond:" << blockMeshCommond;
        int indexError = -1;
        // 判断监控路径是否存在
        if (!_commandRunner->isExistDictionary(monitorPaths, indexError))
        {
            QString str;
            if (indexError != -1)
            {
                str = QString("Not Exist Dictionary Path : %1").arg(monitorPaths[indexError]);
                sendMessage(100101, str);
            }
        }
        // 执行命令并捕获其输出
        connect(_commandRunner, SIGNAL(taskFinishedSig(FITKThreadTask*)), this, SLOT(threadFinishedSlot()), Qt::UniqueConnection);

        _commandRunner->setExecuteCommand(blockMeshCommond);
        _commandRunner->push2ThreadPool();
    }

    void FITKOFBlockMeshDriver::stop()
    {
        _commandRunner->killProcess(_commandRunner->getID());
    }
}


