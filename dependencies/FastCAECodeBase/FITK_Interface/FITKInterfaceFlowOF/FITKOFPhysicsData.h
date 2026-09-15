/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITKOF_PHYSICS_DATA_H___
#define _FITKOF_PHYSICS_DATA_H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include <QList>

namespace Interface
{
    class FITKAbstractOFSolver;
    class FITKAbstractOFTurbulence;
    class FITKOFRadiation;
    class FITKOFThermo;
    class FITKOFTransportProp;
    class FITKOFDiscretization;
    class FITKOFSolution;
    class FITKOFBoundaryManager;
    class FITKOFInitialConditions;
    class FITKOFRunControl;
    class FITKOFOperatingConditions;

    class FITKInterfaceFlowOFAPI FITKOFPhysicsData : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKOFPhysicsData();
        virtual ~FITKOFPhysicsData();
        /**
         * @brief    获取求解器
         * @return   FITKAbstractOFSolver *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKAbstractOFSolver* getSolver();
        /**
         * @brief    获取区域网格类型
         * @param[i] regionMeshID 
         * @return   FITKOFSolverTypeEnum::FITKOFRegionMeshType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        FITKOFSolverTypeEnum::FITKOFRegionMeshType getRegionMeshType(int regionMeshID);
        /**
         * @brief    是否启用湍流
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        bool isEnableTurbulenceEqu();
        /**
         * @brief    获取湍流
         * @return   FITKAbstractOFTurbulence *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKAbstractOFTurbulence* getTurbulence();
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
         * @brief    获取物理材料数据
         * @return   FITKOFTransportProp *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKOFTransportProp* getTransportProp();
        /**
         * @brief    获取离散数据
         * @return   FITKOFDiscretization *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        FITKOFDiscretization* getDiscretization();
        /**
         * @brief    获取solution数据
         * @return   FITKOFSolution *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        FITKOFSolution* getSolution();
        /**
         * @brief    获取InitialConditions数据
         * @return   FITKOFInitialConditions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKOFInitialConditions* getInitialConditions();
        /**
         * @brief    获取边界管理器
         * @return   FITKOFBoundaryManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        FITKOFBoundaryManager* getBoundaryManager();
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
        /**
         * @brief    设置求解器
         * @param[i] solver 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void setSolver(FITKAbstractOFSolver* solver);
        /**
         * @brief    设置区域网格类型
         * @param[i] regionMeshID 
         * @param[i] regionMeshType 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        void setRegionMeshType(int regionMeshID, FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType);
        /**
         * @brief    设置是否启用湍流
         * @param[i] isEnable
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void setEnableTurbulenceEqu(bool isEnable);
        /**
         * @brief    设置湍流
         * @param[i] turbulence 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void setTurbulence(FITKAbstractOFTurbulence* turbulence);

        /**
         * @brief    获取辐射模型参量
         * @return   FITKOFRadiation *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void setRadiation(FITKOFRadiation* radiation);
        /**
         * @brief    获取定义材料的热物理属性
         * @return   FITKOFThermo *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void setThermo(FITKOFThermo* thermo);
        /**
         * @brief    设置物理材料数据
         * @param[i] transProp 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        void setTransportProp(FITKOFTransportProp* transProp);
        /**
         * @brief    设置离散
         * @param[i] discretization 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        void setDiscretization(FITKOFDiscretization* discretization);
        /**
         * @brief    设置求解方式
         * @param[i] solution 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        void setSolution(FITKOFSolution* solution);
        /**
         * @brief    设置InitialCondition数据
         * @param[i] initial 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        void setInitialConditions(FITKOFInitialConditions* initial);
        /**
         * @brief    设置boundary manager
         * @param[i] manager 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-26
         */
        void setBoundaryManager(FITKOFBoundaryManager* manager);
        /**
         * @brief    设置运行配置
         * @param[i] run 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-31
         */
        void setRunControl(FITKOFRunControl* run);
        /**
         * @brief    设置求解工作条件
         * @param[i] operatingCond 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        void setOperatingConditions(FITKOFOperatingConditions* operatingCond);
        /**
         * @brief    获取全部的区域网格类型
         * @return   QHash<int,FITKOFSolverTypeEnum::FITKOFRegionMeshType>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        QHash<int, FITKOFSolverTypeEnum::FITKOFRegionMeshType> getAllRegionMeshType();
        /**
         * @brief    清理数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-13
         */
        void clearData();

    private:
        /**
         * @brief  区域网格类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-23
         */
        QHash<int, FITKOFSolverTypeEnum::FITKOFRegionMeshType> _regionType{};
        /**
         * @brief  是否启用湍流数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        bool _enableTurb{ false };
        /**
         * @brief  求解器数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        FITKAbstractOFSolver* _solver{};
        /**
         * @brief  湍流数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        FITKAbstractOFTurbulence* _turbulence{};
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
        FITKOFTransportProp* _transPortProp{};
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
        FITKOFBoundaryManager* _boundaryManager{};
        /**
         * @brief  InitialConditions数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-23
         */
        FITKOFInitialConditions* _initialCond{};
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
