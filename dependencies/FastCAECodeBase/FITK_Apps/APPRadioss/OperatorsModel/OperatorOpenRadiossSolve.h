/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorOpenRadiossSolve.h
 * @brief  OpenRadioss求解操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-19
 *********************************************************************/
#ifndef _OPERATOROPENRADIOSSSOLVE_H
#define _OPERATOROPENRADIOSSSOLVE_H

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKExecProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"

namespace ModelOper
{
    /**
     * @brief  OpenRadioss 求解操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class OperatorOpenRadiossSolve : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit OperatorOpenRadiossSolve() = default;
        virtual ~OperatorOpenRadiossSolve() = default;
        /**
         * @brief    界面逻辑，生成UI交互
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-18
         */
        virtual bool execGUI() override;
        /**
         * @brief    业务处理逻辑，在execGUI后执行
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-18
         */
        virtual bool execProfession() override;

    private slots:
        /**
         * @brief  求解完成槽
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-19
         */
        void solverFinishedSlot();
        /**
         * @brief    调用stater求解完成开始调用engine求解
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void starterFinished_EngineSolverSlot();

    private:
        /**
         * @brief    写出求解配置文件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-19
         */
        bool writeSolverConfigFile();
        /**
         * @brief    开始求解
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-19
         */
        void startSolver();
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionStartSolver, OperatorOpenRadiossSolve);

    /**
     * @brief  OpenRadioss starter可执行程序驱动
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class FITKOpenRadiossStarterExecProgramDriver : public AppFrame::FITKExecProgramDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKOpenRadiossStarterExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKOpenRadiossStarterExecProgramDriver();
        /**
         * @brief Destroy the FITKOpenRadiossStarterExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKOpenRadiossStarterExecProgramDriver();
        /**
         * @brief 获取程序类型，抽象值
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual int getProgramType() override;
        /**
         * @brief 获取程序名称，任务管理器的名称，不是文件路径
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QString getProgramName() override;
        /**
         * @brief    设置OpenMP 线程数
         * @param[i] num N
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void setOpenMPThreadNum(QString num);
        /**
         * @brief    是否存在可执行程序
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        bool isExistExeProgram();
    };
    //驱动器注册相关操作
    Register2FITKProgramDriverFactory(3, FITKOpenRadiossStarterExecProgramDriver, FITKOpenRadiossStarterExecProgramDriver);

    /**
     * @brief  OpenRadioss engine可执行程序驱动
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class FITKOpenRadiossEngineExecProgramDriver : public AppFrame::FITKExecProgramDriver
    {
    public:
        /**
         * @brief Construct a new FITKOpenRadiossEngineExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKOpenRadiossEngineExecProgramDriver();
        /**
         * @brief Destroy the FITKOpenRadiossEngineExecProgramDriver object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKOpenRadiossEngineExecProgramDriver();
        /**
         * @brief 获取程序类型，抽象值
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual int getProgramType() override;
        /**
         * @brief 获取程序名称，任务管理器的名称，不是文件路径
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QString getProgramName() override;
        /**
         * @brief    设置OpenMP 线程数
         * @param[i] num N
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void setOpenMPThreadNum(QString num);
        /**
         * @brief    是否存在可执行程序
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        bool isExistExeProgram();
    };
    //驱动器注册相关操作
    Register2FITKProgramDriverFactory(3, FITKOpenRadiossEngineExecProgramDriver, FITKOpenRadiossEngineExecProgramDriver);

    /**
     * @brief  OpenRadioss可执行程序驱动输入信息
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-18
     */
    class FITKOpenRadiossExecProgramInputInfo : public AppFrame::FITKProgramInputInfo
    {
    public:
        /**
         * @brief Construct a new FITKOpenRadiossExecProgramInputInfo object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKOpenRadiossExecProgramInputInfo();
        /**
         * @brief Destroy the FITKOpenRadiossExecProgramInputInfo object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKOpenRadiossExecProgramInputInfo();
        /**
         * @brief    设置求解配置路径
         * @param[i] solverConfigFile
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        void setSolverConfigFile(QString solverConfigFile);
        /**
         * @brief    设置线程数
         * @param[i] num 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-20
         */
        void setThreadNum(int num);
        /**
         * @brief 获取启动参数
         * @return QStringList
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual QStringList args() override;

    private:
        /**
         * @brief  求解配置文件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString _solverConfigFile{};
        /**
         * @brief  默认线程数为1
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-20
         */
        int _threadNum{ 1 };
    };
}

#endif