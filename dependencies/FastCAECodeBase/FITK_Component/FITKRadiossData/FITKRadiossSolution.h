/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_RAD__SOLUTION_H__
#define _FITK_RAD__SOLUTION_H__

#include "FITKRadiossDataAPI.h"
#include "FITKRadiossCaseManager.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{

    class FITKRadiossBCSManager;
    class FITKInitialFieldManager;
    class FITKRadiossRWallManager;
    class FITKGravityManager;
    class FITKInteractionManager;
    class FITKRadiossInteractionDefaultManager;
    class FITKRadiossSolverSettings;
    class FITKRadiossResultRequest;
    class FITKProbeManager;
    class FITKTimeHistoryManager;
    class FITKConnectionManager;

    /**
     * @brief Radioss求解类
     * @details 此类用于执行Radioss求解
     * @author libaojun
     * @date 2025-04-25
     */
    class FITKRadiossDataAPI FITKRadiossSolution : public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Radioss, FITKRadiossSolution);
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         * @date 2025-04-25
         */
        explicit FITKRadiossSolution();

        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2025-04-25
         */
        virtual ~FITKRadiossSolution();
        /**
         * @brief 获取边界条件管理器
         * @return FITKRadiossBCSManager* 边界条件管理器指针
         * @author libaojun
         * @date 2025-06-05
         */
        FITKRadiossBCSManager* getBCSManager() const;
        
        
        /**
         * @brief 获取初始场管理器
         * @return FITKInitialFieldManager* 初始场管理器指针
         * @author libaojun
         * @date 2025-09-02
         */
        FITKInitialFieldManager* getInitialFieldManager() const;    

        /**
         * @brief  获取默认相互作用管理器
         * @return FITKRadiossInteractionDefaultManager* 默认管理器指针
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-25
         */
        FITKRadiossInteractionDefaultManager* getDefaultInteractionManager() const;

        /**
         * @brief 获取刚性墙管理器
         * @return FITKRadiossRWallManager* 刚性墙管理器指针
         * @author libaojun
         * @date 2025-08-25
         */
        FITKRadiossRWallManager* getRWallManager() const;
        /**
         * @brief 获取重力场管理器
         * @return FITKGravityManager* 重力场管理器指针
         * @author libaojun
         * @date 2025-08-25
         */
        FITKGravityManager* getGravityManager() const;
        /**
         * @brief 获取相互作用管理器
         * @return FITKInteractionManager* 相互作用管理器指针
         * @author libaojun
         * @date 2025-09-02
         */
        FITKInteractionManager* getInteractionManager() const;
        /**
         * @brief    获取探针管理器
         * @return   FITKProbeManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        FITKProbeManager* getProbeManager() const;
        /**
         * @brief    获取求解设置数据
         * @return   FITKRadiossSolverSettings *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverSettings* getSolverSettings() const;
        /**
         * @brief    获取结果请求数据
         * @return   FITKRadiossResultRequest *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        FITKRadiossResultRequest* getResultRequest() const;
        /**
         * @brief    获取时间历史历程管理器
         * @return   FITKTimeHistoryManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        FITKTimeHistoryManager* getTimeHietoryManager() const;
        /**
         * @brief    连接管理器
         * @return   FITKConnectionManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        FITKConnectionManager* getConnectionManager() const;

    private:
        FITKRadiossBCSManager* _bcsManager{nullptr}; //!< 边界条件管理器
        FITKInitialFieldManager* _initialFieldManager{nullptr}; //!< 初始场管理器
        FITKRadiossInteractionDefaultManager* _defaultInteractionManager{ nullptr }; //!< 默认相互作用管理器
        FITKRadiossRWallManager* _rWallManager{nullptr}; //!< 刚性壁管理器
        FITKGravityManager* _gravityManager{nullptr}; //!< 重力场管理器
        FITKInteractionManager* _interactionManager{nullptr}; //!< 相互作用管理器
        FITKProbeManager* _probeManager{ nullptr }; //!< 探针管理器
        FITKConnectionManager* _connectionManager{ nullptr }; //!< 连接管理器
        FITKRadiossSolverSettings* _solverSettings{ nullptr }; //!< 求解器参数
        FITKRadiossResultRequest* _resultRequest{ nullptr }; //!< 结果数据请求
        FITKTimeHistoryManager* _timeHietoryManager{ nullptr }; //!< 结果数据请求-时间历史历程管理器

    };

    class FITKRadiossDataAPI FITKRadiossSolutionManager : 
        public Core::FITKAbstractDataManager<FITKRadiossSolution>
    {
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         * @date 2025-04-25
         */
        explicit FITKRadiossSolutionManager() = default;
        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2025-04-25
         */
        virtual ~FITKRadiossSolutionManager() = default;

        /**
         * @brief 获取当前求解对象
         * @return FITKRadiossSolution* 当前求解对象
         * @author libaojun
         * @date 2025-04-25
         */
        FITKRadiossSolution* getCurrentSolution();
        /**
         * @brief 设置当前求解对象
         * @param[i]  id             求解对象ID
         * @author libaojun
         * @date 2025-04-25
         */
        void setCurrentSolution(int id);
        /**
         * @brief 获取当前求解对象ID
         * @return int 当前求解对象ID
         * @author libaojun
         * @date 2025-04-25
         */
        int getCurrentSolutionID() const;


    private:
        // 当前求解对象ID
        int _currentSolutionID{-1};
    };

} // namespace Radioss

#endif // _FITK_RAD__SOLUTION_H__