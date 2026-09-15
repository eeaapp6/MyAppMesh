/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorOpenRadiossFormatConversion.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include <QDir>
#include <QDebug>
#include <QProcessEnvironment>

namespace ModelOper
{
    bool OperatorOpenRadiossFormatConversion::execGUI()
    {
        if (!_emitter) return false;
        QString name = _emitter->objectName();
        this->setArgs("ActionName", name);
        //获取工作目录
        QString wk;
        auto settings = FITKAPP->getAppSettings();
        if (settings)
            wk = settings->getWorkingDir();
        if (wk.isEmpty())
        {
            AppFrame::FITKMessageError(QString("WorkingDir Err! Solution failed."));
            return false;
        }
        this->setArgs("WorkDir", wk);
        return true;
    }

    bool OperatorOpenRadiossFormatConversion::execProfession()
    {
        QString actionName;
        if (!this->argValue<QString>("ActionName", actionName)) return false;
        if (actionName == "actionFormatAnimToVTK")
            this->formatAnimToVTK();
        else if (actionName == "actionFormatTHToCSV")
            this->formatTHToCSV();
        return true;
    }

    void OperatorOpenRadiossFormatConversion::formatAnimToVTK()
    {
        QString caseName, workDir;
        if (!this->argValue<QString>("CaseName", caseName)) return;
        if (!this->argValue<QString>("WorkDir", workDir)) return;
        QString openRadiossPath = QDir(QCoreApplication::applicationDirPath() + QString("/OpenRadioss")).absolutePath();

        QStringList vtkFiles;
        //获取exec路径
        QString execPath;
#ifdef Q_OS_LINUX
        execPath = openRadiossPath + QString("/exec/anim_to_vtk_linux64_gf");
#elif defined(Q_OS_WIN)
        execPath = openRadiossPath + QString("/exec/anim_to_vtk_win64.exe");
#endif
        if (!QFile::exists(execPath))
        {
            AppFrame::FITKMessageError(QString("OpenRadioss Format executable not found : %1 !").arg(execPath));
            return;
        }
        //数据转换
        QString fileName = caseName + "A";
        QFileInfoList fileList = QDir(workDir).entryInfoList(QDir::Files);
        foreach(QFileInfo fileInfo, fileList) {
            QString inputFileName = fileInfo.fileName();
            if (!inputFileName.contains(fileName) || inputFileName.endsWith(".vtk"))continue;
            int index = inputFileName.lastIndexOf('A');
            QString outputFileName = inputFileName;
            outputFileName.replace(index, 1, "_");
            outputFileName.append(".vtk");
            //设置命令行参数
            QString command = QString("%1 %2").arg(execPath).arg(inputFileName);
            QString fullCommand;
#ifdef Q_OS_LINUX
            fullCommand = QString("sh -c\"%1 > %2\"").arg(command).arg(outputFileName);
#elif defined(Q_OS_WIN)
            fullCommand = QString("cmd /c\"%1 > %2\"").arg(command).arg(outputFileName);
#endif
            //运行格式转换
            QProcess process;
            process.setWorkingDirectory(workDir);
            process.start(fullCommand);
            if (process.waitForFinished(-1))
                AppFrame::FITKMessageNormal(QString("VTK format conversion finish : %1 to %2").arg(inputFileName).arg(outputFileName));
            else
                AppFrame::FITKMessageError(QString("VTK format conversion err : %1 to %2").arg(inputFileName).arg(outputFileName));
        }
    }

    void OperatorOpenRadiossFormatConversion::formatTHToCSV()
    {
        QString caseName, workDir;
        if (!this->argValue<QString>("CaseName", caseName)) return;
        if (!this->argValue<QString>("WorkDir", workDir)) return;
        QString openRadiossPath = QDir(QCoreApplication::applicationDirPath() + QString("/OpenRadioss")).absolutePath();
        //获取exec路径
        QString execPath;
#ifdef Q_OS_LINUX
        execPath = openRadiossPath + QString("/exec/th_to_csv");
#elif defined(Q_OS_WIN)
        execPath = openRadiossPath + QString("/exec/th_to_csv_win64.exe");
#endif
        if (!QFile::exists(execPath))
        {
            AppFrame::FITKMessageError(QString("OpenRadioss Format executable not found : %1 !").arg(execPath));
            return;
        }
        //数据转换
        QString fileName = caseName + "A";
        QFileInfoList fileList = QDir(workDir).entryInfoList(QDir::Files);
        foreach(QFileInfo fileInfo, fileList) {
            QString inputFileName = fileInfo.fileName();
            if (inputFileName != QString("%1T01").arg(caseName))continue;
            //设置命令行参数
            QString command = QString("%1 %2").arg(execPath).arg(inputFileName);
            QString fullCommand;
#ifdef Q_OS_LINUX
            fullCommand = QString("sh -c\"%1\"").arg(command);
#elif defined(Q_OS_WIN)
            fullCommand = QString("cmd /c\"%1\"").arg(command);
#endif
            //运行格式转换
            QProcess process;
            process.setWorkingDirectory(workDir);
            process.start(fullCommand);
            if (process.waitForFinished(-1))
                AppFrame::FITKMessageNormal(QString("CSV format conversion finish : %1").arg(inputFileName));
            else
                AppFrame::FITKMessageError(QString("CSV format conversion err : %1").arg(inputFileName));
        }
    }




