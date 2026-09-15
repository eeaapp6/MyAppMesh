/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFBoundaryTypeManager.h
 * @brief  边界类型管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-26
 *********************************************************************/
#ifndef _FITK_OF_BOUNDARY_TYPEMANAGER_H___
#define _FITK_OF_BOUNDARY_TYPEMANAGER_H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include <functional>
#include <QStringList>
#include <QHash>

namespace Interface
{
    class FITKOFAbsBoundaryType;
}

typedef std::function<Interface::FITKOFAbsBoundaryType*()>  CREATEBOUNDARYTYPEFUN;

namespace Interface
{
    /**
     * @brief  边界类型信息
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-23
     */
    struct FITKOFBoundaryTypeInfo
    {
        QString _boundaryTypeName{};
        QHash<FITKOFSolverTypeEnum::FITKOFBoundaryType, QStringList> _enableBoundary;//边界类型, 变量名
        CREATEBOUNDARYTYPEFUN _createFuns;
    };
    /**
     * @brief  边界类型管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-23
     */
    class FITKInterfaceFlowOFAPI FITKOFBoundaryTypeManager
    {
    public:
        explicit FITKOFBoundaryTypeManager();
        virtual ~FITKOFBoundaryTypeManager();
        /**
         * @brief    过滤器
         * @param[i] solverType 
         * @param[i] boundaryType 
         * @param[i] variableType 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        QStringList filterBoundariesType(FITKOFSolverTypeEnum::FITKOFSolverType solverType, FITKOFSolverTypeEnum::FITKOFBoundaryType boundaryType, QString variableType);
        /**
         * @brief    获取边界类型信息
         * @param[i] boundaryTypeName 
         * @param[i] solverType 
         * @param[i] variableType 
         * @param[i] boundaryType 
         * @return   FITKOFBoundaryTypeInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        FITKOFBoundaryTypeInfo getBoundaryTypeInfo(QString boundaryTypeName, FITKOFSolverTypeEnum::FITKOFSolverType solverType, QString variableType, FITKOFSolverTypeEnum::FITKOFBoundaryType boundaryType);
    
    private:
        /**
         * @brief    初始化SIMPLE求解器Boundary Type数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-31
         */
        void initSIMPLESolverBTy();
        /**
         * @brief    初始化Inter求解器Boundary Type数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-31
         */
        void initInterSolverBTy();
        /**
         * @brief    初始化CHT Multi Region求解器Boundary Type数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        void initCHTMultiRegionSolverBTy();

    private:
        /**
         * @brief  边界数据信息 求解器，边界信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-31
         */
        QHash<FITKOFSolverTypeEnum::FITKOFSolverType, QList<FITKOFBoundaryTypeInfo*>> m_boundaryTypes{};
    };
}


#endif
