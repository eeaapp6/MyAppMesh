/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SolverWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include <QDir>

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

//初始化成员变量
Solver::Solver()
{
    _workDir = FITKAPP->getAppSettings()->getWorkingDir();
}

bool Solver::startSolve(const QString& name, const QString& file)
{
    //检查工作目录是否有效
    if (_workDir.isEmpty() || !QDir(_workDir).exists())
    {
        AppFrame::FITKMessageError("Work directory is empty or does not exist!");
        return false;
    }
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionStartSolver");
    if (oper == nullptr)
    {
        AppFrame::FITKMessageError("Cannot find solver operator!");
        return false;
    }
    Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (!radiossCase) return false;
    Radioss::FITKRadiossSolutionManager* solutionManager = radiossCase->getSolutionManager();
    if (!solutionManager) return false;
    if (file.isEmpty())
    {
        Radioss::FITKRadiossSolution* solution = name.isEmpty() ? solutionManager->getCurrentSolution() : solutionManager->getDataByName(name);
        if (!solution)
        {
            AppFrame::FITKMessageError(QString("Cannot find solution %1!").arg(name));
            return false;
        }
        QString workDir = QString("%1/%2").arg(_workDir).arg(solution->getDataObjectName());
        oper->setArgs("WorkDir", workDir);
        oper->setArgs("CaseName", solution->getDataObjectName());
        oper->setArgs("isImport", false);
        oper->setArgs("StarterFileName", QString("%1/%2_0000.rad").arg(workDir).arg(solution->getDataObjectName()));
        oper->setArgs("EngineFileName", QString("%1/%2_0001.rad").arg(workDir).arg(solution->getDataObjectName()));
    }
    else
    {
        QString fileName = QString(QFileInfo(file).baseName()).replace("_0000", "").replace("_0001", "");
        QString workDir = QString("%1/%2").arg(_workDir).arg(fileName);
        oper->setArgs("WorkDir", workDir);
        oper->setArgs("CaseName", fileName);
        oper->setArgs("isImport", true);
        oper->setArgs("StarterFileName", QString("%1/%2_0000.rad").arg(QFileInfo(file).absolutePath()).arg(fileName));
        oper->setArgs("EngineFileName", QString("%1/%2_0001.rad").arg(QFileInfo(file).absolutePath()).arg(fileName));
    }

    oper->setArgs("ThreadNum", _threadNum);

    //执行求解
    bool success = oper->execProfession();
    if (!success)
    {
        AppFrame::FITKMessageError("Start solver failed!");
        return false;
    }

    return true;
}

//停止求解
bool Solver::stopSolve()
{

    return true;
}

//获取求解状态
int Solver::getSolveStatus()
{
    //获取操作器
    Core::FITKActionOperator* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionStartSolver");
    if (oper == nullptr)
    {
        AppFrame::FITKMessageError("Cannot find solver operator!");
        return false;
    }
    int status = 0;
    oper->argValue<int>("SolveStatus", status);
    return status;
}

//创建Solver实例
Solver* SolverWapper::new_Solver()
{
    //创建Solver实例
    return new Solver();
}

//释放Solver实例
void SolverWapper::delete_Solver(Solver* obj)
{
    //释放Solver实例
    if (obj) delete obj;
}

Solver* SolverWapper::static_Solver_runSolveConfigFile(const QString& file, int threadNum)
{
    //创建Solver实例并设置线程数
    Solver* solver = new Solver();
    solver->_threadNum = threadNum;
    //开始求解
    solver->startSolve("", file);
    return solver;
}

Solver* SolverWapper::static_Solver_runSolveCase(const QString& name_solution, int threadNum)
{
    //创建Solver实例并设置线程数
    Solver* solver = new Solver();
    solver->_threadNum = threadNum;
    //开始求解
    solver->startSolve(name_solution);
    return solver;
}

QString SolverWapper::startRun(Solver* obj, const QString& name)
{
    //检查Solver对象是否为空
    if (!obj)
    {
        AppFrame::FITKMessageError("Solver object is null!");
        return sendMessageToServer(tr("Failed, object is null"));
    }
    //调用Solver的startSolve方法开始求解
    if (!obj->startSolve(name))
    {
        return name.isEmpty() ? sendMessageToServer(tr("Failed, unable to start current solution"))
            : sendMessageToServer(tr("Failed, unable to start solution %1").arg(name));
    }
    return name.isEmpty() ? sendMessageToServer(tr("Success, current solution started"))
        : sendMessageToServer(tr("Success, started solution %1").arg(name));
}

QString SolverWapper::startRunConfigFile(Solver* obj, const QString& file)
{
    //检查Solver对象是否为空
    if (!obj)
    {
        AppFrame::FITKMessageError("Solver object is null!");
        return sendMessageToServer(tr("Failed, object is null"));
    }
    //调用Solver的startSolve方法开始求解
    if (!obj->startSolve("", file))
    {
        return sendMessageToServer(tr("Failed, unable to start solver with config file %1").arg(file));
    }
    return sendMessageToServer(tr("Success, started solver with config file %1").arg(file));
}

QString SolverWapper::setThreadNum(Solver* obj, int threadNum)
{
    if (!obj)
    {
        AppFrame::FITKMessageError("Solver object is null!");
        return sendMessageToServer(tr("Failed, object is null"));
    }
    if (threadNum <= 0)
    {
        return sendMessageToServer(tr("Failed, thread number must be greater than 0"));
    }
    //设置线程数
    obj->_threadNum = threadNum;
    return sendMessageToServer(tr("Success, thread number set to %1").arg(threadNum));
}

QString SolverWapper::setSolverWorkDir(Solver* obj, const QString& workDir)
{
    //检查Solver对象是否为空
    if (!obj)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }
    //检查工作目录是否存在
    if (QDir(workDir).exists())
    {
        //设置工作目录
        obj->_workDir = workDir;
    }
    else
    {
        //工作目录不存在，输出错误信息
        AppFrame::FITKMessageError("Work directory does not exist!");
        return sendMessageToServer(tr("Failed, work directory %1 does not exist").arg(workDir));
    }
    return sendMessageToServer(tr("Success, work directory set to %1").arg(workDir));
}

//停止求解
void SolverWapper::stopSolve(Solver* obj)
{
    //停止求解
    if (obj == nullptr || !obj->stopSolve())
    {
        AppFrame::FITKMessageError("Stop solver failed!");
    }
}

//获取求解状态
int SolverWapper::getSolveStatus(Solver* obj)
{
    //获取求解状态
    if (obj == nullptr)
    {
        return 0;
    }

    return obj->getSolveStatus();
}

//更新图形和树形事件
void SolverWapper::updateGraphAndTreeEvent(int dataObjId)
{
    //更新树形菜单事件
    QStringList listOpertor = { "TreeSolutionEvent" };
    for (int i = 0; i < listOpertor.size(); ++i)
    {
        EventOper::TreeEventOperator* oper =
            dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr)
        {
            oper->updateTree();
        }
    }

    //更新图形显示
    EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph)
    {
        operGraph->updateGraph(dataObjId, true);
    }
}