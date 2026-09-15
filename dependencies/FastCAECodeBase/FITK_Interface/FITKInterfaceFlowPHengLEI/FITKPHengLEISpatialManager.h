/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEISpatialManager.h
 * @brief  空间离散类型管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK_PHengLEISpatialManager_H___
#define _FITK_PHengLEISpatialManager_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITKPHengLEIAbsSolver.h"
#include <functional>
#include <QStringList>

namespace Interface
{
    class FITKPHengLEIAbsSpatialDiscretisation;
}

typedef std::function<Interface::FITKPHengLEIAbsSpatialDiscretisation*(Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type)>  CREATESpatialFUN;

namespace Interface
{
    /**
     * @brief  空间离散类型参数信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKPHengLEISpatialInfo
    {
        QString _spatialTypeName{};
        QList<FITKPHengLEIAbsSolver::PHengLEISolverType> _enableSolverType;
		CREATESpatialFUN _createFuns{};
    };

    /**
     * @brief  空间离散类型管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEISpatialManager
    {
    public:
        explicit FITKPHengLEISpatialManager();
        virtual ~FITKPHengLEISpatialManager();
        /**
         * @brief    获取空间离散类型链表
         * @param[i] tbType 湍流模型类型
         * @param[i] stype 求解器类型
         * @return   QList<QString>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        QStringList filterSpatial();
        /**
         * @brief    获取空间离散类型参数信息
         * @param[i] turbulenceName 湍流模型名称
         * @return   FITKOFTurbulenceInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
		FITKPHengLEISpatialInfo getSpatialTypeInfoByName(QString spatialTypeName);
    private:
        /**
         * @brief    初始化空间离散类型参数信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initspatialTypes();

    private:
        /**
         * @brief  空间离散参数信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        QList<FITKPHengLEISpatialInfo> _spatialTypes{};
    };
 
}


#endif
