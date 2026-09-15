/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKJob.h
 * @brief 作业数据声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-04-01
 * 
 */
#ifndef _FITKJob_H____
#define _FITKJob_H____
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKAbaqusDataAPI.h"

namespace AbaqusData
{
    class DataCase;
   
    /**
     * @brief 作业数据声明
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-04-01
     */
    class FITKAbaqusDataAPI FITKJob : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
        //FITKSafeDownCastA(FITKJob, Core::FITKAbstractNDataObject);
    public:
        /**
        * @brief 作业运行状态
        * @author BaGuijun (baguijun@163.com)
        * @date 2024-04-01
        */
        enum RunningState
        {
            NotStarted,  ///< 未开始
            Running,     ///< 运行中
            Pause,       ///< 暂停
            Finished,    ///< 完成
            Termination,  ///< 终止
        };
        Q_ENUM(RunningState);
        /**
         * @brief 多CPU方法
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        enum MutiCPUMode
        {
            Default,  ///< 默认
            Thread,   ///< 线程模式
            MPI,       ///< MPI模式
        };
        Q_ENUM(MutiCPUMode);
        /**
         * @brief 并行方法
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        enum ParallelMethod
        {
            Field,  ///< 作用域
            Cycle,   ///< 循环
        };
        Q_ENUM(ParallelMethod);
        /**
         * @brief 求解类型
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        enum SolverType
        {
            STUnKonw,    ///< 错误类型
            STExplicit,  ///< 显式
            STImplicit,  ///< 隐式
        }; 
        Q_ENUM(SolverType);
    


    public:
        /**
         * @brief 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        FITKJob(/*bool bTemp = false*/);
        /**
         * @brief Destroy the FITKJob object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        ~FITKJob() override;
        /**
         * @brief 拷贝
         * @param[i]  obj            作业对象
         * @return true 
         * @return false 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief 获取分析步描述
         * @return 分析步描述 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        const QString getDesctiption() const;
        /**
         * @brief 设置分析步描述
         * @param[i]  str            分析步描述
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setDesctiption(const QString str);
        /**
         * @brief 设置是否使用多CPU
         * @param[i]  b              是否使用CPU
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setIsMutiCPU(bool b);
        /**
         * @brief 获取是否使用多CPU进行计算
         * @return true 是
         * @return false 否
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        bool isMutiCPU() const;
        /**
         * @brief 设置多CPU的计算模式
         * @param[i]  mode           CPU计算模式
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setMutiCPUMode(MutiCPUMode mode);
        /**
         * @brief 获取多CPU计算模式
         * @return MutiCPUMode 计算模式
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        MutiCPUMode getMutiCPUMode() const;
        /**
         * @brief 设置要使用的CPU核数
         * @param[i]  n              CPU核数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setNCPU(int n);
        /**
         * @brief 获取CPU核数
         * @return int CPU核数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        int getNCPU() const;
        /**
         * @brief 设置是否使用CPU加速
         * @param[i]  b              是否使用GPU加速
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setIsGPU(bool b);
        /**
         * @brief 获取GPU加速状态
         * @return true 开启GPU加速
         * @return false 关闭GPU加速
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        bool isGPU() const;
        /**
         * @brief 设置GPU的核数
         * @param[i]  n              GPU核数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setNGPU(int n);
        /**
         * @brief 获取GPU核数
         * @return int GPU核数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        int getNGPU() const;
        /**
         * @brief 设置并行域个数
         * @param[i]  n              并行域个数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setNumberOfFields(int n);
        /**
         * @brief 获取并行域个数
         * @return int 并行域个数
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        int getNumberOfFields() const;
        /**
         * @brief 设置并行方法
         * @param[i]  method         并行方法
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setParallelMethod(ParallelMethod method);
        /**
         * @brief 获取并行方法
         * @return ParallelMethod 并行方法
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        ParallelMethod getParallelMethod() const;
        /**
         * @brief 设置运行状态
         * @param[i]  state          运行状态
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setRunningState(RunningState state);
        /**
         * @brief 获取运行状态
         * @return RunningState 运行状态 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        RunningState getRunningState() const;
        /**
         * @brief 获取求解器类型
         * @return SolverType 求解器类型
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        SolverType getSolverType();
        /**
         * @brief 设置inp文件路径
         * @param[i]  str            inp文件路径
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setFilePath(QString str);
        /**
         * @brief 为inp作业设置inp文件路径
         * @param[i]  str            inp文件路径
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setFilePathForInpJob(QString str);
        /**
         * @brief 获取inp文件路径
         * @return const QString inp文件路径 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        const QString& getFilePath() const;
        /**
         * @brief 获取当前作业的算例ID
         * @return int 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-02
         */
        int getCaseID();
        /**
         * @brief 设置当前作业的算例的ID
         * @param[i]  caseId         算例ID
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-02
         */
        void setCaseID(const int caseId);
        /**
        * @brief 判断是否为inp的作业
        * @return true,inp的作业;false,模型的作业。
        * @param
        * @author xiewenxi (xwx14@qq.com)
        * @date 2023-09-07
        */
        bool isInpJob() const;
        /**
         * @brief 设置求解器类型
         * @param[i]  type           求解器类型
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        void setSolveType(SolverType type);
    private:
        /**
         * @brief 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        QString        _description{};
        /**
         * @brief 是否使用CPu
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        bool           _bMutiCPU = false;
        /**
         * @brief CPU方法
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        MutiCPUMode    _mutiCPUMode = Default;
        /**
         * @brief CPU数量
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        int            _nCPU = 1;
        /**
         * @brief 是否使用GPU
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        bool           _bGPU = false;
        /**
         * @brief GPU方法
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        int            _nGPU = 1;
        /**
         * @brief 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        int            _nFields = 1;
        /**
         * @brief 并行方法
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        ParallelMethod _parallelMethod = Field;
        /**
         * @brief 运行状态
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        RunningState   _runState = NotStarted;
        /**
         * @brief 文件路径
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        QString        _filePath{};
        /**
         * @brief 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        //static int     MAXPROID;
        /**
         * @brief 求解类型
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        SolverType     _solveType = STExplicit;
        /**
         * @brief 当前作业的算例ID
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-02
         */
        int _caseID = -1;
    };
}  // namespace Document
#endif
