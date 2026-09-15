/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   SolverWapper.h
 * @brief  求解器相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 *********************************************************************/
#ifndef _SOLVER_WAPPER_H___
#define _SOLVER_WAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceGlobalAPI.h"

namespace Radioss {
    class FITKRadiossSolution;
}

/**
 * @brief  求解器接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 */
class Solver
{
public:
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    explicit Solver();
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    virtual ~Solver() = default;

    /**
     * @brief    开始求解
     * @param[i] name 求解名称（可选）
     * @param[i] file 文件路径（可选）
     * @return   是否开始求解成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    bool startSolve(const QString& name = "", const QString& file = "");

    /**
     * @brief    停止求解
     * @return   是否停止求解成功
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    bool stopSolve();

    /**
     * @brief    获取当前求解状态
     * @return   求解状态 (0: 空闲, 1: 求解中, 2: 暂停)
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    int getSolveStatus();

public:
    /**
     * @brief    工作目录
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString _workDir{};
    /**
     * @brief    线程数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    int _threadNum{ 1 };
};

/**
 * @brief  求解器相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 */
class PythonInterfaceGlobalAPI SolverWapper : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief    创建 Solver 实例
     * @return   新分配的 Solver 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    Solver* new_Solver();

    /**
     * @brief    释放 Solver 实例
     * @param[i] obj 需要释放的 Solver 指针
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    void delete_Solver(Solver* obj);

    /**
     * @brief    静态方法开始求解已存在的配置文件
     * @param[i] file 文件路径（可选）
     * @param[i] threadNum 线程数
     * @return   Solver 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    Solver* static_Solver_runSolveConfigFile(const QString& file, int threadNum = 1);

    /**
     * @brief    静态方法开始求解已存在的求解对象
     * @param[i] name_solution 求解对象名称（可选）
     * @param[i] threadNum 线程数
     * @return   Solver 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    Solver* static_Solver_runSolveCase(const QString& name_solution = "", int threadNum = 1);

    /**
     * @brief    开始求解
     * @param[i] obj Solver 对象指针
     * @param[i] name 求解名称（可选）
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString startRun(Solver* obj, const QString& name= "");

    /**
     * @brief    开始求解已存在的配置文件
     * @param[i] obj Solver 对象指针
     * @param[i] file 文件路径（可选）
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString startRunConfigFile(Solver* obj, const QString& file);

    /**
     * @brief    设置线程数
     * @param[i] obj Solver 对象指针
     * @param[i] threadNum 线程数
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setThreadNum(Solver* obj, int threadNum);

    /**
     * @brief    设置求解器工作目录
     * @param[i] obj Solver 对象指针
     * @param[i] workDir 工作目录
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setSolverWorkDir(Solver* obj, const QString& workDir);

    /**
     * @brief    停止求解
     * @param[i] obj Solver 对象指针
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    void stopSolve(Solver* obj);

    /**
     * @brief    获取求解状态
     * @param[i] obj Solver 对象指针
     * @return   求解状态 (0: 空闲, 1: 求解中, 2: 暂停)
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    int getSolveStatus(Solver* obj);

private:
    /**
     * @brief    更新图形和树形事件
     * @param[i] dataObjId 数据对象ID
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    void updateGraphAndTreeEvent(int dataObjId);
};

#endif // _SOLVER_WAPPER_H___