/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractOFSolver.h
 * @brief  求解器基类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK_OF_ABSTRACT_SOLVER__H___
#define _FITK_OF_ABSTRACT_SOLVER__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOFEnum.hpp"

namespace Interface
{
    class FITKOFRadiation;
    class FITKOFThermo;
    class FITKOFTransportProp;
    class FITKOFDiscretization;
    class FITKOFSolution;
    class FITKOFBoundary;
    class FITKOFInitialConditions;
    class FITKOFRunControl;
    class FITKOFOperatingConditions;
    /**
     * @brief  求解器基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKAbstractOFSolver : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKAbstractOFSolver();
        virtual ~FITKAbstractOFSolver();
        /**
         * @brief    获取求解器运行命令
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-06
         */
        virtual QString getSolverCommand() = 0;

        /**
         * @brief    获取求解器类型
         * @return   FITKOFSolverTypeEnum::FITKOFSolverType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual FITKOFSolverTypeEnum::FITKOFSolverType getSolverType() = 0;

        /**
         * @brief    是否启用湍流
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        bool turbEnabled() const;
        /**
         * @brief    获取辐射模型参量
         * @return   FITKOFRadiation *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKOFRadiation* getRadiation();
        /**
         * @brief    获取定义材料的热物理属性
         * @return   FITKOFThermo *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKOFThermo* getThermo();
        /**
         * @brief    获取离散数据
         * @return   FITKOFDiscretization *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFDiscretization* getDiscretization();
        /**
         * @brief    获取材料
         * @return   FITKOFTransportProp *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFTransportProp* getTransportProp();
        /**
         * @brief    获取solution数据
         * @return   FITKOFSolution *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        FITKOFSolution* getSolution();
        /**
         * @brief    获取Initial Conditions数据
         * @return   FITKOFInitialConditions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKOFInitialConditions* getInitialConditions();
        /**
         * @brief    获取边界
         * @param[i] bType 边界类型
         * @return   FITKOFBoundary *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        FITKOFBoundary* getBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType bType);
        /**
         * @brief    获取求解运行配置
         * @return   FITKOFRunControl *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-31
         */
        FITKOFRunControl* getRunControl();
        /**
         * @brief    获取求解工作条件
         * @return   FITKOFOperatingConditions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        FITKOFOperatingConditions* getOperatingConditions();

    protected:
        /**
         * @brief    初始化
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void init();
        /**
         * @brief    初始化辐射参量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        virtual void initRadiation();
        /**
         * @brief    初始化定义材料的热物理属性
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        virtual void initThermo();
        /**
         * @brief    初始化材料
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initTransportProp();
        /**
         * @brief    初始化离散数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        virtual void initDiscretization();
        /**
         * @brief    初始化solution数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initSolution();
        /**
         * @brief    初始化边界数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        virtual void initBoundarys();
        /**
         * @brief    初始化 Initial Conditions数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        virtual void initInitialConditions();
        /**
         * @brief    初始化运行配置数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-31
         */
        virtual void initRunControl();
        /**
         * @brief    初始化工作条件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        virtual void initOperatingConditions();

    protected:
        /**
         * @brief  是否启用湍流数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        bool _enableTurb{false};
        /**
         * @brief  辐射模型参量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        FITKOFRadiation* _radiation{};
        /**
         * @brief  定义材料的热物理属性
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        FITKOFThermo* _thermo{};
        /**
         * @brief  材料数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        FITKOFTransportProp* _transportProp{};
        /**
         * @brief  离散数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        FITKOFDiscretization* _discretization{};
        /**
         * @brief  solution数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        FITKOFSolution* _solution{};
        /**
         * @brief  边界数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-26
         */
        FITKOFInitialConditions* _initialCond{};
        /**
         * @brief  InitialConditions数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-23
         */
        QList<FITKOFBoundary*> _boundaryInfos{};
        /**
         * @brief  运行配置数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-31
         */
        FITKOFRunControl* _runControl{};
        /**
         * @brief  工作条件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-04
         */
        FITKOFOperatingConditions* _operatingCond{};
    };
}


#endif
