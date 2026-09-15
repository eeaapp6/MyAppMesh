/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorOpenRadiossSolve.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "GUIDialog/GUIPostDialog/GUISolverRunDialog.h"
#include <QDir>
#include <QDebug>
#include <QProcessEnvironment>

namespace ModelOper
{
    bool OperatorOpenRadiossSolve::execGUI()
    {
        //获取文件路径
        GUI::GUISolverRunDialog dialog(this, FITKAPP->getGlobalData()->getMainWindow());
        int res = dialog.exec();
        if (res == QDialog::Accepted)
            return true;
        return false;
    }

    bool OperatorOpenRadiossSolve::execProfession()
    {
        if (!this->writeSolverConfigFile()) return false;
        //获取线程池
        Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
        if (pool == nullptr)return false;
        pool->wait();
        //开始求解
        this->startSolver();
        return true;
    }

    void OperatorOpenRadiossSolve::solverFinishedSlot()
    {
        this->setArgs("SolverStatus", 0);
        AppFrame::FITKMessageNormal(QString("Solver Finish !!!"));
        //格式转化
        QString workDir, caseName;
        if (!this->argValue("WorkDir", workDir)) return;
        if (!this->argValue("CaseName", caseName)) return;
        //格式转化
        Core::FITKActionOperator* oper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionFormatAnimToVTK");
        if (!oper) return;
        oper->setArgs("ActionName", "actionFormatAnimToVTK");
        oper->setArgs("WorkDir", workDir);
        oper->setArgs("CaseName", caseName);
        oper->execProfession();
        //格式转化
        oper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionFormatTHToCSV");
        if (!oper) return;
        oper->setArgs("ActionName", "actionFormatTHToCSV");
        oper->setArgs("WorkDir", workDir);
        oper->setArgs("CaseName", caseName);
        oper->execProfession();
        //获取算例ID
        int caseDataID = -1, solutionID = -1;
        this->argValue<int>("CaseDataID", caseDataID);
        this->argValue<int>("SolutionID", solutionID);
        //读取结果
        QDir dir_work(workDir);
        QStringList files = dir_work.entryList(QStringList() << "*.vtk", QDir::Files | QDir::NoSymLinks);
        QStringList csvfiles = dir_work.entryList(QStringList() << "*.csv", QDir::Files | QDir::NoSymLinks);
        QStringList vtkFiles;
        for (int i = 1; i > 0; ++i)
        {
            QString vtkFilter = QString("%1.vtk").arg(QString::number(i));
            QString fileName;
            bool isExist = false;
            for (QString file : files)
            {
                if (!file.startsWith(caseName) || !file.endsWith(vtkFilter))continue;
                fileName = file;
                isExist = true;
                break;
            }
            if (!isExist)
                break;
            vtkFiles.append(QString("%1/%2").arg(workDir).arg(fileName));
        }
        this->setArgs("vtkResultPath", workDir);
        //调用读取操作器
        if (!vtkFiles.isEmpty()) {
            oper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionImportVTKResult");
            if (!oper) return;

            oper->setArgs("FileNames", vtkFiles);
            oper->setArgs("CaseDataID", caseDataID);
            oper->setArgs("SolutionID", solutionID);
            oper->execProfession();
        }
        if (!csvfiles.isEmpty()) {
            oper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionImportCSVResult");
            if (!oper) return;
            QString csvFullPath = QString("%1/%2").arg(workDir).arg(csvfiles[0]);
            oper->setArgs("ImportFile", csvFullPath); // 传入完整路径
            oper->setArgs("CaseID", caseDataID);
            oper->setArgs("SolutionID", solutionID);
            oper->execProfession();
        }
    }

