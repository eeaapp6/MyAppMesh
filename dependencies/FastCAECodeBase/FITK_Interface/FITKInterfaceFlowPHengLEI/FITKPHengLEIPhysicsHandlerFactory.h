/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIPhysicsHandlerFactory.h
 * @brief  物理场数据搬运工厂
 * @author PHengLEI
 * @date   2024-08-21
 *********************************************************************/
#ifndef FITKPHENGLEIPHYSICSHANDLERFACTORY_H_
#define FITKPHENGLEIPHYSICSHANDLERFACTORY_H_

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITKPHengLEIBoundary.h"

namespace Interface
{
    class FITKPHengLEIAbsSolver;
	class FITKPHengLEIAbstractInFlowType;
    /**
     * @brief  物理场数据搬运工厂
     * @author PHengLEI
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPhysicsHandlerFactory : public AppFrame::FITKComponentInterface
    {
    public:
        explicit FITKPhysicsHandlerFactory() = default;
        virtual ~FITKPhysicsHandlerFactory() = default;

        /**
        * @brief 获取部件名称，不能重复  return "FITKPHengLEIPhysicsHandlerFactory"
        * @return QString
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-04
        */
        virtual QString getComponentName() override;
        /**
         * @brief    设置求解器
         * @param[i] solverName 求解器名称
         * @return   void
         * @author   PHengLEI
         * @date     2024-08-21
         */
        void setSolver(QString solverName);
		/**
		 * @brief 设置来流类型
		 * @params InFlowType
		 * @params solverName
		 * @author 
		 * @date 2024-10-05
		 */
		void setInflowType(QString InFlowType,QString solverName);

		void setViscousType(QString ViscousType);
		/**
		 * @brief 设置空间离散类型
		 * @params SpatialType
		 * @params sovlerName
		 * @author 
		 * @date 2024-10-05
		 */
		void setPHengLEIDiscretisationType(QString SpatialType, QString sovlerName);
		/**
		 * @brief    设置边界
		 * @param[i] meshBoundaryId 网格边界Id
		 * @param[i] bType 边界类型
		 * @return   void
		 * @author   liuzhonghua (liuzhonghuaszch@163.com)
		 * @date     2024-08-26
		 */
		void setBoundary(int meshBoundaryId, FITKPHengLEIBoundary::PHengLEIBoundaryType bType);
		/**
		 * @brief    设置边界类型
		 * @param[i] boundaryIndex
		 * @param[i] bType
		 * @return   void
		 * @author   liuzhonghua (liuzhonghuaszch@163.com)
		 * @date     2024-08-26
		 */
		void setBoundaryType(int meshID,QString typeName, FITKPHengLEIBoundary::PHengLEIBoundaryType bType);
    protected:
        /**
         * @brief    设置求解器动作 - 初始化物理场数据
         * @param[i] solver 
         * @return   void
         * @author   PHengLEI
         * @date     2024-08-26
         */
        void actionSetSolver(FITKPHengLEIAbsSolver* solver);

		/**
		 * @brief 设置通用控制参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-13
		 */
		void setGeneralControl();
		/**
		 * @brief 设置来流参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-14
		 */
		/**
		 * @brief 设置网格相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setMeshPara();
		/**
		 * @brief 设置粘性模型参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setViscousModel();
		/**
		 * @brief 设置多重网格参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setMultiGrid();
		/**
		 * @brief 设置来流参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setInFlow();
		/**
		 * @brief 设置离散参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setDiscretisation();
		/**
		 * @brief 设置计算输出参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setOutputSetting();
		/**
		 * @brief   设置来流
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-04
		 */
		void setFlow();
		/**
		 * @brief   设置计算设置参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-04
		 */
		void setCalculation();
		/**
		 * @brief   设置Solution参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-04
		 */
		void setSolution();
		/**
		 * @brief   设置初始条件参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-04
		 */
		void setInitialCondition();
		/**
		 * @brief   设置模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-04
		 */
		void setModels();
    };
}

#endif // !FITKPhysicsHandlerFactory_H_