    FITKOpenRadiossToVTKFormatExecProgramDriver::FITKOpenRadiossToVTKFormatExecProgramDriver()
    {
        QString openRadiossPath = QDir(QCoreApplication::applicationDirPath() + QString("/OpenRadioss")).absolutePath();
        //获取exec路径
        QString execPath;
#ifdef Q_OS_LINUX
        execPath = openRadiossPath + QString("/exec/anim_to_vtk_linux64_gf");
#elif defined(Q_OS_WIN)
        execPath = openRadiossPath + QString("/exec/anim_to_vtk_win64.exe");
#endif

        if (!QFile::exists(execPath))
        {
            AppFrame::FITKMessageError(QString("FITKOpenRadiossToVTKFormatExecProgramDriver: OpenRadioss Format executable not found : %1").arg(execPath));
            qDebug() << QString("FITKOpenRadiossToVTKFormatExecProgramDriver: OpenRadioss Format executable not found : %1").arg(execPath);
        }
        this->setExecProgram(execPath);
    }
    
    FITKOpenRadiossToVTKFormatExecProgramDriver::~FITKOpenRadiossToVTKFormatExecProgramDriver()
    {
    }

    int FITKOpenRadiossToVTKFormatExecProgramDriver::getProgramType()
    {
        return 3;
    }

    QString FITKOpenRadiossToVTKFormatExecProgramDriver::getProgramName()
    {
        return "FITKOpenRadiossToVTKFormatExecProgramDriver";
    }

    bool FITKOpenRadiossToVTKFormatExecProgramDriver::isExistExeProgram()
    {
        return QFileInfo(this->getExecProgram()).isFile();
    }

    void ModelOper::FITKOpenRadiossToVTKFormatExecProgramDriver::waitForFinished(int time)
    {
        _process->waitForFinished(-1);
    }





    FITKOpenRadiossToCSVFormatExecProgramDriver::FITKOpenRadiossToCSVFormatExecProgramDriver()
    {
        QString openRadiossPath = QDir(QCoreApplication::applicationDirPath() + QString("/OpenRadioss")).absolutePath();
        //获取exec路径
        QString execPath;
#ifdef Q_OS_LINUX
        execPath = openRadiossPath + QString("/exec/th_to_csv");
#elif defined(Q_OS_WIN)
        execPath = openRadiossPath + QString("/exec/th_to_csv_win64.exe");
#endif

        if (!QFile::exists(execPath))
        {
            AppFrame::FITKMessageError(QString("FITKOpenRadiossToCSVFormatExecProgramDriver: OpenRadioss Format executable not found : %1").arg(execPath));
            qDebug() << QString("FITKOpenRadiossToCSVFormatExecProgramDriver: OpenRadioss Format executable not found : %1").arg(execPath);
        }
        this->setExecProgram(execPath);

    }

    FITKOpenRadiossToCSVFormatExecProgramDriver::~FITKOpenRadiossToCSVFormatExecProgramDriver()
    {
    }

    int FITKOpenRadiossToCSVFormatExecProgramDriver::getProgramType()
    {
        return 3;
    }

    QString FITKOpenRadiossToCSVFormatExecProgramDriver::getProgramName()
    {
        return "FITKOpenRadiossToCSVFormatExecProgramDriver";
    }

    bool FITKOpenRadiossToCSVFormatExecProgramDriver::isExistExeProgram()
    {
        return QFileInfo(this->getExecProgram()).isFile();
    }





    FITKOpenRadiossToVTKExecProgramInputInfo::FITKOpenRadiossToVTKExecProgramInputInfo()
    {
    }

    FITKOpenRadiossToVTKExecProgramInputInfo::~FITKOpenRadiossToVTKExecProgramInputInfo()
    {
    }

    void FITKOpenRadiossToVTKExecProgramInputInfo::setInputFile(QString inputFile)
    {
        _inputFile = inputFile;
    }

    void FITKOpenRadiossToVTKExecProgramInputInfo::setOutputFile(QString outputFile)
    {
        _outputFile = outputFile;
    }

    QStringList FITKOpenRadiossToVTKExecProgramInputInfo::args()
    {
        QString command = QString("%1 > %2").arg(_inputFile).arg(_outputFile);
        return command.split(' ');
    }





    FITKOpenRadiossToCSVExecProgramInputInfo::FITKOpenRadiossToCSVExecProgramInputInfo()
    {
    }

    FITKOpenRadiossToCSVExecProgramInputInfo::~FITKOpenRadiossToCSVExecProgramInputInfo()
    {
    }

    void FITKOpenRadiossToCSVExecProgramInputInfo::setInputFile(QString inputFile)
    {
        _inputFile = inputFile;
    }

    QStringList FITKOpenRadiossToCSVExecProgramInputInfo::args()
    {
        QString command = QString("%1").arg(_inputFile);
        return command.split(' ');
    }
}