    bool OperatorOpenRadiossSolve::writeSolverConfigFile()
    {
        QString workDir, caseName, starterFileName, engineFileName;
        bool isImport;
        if (!this->argValue<QString>("WorkDir", workDir)) return false;
        if (!this->argValue<QString>("CaseName", caseName)) return false;
        if (!this->argValue<bool>("isImport", isImport)) return false;
        if (!this->argValue<QString>("StarterFileName", starterFileName)) return false;
        if (!this->argValue<QString>("EngineFileName", engineFileName)) return false;
        //判断工作目录是否存在
        if (!QDir(workDir).exists())
        {
            QDir(workDir).mkpath(".");
        }

        //清理结果文件，避免干扰
        QStringList fileList = QDir(workDir).entryList(QDir::Files);
        foreach(QString fileName, fileList) {
            QString filePath = QString("%1/%2").arg(workDir).arg(fileName);
            if (!fileName.startsWith(caseName))continue;
            if (fileName.endsWith(".rad")) continue;
            QFile::remove(filePath);
        }

        //根据名称从仓库查找
        if (!isImport) {
            //清理配置文件
            QFile::remove(starterFileName);
            QFile::remove(engineFileName);
            //写出新的配置文件
            Core::FITKActionOperator* oper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionExportRad");
            if (!oper) return false;
            oper->setArgs("FileName", QString("%1/%2.rad").arg(workDir).arg(caseName));
            oper->execProfession();
        }
        
    }

    void OperatorOpenRadiossSolve::startSolver()
    {
        QString solverConfigFile, workDir;
        if (!this->argValue("StarterFileName", solverConfigFile)) return;
        if (!this->argValue("WorkDir", workDir)) return;
        // 第三方程序执行任务管理器
        AppFrame::FITKProgramTaskManeger* programTaskMgr = FITKAPP->getProgramTaskManager();
        if (!programTaskMgr) return;
        //创建命令行参数
        FITKOpenRadiossExecProgramInputInfo* inputInfo = new FITKOpenRadiossExecProgramInputInfo;
        inputInfo->setSolverConfigFile(solverConfigFile);
        //创建HQIgo驱动
        FITKOpenRadiossStarterExecProgramDriver* openRadioss_StarterExec = dynamic_cast<FITKOpenRadiossStarterExecProgramDriver*>(programTaskMgr->createProgram(3, "FITKOpenRadiossStarterExecProgramDriver", inputInfo));
        if (!openRadioss_StarterExec)return;
        openRadioss_StarterExec->setWorkingDirectory(workDir);
        if (!openRadioss_StarterExec->isExistExeProgram())
        {
            delete openRadioss_StarterExec;
            return;
        }
        connect(openRadioss_StarterExec, SIGNAL(sig_Finish()), this, SLOT(starterFinished_EngineSolverSlot()));
        AppFrame::FITKMessageNormal(QString("**Run Starter**"));
        openRadioss_StarterExec->start();
        this->setArgs("SolverStatus", 1);
    }

    void OperatorOpenRadiossSolve::starterFinished_EngineSolverSlot()
    {
        QString solverConfigFile, workDir;
        int threadNum = 1;
        if (!this->argValue("EngineFileName", solverConfigFile)) return;
        if (!this->argValue("WorkDir", workDir)) return;
        if (!this->argValue("ThreadNum", threadNum)) return;
        // 第三方程序执行任务管理器
        AppFrame::FITKProgramTaskManeger* programTaskMgr = FITKAPP->getProgramTaskManager();
        if (!programTaskMgr) return;
        //创建命令行参数
        FITKOpenRadiossExecProgramInputInfo* inputInfo = new FITKOpenRadiossExecProgramInputInfo;
        inputInfo->setSolverConfigFile(solverConfigFile);
        inputInfo->setThreadNum(threadNum);
        //计算结果
        FITKOpenRadiossEngineExecProgramDriver* openRadioss_EngineExec = dynamic_cast<FITKOpenRadiossEngineExecProgramDriver*>(programTaskMgr->createProgram(3, "FITKOpenRadiossEngineExecProgramDriver", inputInfo));
        if (!openRadioss_EngineExec)return;
        openRadioss_EngineExec->setWorkingDirectory(workDir);
        if (!openRadioss_EngineExec->isExistExeProgram())
        {
            delete openRadioss_EngineExec;
            return;
        }
        connect(openRadioss_EngineExec, SIGNAL(sig_Finish()), this, SLOT(solverFinishedSlot()));
        AppFrame::FITKMessageNormal(QString("**Run Engine**"));
        openRadioss_EngineExec->start();
    }



