/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRegionGeometryRefine.h
 * @brief  几何区域优化接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-02
 *********************************************************************/
#ifndef __FITKREGIONGEOMETRYREFINE_H__
#define __FITKREGIONGEOMETRYREFINE_H__

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    class FITKAbsGeoCommand;
    /**
     * @brief 几何划分网格尺寸类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-22
     */
    class FITKInterfaceMeshGenAPI FITKRegionGeometryRefine :
        public Core::FITKAbstractDataObject, public Core::FITKVarientParams
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKRegionGeometryRefine);
    public:
        /**
         * @brief  完善类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        enum RefineModelType
        {
            RefineDistance,
            RefineInside,
            RefineOutside,
        };
        Q_ENUM(RefineModelType);
        /**
         * @brief  间隙类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        enum GapModelType
        {
            GapNone,
            GapMixed,
            GapInside,
            GapOutside,
        };
        Q_ENUM(GapModelType);
        /**
         * @brief Construct a new FITKRegionGeometryRefine object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        explicit FITKRegionGeometryRefine() = default;
        /**
         * @brief Destroy the FITKRegionGeometryRefine object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        virtual ~FITKRegionGeometryRefine() = default;
        /**
         * @brief 设置几何Id
         * @param[i] geoId 几何Id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        void setGeometryId(int geoId);
        /**
         * @brief 获取几何Id
         * @return int 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        int getGeometryId();
        /**
         * @brief 获取几何数据对象
         * @return Interface::FITKAbsGeoCommand*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-26
         */
        Interface::FITKAbsGeoCommand* getGeometryModel();
        /**
         * @brief    设置是否限制优化区域
         * @param[i] limit 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        void setLimitRefinement(bool limit);
        /**
         * @brief    获取是否限制优化区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        bool getLimitRefineMent();
        /**
         * @brief    设置优化模式类型
         * @param[i] refineType 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        void setRefineModel(RefineModelType refineType);
        /**
         * @brief    获取优化模式类型
         * @return   RefineModelType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        RefineModelType getRefineModel();
        /**
         * @brief    设置距离
         * @param[i] distance 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        void setDistance(double distance);
        /**
         * @brief    获取距离
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        double getDistance();
        /**
         * @brief    设置级别
         * @param[i] level 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        void setLevel(int level);
        /**
         * @brief    获取级别
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        int getLevel();
        /**
         * @brief    设置间隙模式
         * @param[i] gapType 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        void setGapRefineModel(GapModelType gapType);
        /**
         * @brief    获取间隙模式
         * @return   GapModelType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        GapModelType getGapRefineModel();
        /**
         * @brief    设置网格在间隙的数量
         * @param[i] cells 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        void setCellsInGapNum(int cells);
        /**
         * @brief    获取网格在间隙的数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        int getCellsInGapNum();
        /**
         * @brief    设置最大间隙级别
         * @param[i] maxGap 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        void setMaxGapLevel(int maxGap);
        /**
         * @brief    获取最大间隙级别
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        int getMaxGapLevel();
        
    private:
        /**
         * @brief 几何Id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        int m_geometryId{ 0 };
        /**
         * @brief  是否进行优化限制
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        bool m_limitRefinement{ false };
        /**
         * @brief  优化模式
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        RefineModelType m_refineModel{ RefineModelType::RefineDistance };
        /**
         * @brief  距离
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        double m_distance{ 0.0 };
        /**
         * @brief  级别
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        int m_level{ 0 };
        /**
         * @brief  间隙模式
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        GapModelType m_gapModel{ GapModelType::GapNone };
        /**
         * @brief  网格在间隙
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        int m_cellInGapNum{ 4 };
        /**
         * @brief  最大间隙级别
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        int m_maxGapLevel{ 8 };
    };
}

namespace Interface
{
    /**
     * @brief 几何划分网格管理类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-22
     */
    class FITKInterfaceMeshGenAPI FITKRegionGeometryRefineManager :
        public Core::FITKAbstractDataManager<FITKRegionGeometryRefine>
    {
    public:
        /**
         * @brief Construct a new FITKGeometryMeshSizeManager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        explicit FITKRegionGeometryRefineManager() = default;
        /**
         * @brief Destroy the FITKGeometryMeshSizeManager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        virtual ~FITKRegionGeometryRefineManager() = default;
    };
}

#endif