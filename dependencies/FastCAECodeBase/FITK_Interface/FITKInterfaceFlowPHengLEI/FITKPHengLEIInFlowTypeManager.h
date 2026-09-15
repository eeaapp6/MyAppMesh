/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIInFlowTypeManager.h
 * @brief  来流类型管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK_PHengLEIInFlowTypeManager_H___
#define _FITK_PHengLEIInFlowTypeManager_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITKPHengLEIAbsSolver.h"
#include <functional>
#include <QStringList>

namespace Interface
{
    class FITKPHengLEIAbstractInFlowType;
}

typedef std::function<Interface::FITKPHengLEIAbstractInFlowType*(Interface::FITKPHengLEIAbsSolver::PHengLEISolverType type)>  CREATEInFlowTypeFUN;

namespace Interface
{
    /**
     * @brief  来流类型参数信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKPHengLEIInFlowTypeInfo
    {
        QString _InFlowTypeName{};
		QList<FITKPHengLEIAbsSolver::PHengLEISolverType> _enableType;
		CREATEInFlowTypeFUN _createFuns{};
    };

    /**
     * @brief  来流类型管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIInFlowTypeManager
    {
    public:
        explicit FITKPHengLEIInFlowTypeManager();
        virtual ~FITKPHengLEIInFlowTypeManager();

        /**
         * @brief    过滤求解器类型获取来流类型链表
         * @param[i] type 求解器类型
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        QStringList filterTurbence(FITKPHengLEIAbsSolver::PHengLEISolverType type);
        /**
         * @brief    获取来流类型信息
         * @param[i] turbulenceName 来流类型名称
         * @return   FITKPHengLEIInFlowTypeInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
		FITKPHengLEIInFlowTypeInfo getInFlowTypeInfoByName(QString inFLowTypeName);
    private:
        /**
         * @brief    初始化来流类型参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initInFlowTypes();
    private:
        /**
         * @brief  来流类型链表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        QList<FITKPHengLEIInFlowTypeInfo> _inFlowTypes{};
    };
 
}


#endif