    FITKOpenRadiossStarterExecProgramDriver::FITKOpenRadiossStarterExecProgramDriver()
    {
        QString openRadiossPath = QDir(QCoreApplication::applicationDirPath() + QString("/OpenRadioss")).absolutePath();
        //获取求解器路径
        QString solverPath;
#ifdef Q_OS_LINUX
        solverPath = openRadiossPath + QString("/exec/starter_linux64_gf");
#elif defined(Q_OS_WIN)
        solverPath = openRadiossPath + QString("/exec/starter_win64.exe");
#endif

        if (!QFile::exists(solverPath))
        {
            AppFrame::FITKMessageError(QString("FITKOpenRadiossStarterExecProgramDriver: OpenRadioss starter executable not found : %1").arg(solverPath));
            qDebug() << QString("FITKOpenRadiossStarterExecProgramDriver: OpenRadioss starter executable not found : %1").arg(solverPath);
        }
        this->setExecProgram(solverPath);
        // 获取系统环境变量
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        //配置求解器的环境变量
        QString rad_cfg_path, rad_h3d_path, add_env_path;
#ifdef Q_OS_LINUX
        rad_cfg_path = openRadiossPath + QString("/hm_cfg_files");
        rad_h3d_path = openRadiossPath + QString("/extlib/h3d/lib/linux64");
        add_env_path = QString("%1:%2:").arg(openRadiossPath + QString("/extlib/hm_reader/win64")).arg(openRadiossPath + QString("/extlib/h3d/lib/win64"));
        env.insert("RAD_CFG_PATH", rad_cfg_path);//设置配置文件路径
        env.insert("RAD_H3D_PATH", rad_h3d_path);//设置H3D路径
        env.insert("OMP_STACKSIZE", "400m");//设置栈大小
        env.insert("LD_LIBRARY_PATH", add_env_path + env.value("LD_LIBRARY_PATH"));
#elif defined(Q_OS_WIN)
        rad_cfg_path = openRadiossPath + QString("/hm_cfg_files");
        rad_h3d_path = openRadiossPath + QString("/extlib/h3d/lib/win64");
        add_env_path = QString("%1;%2;%3;").arg(openRadiossPath + QString("/extlib/hm_reader/win64")).arg(openRadiossPath + QString("/extlib/h3d/lib/win64")).arg(openRadiossPath + QString("/extlib/intelOneAPI_runtime/win64"));
        env.insert("RAD_CFG_PATH", rad_cfg_path);//设置配置文件路径
        env.insert("RAD_H3D_PATH", rad_h3d_path);//设置H3D路径
        env.insert("OMP_STACKSIZE", "400m");//设置栈大小
        env.insert("PATH", add_env_path + env.value("PATH"));
#endif
        // 将环境变量设置给进程
        _process->setProcessEnvironment(env);

    }
    
    FITKOpenRadiossStarterExecProgramDriver::~FITKOpenRadiossStarterExecProgramDriver()
    {
    }

    int FITKOpenRadiossStarterExecProgramDriver::getProgramType()
    {
        return 3;
    }

    QString FITKOpenRadiossStarterExecProgramDriver::getProgramName()
    {
        return "FITKOpenRadiossStarterExecProgramDriver";
    }

    void FITKOpenRadiossStarterExecProgramDriver::setOpenMPThreadNum(QString num)
    {
        //设置openMP线程数
        QProcessEnvironment env = _process->processEnvironment();
        env.insert("OMP_NUM_THREADS", num);
        _process->setProcessEnvironment(env);
    }

    bool FITKOpenRadiossStarterExecProgramDriver::isExistExeProgram()
    {
        return QFileInfo(this->getExecProgram()).isFile();
    }





