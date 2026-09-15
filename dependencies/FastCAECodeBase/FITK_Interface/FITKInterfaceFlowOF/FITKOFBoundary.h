/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFBoundary.h
 * @brief  流体相关边界数据结构
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-04-14
 *********************************************************************/
#ifndef _FITK_OF_BOUNDARY__H___
#define _FITK_OF_BOUNDARY__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKOFEnum.hpp"

namespace Interface
{
    class FITKOFAbsBoundaryType;
    class FITKBoundaryMeshVTK;
    /**
     * @brief  边界数据对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-04-14
     */
    class FITKInterfaceFlowOFAPI FITKOFBoundary : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT;
        FITKCLASS(Interface, FITKOFBoundary);
    public:
        /**
         * @brief    构造函数
         * @param[i] bType                      边界类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        explicit FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType bType);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        virtual ~FITKOFBoundary();
        /**
         * @brief    获取边界类型
         * @return   FITKOFSolverTypeEnum::FITKOFBoundaryType           边界类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        FITKOFSolverTypeEnum::FITKOFBoundaryType getBoundaryType();
        /**
         * @brief    设置网格边界id
         * @param[i] regionID                    网格区域id
         * @param[i] meshId                      网格边界id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void setMeshBoundaryID(int regionID, int meshId);
        /**
         * @brief    获取网格边界id
         * @return   int                         网格边界id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        int getMeshBoundaryID();
        /**
         * @brief    获取区域网格ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-26
         */
        int getMeshRegionID();
        /**
         * @brief    获取网格边界
         * @return   FITKBoundaryMeshVTK*         网格边界
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        FITKBoundaryMeshVTK* getMeshBoundary();
        /**
         * @brief    获取流体参数类型数量
         * @return   int                        流体参数类型数量（P、U等）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        int getFlowCount();
        /**
         * @brief    通过索引获取流体参数类型名称
         * @param[i] index                       索引
         * @return   QString                     流体参数类型名称（P、U等）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        QString getFlowVariableName(int index);
        /**
         * @brief    通过索引获取流体参数类型对象
         * @param[i] index                       索引
         * @return   FITKOFAbsBoundaryType*      流体参数类型对象（P、U等）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        FITKOFAbsBoundaryType* getFlowVBType(int index);
        /**
         * @brief    获取湍流参数类型数量
         * @return   int                         湍流参数类型数量（K、omega等）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        int getTurbulenceCount();
        /**
         * @brief    通过索引获取湍流参数类型名称
         * @param[i] index                       索引
         * @return   QString                     湍流参数类型名称（K、omega等）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        QString getTurbulenceVariableName(int index);
        /**
         * @brief    通过索引获取湍流参数类型对象
         * @param[i] index                       索引
         * @return   FITKOFAbsBoundaryType*      湍流参数类型对象（K、omega等）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        FITKOFAbsBoundaryType* getTurbulenceVBType(int index);
        /**
         * @brief    获取Phases参数数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        int getPhasesCount();
        /**
         * @brief    通过index获取Phases的变量
         * @param[i] index 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        QString getPhasesVariableName(int index);
        /**
         * @brief    通过index获取Phases的边界数据
         * @param[i] index 
         * @return   FITKOFAbsBoundaryType *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        FITKOFAbsBoundaryType* getPhasesVBType(int index);
        /**
         * @brief    获取热力学参数数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        int getThermalCount();
        /**
         * @brief    通过index获取热力学的变量
         * @param[i] index 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        QString getThermalVariableName(int index);
        /**
         * @brief    通过index获取热力学的变量的边界数据
         * @param[i] index 
         * @return   FITKOFAbsBoundaryType *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        FITKOFAbsBoundaryType* getThermalVBType(int index);
        /**
         * @brief    设置流体边界参数类型对象（数据存在替换、数据不存在添加）
         * @param[i] variableName                      名称
         * @param[i] boundaryType                      参数类型对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void setFlowBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType);
        /**
         * @brief    设置湍流边界参数类型对象（数据存在替换、数据不存在添加）
         * @param[i] variableName                      名称
         * @param[i] boundaryType                      参数类型对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void setTurbulenceBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType);
        /**
         * @brief    设置材料边界参数类型对象
         * @param[i] variableName 
         * @param[i] boundaryType 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        void setPhasesBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType);
        /**
         * @brief    设置热力学边界参数类型对象（数据存在替换、数据不存在添加）
         * @param[i] variableName                      名称
         * @param[i] boundaryType                      参数类型对象
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-23
         */
        void setThermalBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType);
        /**
         * @brief    设置边界参数类型对象（数据存在替换、数据不存在添加）
         * @param[i] variableName                      名称
         * @param[i] boundaryType                      参数类型对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void setBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType);
        /**
         * @brief    清理湍流边界变量数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-24
         */
        void clearTurbulenceBoundary();
        /**
         * @brief    清理流体边界变量数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-24
         */
        void clearFlowBoundary();
        /**
         * @brief    通过名称获取边界参数类型对象
         * @param[i] variableName                     名称
         * @return   FITKOFAbsBoundaryType*           边界参数类型对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        FITKOFAbsBoundaryType* getBoundary(QString variableName);
    private:
        /**
         * @brief  区域ID, 网格边界id
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-14
         */
        QPair<int, int> _meshBoundaryID{ -1, -1 };
        /**
         * @brief  边界类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-14
         */
        FITKOFSolverTypeEnum::FITKOFBoundaryType _boundaryType{ FITKOFSolverTypeEnum::FITKOFBoundaryType::BNone };
        /**
         * @brief  Flow 里的变量边界信息 <变量名称, 边界类型数据对象>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-26
         */
        QList<QPair<QString, FITKOFAbsBoundaryType*>> _flow{};
        /**
         * @brief  Turbulence 里的变量边界信息 <变量名称, 边界类型数据对象>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-26
         */
        QList<QPair<QString, FITKOFAbsBoundaryType*>> _turbulence{};
        /**
         * @brief  Phases 里的变量边界信息 <变量名称, 边界类型数据对象>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-26
         */
        QList<QPair<QString, FITKOFAbsBoundaryType*>> _phases{};
        /**
         * @brief  热力学变量边界信息 <变量名称, 边界类型数据对象>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-23
         */
        QList<QPair<QString, FITKOFAbsBoundaryType*>> _thermal{};
    };
    /**
     * @brief  边界管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-04-14
     */
    class FITKInterfaceFlowOFAPI FITKOFBoundaryManager
        : public Core::FITKAbstractDataManager<FITKOFBoundary>
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        explicit FITKOFBoundaryManager() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        virtual ~FITKOFBoundaryManager() = default;
        /**
         * @brief    获取同种类型的所有边界
         * @param[i] t                               边界类型
         * @return   QList<FITKOFBoundary*>          所有边界
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        QList<FITKOFBoundary*> getBoundarys(FITKOFSolverTypeEnum::FITKOFBoundaryType t);
        /**
         * @brief    根据网格边界ID获取对应的boundary
         * @param[i] boundaryID    网格边界ID
         * @return   FITKOFBoundary*  边界对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-26
         */
        FITKOFBoundary* getBoundary(int MeshBoundaryID);
        /**
         * @brief    设置两个边界相互映射
         * @param[i] b1 
         * @param[i] b2 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-04
         */
        void setMappedWallBoundary(int b1, int b2);
        /**
         * @brief    获取两个边界相互映射
         * @return   QHash<int,int>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        QHash<int, int> getMappedWallBoundary();
        /**
         * @brief    通过一个边界ID获取另一个映射区域对象
         * @param[i] boundaryID 
         * @return   FITKOFBoundary *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-04
         */
        FITKOFBoundary* getMappendWallBoundary(int boundaryID);

    private:
        /**
         * @brief  映射边界条件-用于关联载荷工况在不同区域之间的数据传递-仅用于mappedWall边界类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-07-04
         */
        QHash<int, int> _mappedWallBoundary{};

    };

}


#endif
