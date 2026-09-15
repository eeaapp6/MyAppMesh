/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIBoundaryTypeManager.h
 * @brief  边界条件类型管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK_PHengLEIBoundaryTypeManager_H___
#define _FITK_PHengLEIBoundaryTypeManager_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITKPHengLEIAbsSolver.h"
#include <functional>
#include <QStringList>

namespace Interface
{
    class FITKPHengLEIBoundaryType;
}

typedef std::function<Interface::FITKPHengLEIBoundaryType*()>  CREATEBoundaryTypeFUN;

namespace Interface
{
    /**
     * @brief  边界条件类型信息结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKPHengLEIBoundaryTypeInfo
    {
        QString _BoundaryTypeTypeName{};
		FITKPHengLEIBoundary::PHengLEIBoundaryType bType{ };
		CREATEBoundaryTypeFUN _createFuns{};
    };

    /**
     * @brief  边界条件类型管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIBoundaryTypeManager
    {
    public:
        explicit FITKPHengLEIBoundaryTypeManager();
        virtual ~FITKPHengLEIBoundaryTypeManager();
		/**
		 * @brief 根据边界类型筛选出对应边界推荐配置类型
		 * @params bType
		 * @return  QStringList
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-04
		 */
		QStringList filterBoundary(FITKPHengLEIBoundary::PHengLEIBoundaryType bType);
		/**
		 * @brief 根据边界条件类型对应的推荐配置参数名称和边界条件枚举值类型获取边界条件类型信息结构体
		 * @params boundaryTypeName
		 * @params bType
		 * @return  FITKPHengLEIBoundaryTypeInfo
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIBoundaryTypeInfo getBoundaryType(QString boundaryTypeName,FITKPHengLEIBoundary::PHengLEIBoundaryType bType);
    private:
        /**
         * @brief    初始化边界条件类型全部的推荐配置参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initBoundaryTypes();
    private:
        /**
         * @brief  边界条件类型链表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        QList<FITKPHengLEIBoundaryTypeInfo*> _boundaryTypes{};
    };
 
}


#endif
