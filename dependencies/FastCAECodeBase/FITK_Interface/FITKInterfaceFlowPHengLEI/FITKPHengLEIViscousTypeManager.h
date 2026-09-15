/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIViscousTypeManager.h
 * @brief  来流类型管理器
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK_PHengLEIViscousTypeManager_H___
#define _FITK_PHengLEIViscousTypeManager_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITKPHengLEIAbsSolver.h"
#include <functional>
#include <QStringList>

namespace Interface
{
    class FITKPHengLEIViscousType;
}

typedef std::function<Interface::FITKPHengLEIViscousType*()>  CREATEViscousTypeFUN;

namespace Interface
{
    /**
     * @brief  粘性模型参数信息 结构体
     * @author WangKai (wang_starry@outlook.com)
     * @date   2024-08-21
     */
    struct FITKPHengLEIViscousTypeInfo
    {
        QString _viscousTypeName{};
		QList<FITKPHengLEIAbsSolver::PHengLEISolverType> _enableType;
		QList<FITKPHengLEIAbsSolver::PHengLEIViscousType> _viscousType;
		CREATEViscousTypeFUN _createFuns{};
    };

    /**
     * @brief  粘性模型管理器
     * @author WangKai (wang_starry@outlook.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIViscousTypeManager
    {
    public:
        explicit FITKPHengLEIViscousTypeManager();
        virtual ~FITKPHengLEIViscousTypeManager();

        /**
         * @brief    获取粘性模型链表
         * @return   QStringList
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        QStringList filterTurbence(FITKPHengLEIAbsSolver::PHengLEISolverType solverType,FITKPHengLEIAbsSolver::PHengLEIViscousType viscousType);
        /**
         * @brief    获取粘性模型信息
         * @param[i] turbulenceName 来流类型名称
         * @return   FITKPHengLEIInFlowTypeInfo
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
		FITKPHengLEIViscousTypeInfo getViscousTypeInfoByName(QString viscousTypeName);
    private:
        /**
         * @brief    初始化粘性模型参数
         * @return   void
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        void initViscousTypes();
    private:
        /**
         * @brief  粘性模型链表
         * @author WangKai (wang_starry@outlook.com)
         * @date   2024-08-21
         */
        QList<FITKPHengLEIViscousTypeInfo> _viscousTypes{};
    };
 
}


#endif