    FITKOpenRadiossEngineExecProgramDriver::FITKOpenRadiossEngineExecProgramDriver()
    {
        QString openRadiossPath = QDir(QCoreApplication::applicationDirPath() + QString("/OpenRadioss")).absolutePath();
        //获取求解器路径
        QString solverPath;
#ifdef Q_OS_LINUX
        solverPath = openRadiossPath + QString("/exec/engine_linux64_gf");
#elif defined(Q_OS_WIN)
        solverPath = openRadiossPath + QString("/exec/engine_win64.exe");
#endif

        if (!QFile::exists(solverPath))
        {
            AppFrame::FITKMessageError(QString("FITKOpenRadiossStarterExecProgramDriver: OpenRadioss starter executable not found : %1").arg(solverPath));
            qDebug() << QString("FITKOpenRadiossStarterExecProgramDriver: OpenRadioss starter executable not found : %1").arg(solverPath);
        }
        this->setExecProgram(solverPath);
        // 获取系统环境变量
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        //配置求解器的环境变量
        QString rad_cfg_path, rad_h3d_path, add_env_path;
#ifdef Q_OS_LINUX
        rad_cfg_path = openRadiossPath + QString("/hm_cfg_files");
        rad_h3d_path = openRadiossPath + QString("/extlib/h3d/lib/linux64");
        add_env_path = QString("%1:%2:").arg(openRadiossPath + QString("/extlib/hm_reader/win64")).arg(openRadiossPath + QString("/extlib/h3d/lib/win64"));
        env.insert("RAD_CFG_PATH", rad_cfg_path);//设置配置文件路径
        env.insert("RAD_H3D_PATH", rad_h3d_path);//设置H3D路径
        env.insert("OMP_STACKSIZE", "400m");//设置栈大小
        env.insert("LD_LIBRARY_PATH", add_env_path + env.value("LD_LIBRARY_PATH"));
#elif defined(Q_OS_WIN)
        rad_cfg_path = openRadiossPath + QString("/hm_cfg_files");
        rad_h3d_path = openRadiossPath + QString("/extlib/h3d/lib/win64");
        add_env_path = QString("%1;%2;%3;").arg(openRadiossPath + QString("/extlib/hm_reader/win64")).arg(openRadiossPath + QString("/extlib/h3d/lib/win64")).arg(openRadiossPath + QString("/extlib/intelOneAPI_runtime/win64"));
        env.insert("RAD_CFG_PATH", rad_cfg_path);//设置配置文件路径
        env.insert("RAD_H3D_PATH", rad_h3d_path);//设置H3D路径
        env.insert("OMP_STACKSIZE", "400m");//设置栈大小
        env.insert("PATH", add_env_path + env.value("PATH"));
#endif
        // 将环境变量设置给进程
        _process->setProcessEnvironment(env);
    }

    FITKOpenRadiossEngineExecProgramDriver::~FITKOpenRadiossEngineExecProgramDriver()
    {
    }

    int FITKOpenRadiossEngineExecProgramDriver::getProgramType()
    {
        return 3;
    }

    QString FITKOpenRadiossEngineExecProgramDriver::getProgramName()
    {
        return "FITKOpenRadiossEngineExecProgramDriver";
    }

    void FITKOpenRadiossEngineExecProgramDriver::setOpenMPThreadNum(QString num)
    {
        //设置openMP线程数
        QProcessEnvironment env = _process->processEnvironment();
        env.insert("OMP_NUM_THREADS", num);
        _process->setProcessEnvironment(env);
    }

    bool FITKOpenRadiossEngineExecProgramDriver::isExistExeProgram()
    {
        return QFileInfo(this->getExecProgram()).isFile();
    }





    FITKOpenRadiossExecProgramInputInfo::FITKOpenRadiossExecProgramInputInfo()
    {
    }

    FITKOpenRadiossExecProgramInputInfo::~FITKOpenRadiossExecProgramInputInfo()
    {
    }

    void FITKOpenRadiossExecProgramInputInfo::setSolverConfigFile(QString solverConfigFile)
    {
        _solverConfigFile = solverConfigFile;
    }

    void FITKOpenRadiossExecProgramInputInfo::setThreadNum(int num)
    {
        _threadNum = num;
    }

    QStringList FITKOpenRadiossExecProgramInputInfo::args()
    {
        QString command = QString("-i %1 -nt %2").arg(_solverConfigFile).arg(_threadNum);
        return command.split(' ');
    }

}

