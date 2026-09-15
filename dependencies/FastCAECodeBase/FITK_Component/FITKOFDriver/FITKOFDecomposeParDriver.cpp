/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFDecomposeParDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractCommandRunner.h"
// QT
#include <QDebug>
namespace FoamDriver
{
    int FITKOFDecomposeParDriver::getProgramType()
    {
        return 1;
    }

    QString FITKOFDecomposeParDriver::getProgramName()
    {
        return "decomposePar";
    }

    void FITKOFDecomposeParDriver::start()
    {
        //输入参数判空
        if (!_inputInfo) return;

        // 获取blockMesh启动参数
        QStringList decomposeParArguments = _inputInfo->args();

        qDebug() << "decomposeParArguments:" << decomposeParArguments;

        // 获取监控的路径
        QStringList monitorPaths = _inputInfo->getMonitorPath();

        qDebug() << "monitorPaths:" << monitorPaths;

        QString decomposeParCommond = "blockMesh " + decomposeParArguments.join(" ");

        qDebug() << "decomposeParCommond:" << decomposeParCommond;

        // 判断监控路径是否存在
        if (_commandRunner->isExistDictionary(monitorPaths))
        {
            // 执行命令并捕获其输出
            connect(_commandRunner, SIGNAL(taskFinishedSig(FITKThreadTask*)), this, SLOT(threadFinishedSlot()), Qt::UniqueConnection);

            _commandRunner->setExecuteCommand(decomposeParCommond);
            _commandRunner->push2ThreadPool();
        }
    }

    void FITKOFDecomposeParDriver::stop()
    {
        _commandRunner->killProcess(_commandRunner->getID());
    }


    //    bool FITKOFDecomposeParDriver::runCommand(const QString &command)
    //    {
    //        try
    //        {
    //            // 执行命令并捕获其输出
    //            QString output = executeCommand(command, _processId);
    //            captureOutput(output);
    //            return true;
    //        }
    //        catch (const std::runtime_error &e)
    //        {
    //            emit commandError(e.what());
    //            return false;
    //        }
    //    }

    //    void FITKOFDecomposeParDriver::captureOutput(const QString &output)
    //    {
    //        emit commandOutput(output);
    //        qDebug() << output;
    //    }

    //    qint64 FITKOFDecomposeParDriver::getProcessId() const
    //    {
    //        return _processId;
    //    }

    //    bool FITKOFDecomposeParDriver::executeDecomposePar(const QString &casePath, const QStringList &options)
    //    {
    //        // 设置当前算例路径
    //        setWorkDirect(casePath);

    //        bool isSuccess = false;

    //        QString decomposeParOptions = parseDecomposeParOptions(casePath, options);

    //        QString command = QString("decomposePar %1").arg(decomposeParOptions);

    //        // 检查算例路径下是否包含必须文件
    //        if (isExistDictionary(casePath))
    //        {
    //            isSuccess = runCommand(command);
    //        }

    //        return isSuccess;
    //    }

    //    QString  FITKOFDecomposeParDriver::parseDecomposeParOptions(const QString &casePath, const QStringList &options)
    //    {
    //        // 选项结果
    //        QString optionResult;

    //        // 使用不区分大小写的包含检查
    //        for (const QString &option : options)
    //        {
    //            if (option.compare("case", Qt::CaseInsensitive) == 0)
    //            {
    //                // 指定要使用的案例目录（而不是 cwd）
    //                optionResult += "-case " + casePath;
    //            }
    //            else if (option.compare("allRegions", Qt::CaseInsensitive) == 0)
    //            {
    //                // operate on all regions in regionSolvers
    //                optionResult += " -allRegions";
    //            }
    //            else if (option.compare("cellProc", Qt::CaseInsensitive) == 0)
    //            {
    //                // write cell processor indices as a volScalarField::Internal for post-processing
    //                optionResult += " -cellProc";
    //            }
    //            else if (option.compare("constant", Qt::CaseInsensitive) == 0)
    //            {
    //                // include the 'constant/' dir in the times list
    //                optionResult += " -constant";
    //            }
    //            else if (option.compare("copyUniform", Qt::CaseInsensitive) == 0)
    //            {
    //                // copy any uniform/ directories too
    //                optionResult += " -copyUniform";
    //            }
    //            else if (option.compare("copyZero", Qt::CaseInsensitive) == 0)
    //            {
    //                //Copy  0 directory to processor* rather than decompose the fields
    //                optionResult += " -copyZero";
    //            }
    //            else if (option.compare("fields", Qt::CaseInsensitive) == 0)
    //            {
    //                //use existing geometry decomposition and convert fields only
    //                optionResult += " -fields";
    //            }
    //            else if (option.compare("noFields", Qt::CaseInsensitive) == 0)
    //            {
    //                //opposite of -fields; only decompose geometry
    //                optionResult += " -noFields";
    //            }
    //        }

    //        return optionResult;
    //    }

    //    bool FITKOFDecomposeParDriver::isExistDictionary(const QString &dicFilePath)
    //    {
    //        // decomposeParDict 字典，用来分解网格和场，位于 system 子文件夹下
    //        QString getNumberSubdomainsRequiredDict = QString("%1/system/decomposeParDict").arg(dicFilePath);

    //        // 使用 QFileInfo 判断是否存在
    //        QFileInfo checkFile(getNumberSubdomainsRequiredDict);
    //        if (checkFile.exists())
    //        {
    //            return true;
    //        }
    //        return false;
    //    }

}
