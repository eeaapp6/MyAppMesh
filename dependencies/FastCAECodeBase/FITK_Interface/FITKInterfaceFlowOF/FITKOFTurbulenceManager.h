/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFTurbulenceManager.h
 * @brief  湍流模型管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK_TURBS_OF_MANAGER_H___
#define _FITK_TURBS_OF_MANAGER_H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include <functional>
#include <QStringList>

namespace Interface
{
    class FITKAbstractOFTurbulence;
}

typedef std::function<Interface::FITKAbstractOFTurbulence*()>  CREATETURBULENCEFUN;

namespace Interface
{
    /**
     * @brief  湍流模型信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFTurbulenceInfo
    {
        QString _turbulenceName{};
        FITKOFSolverTypeEnum::FITKOFTurbulenceType _turbType{ FITKOFSolverTypeEnum::FITKOFTurbulenceType::TurbNone };
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableSolverType;
        CREATETURBULENCEFUN _createFuns{};
    };

    /**
     * @brief  湍流模型管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFTurbulenceManager
    {
    public:
        explicit FITKOFTurbulenceManager();
        virtual ~FITKOFTurbulenceManager();

        /**
         * @brief    过滤湍流模型类型获取模型类型链表
         * @param[i] tbType 湍流模型类型
         * @param[i] stype 求解器类型
         * @return   QList<QString>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        QStringList filterTurbence(FITKOFSolverTypeEnum::FITKOFTurbulenceType tbType, FITKOFSolverTypeEnum::FITKOFSolverType stype);
        /**
         * @brief    获取湍流模型信息
         * @param[i] turbulenceName 湍流模型名称
         * @return   FITKOFTurbulenceInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFTurbulenceInfo getTurbenceInfoByTurbulenceName(QString turbulenceName);

    private:
        /**
         * @brief    初始化湍流模型信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initTurbulences();

    private:
        /**
         * @brief  湍流模型信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        QList<FITKOFTurbulenceInfo> _turbulences{};
    };
 
}


#endif
