/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRegionGeometryRefine.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Interface
{
    void FITKRegionGeometryRefine::setGeometryId(int geoId)
    {
        //设置组件
        m_geometryId = geoId;
    }

    int FITKRegionGeometryRefine::getGeometryId()
    {
        //获取组件Id
        return m_geometryId;
    }

    Interface::FITKAbsGeoCommand* FITKRegionGeometryRefine::getGeometryModel()
    {
        //获取几何数据对象
        return FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_geometryId);
    }

    void FITKRegionGeometryRefine::setLimitRefinement(bool limit)
    {
        //设置是否限制优化区域
        m_limitRefinement = limit;
    }
    bool FITKRegionGeometryRefine::getLimitRefineMent()
    {
        //获取是否限制优化区域
        return m_limitRefinement;
    }

    void FITKRegionGeometryRefine::setRefineModel(FITKRegionGeometryRefine::RefineModelType refineType)
    {
        //设置优化模式类型
        m_refineModel = refineType;
    }

    FITKRegionGeometryRefine::RefineModelType FITKRegionGeometryRefine::getRefineModel()
    {
        //获取优化模式类型
        return m_refineModel;
    }

    void FITKRegionGeometryRefine::setDistance(double distance)
    {
        //设置距离
        m_distance = distance;
    }
    double FITKRegionGeometryRefine::getDistance()
    {
        //获取距离
        return m_distance;
    }

    void FITKRegionGeometryRefine::setLevel(int level)
    {
        //设置级别
        m_level = level;
    }
    int FITKRegionGeometryRefine::getLevel()
    {
        //获取级别
        return m_level;
    }

    void FITKRegionGeometryRefine::setGapRefineModel(FITKRegionGeometryRefine::GapModelType gapType)
    {
        //设置间隙模式
        m_gapModel = gapType;
    }
    FITKRegionGeometryRefine::GapModelType FITKRegionGeometryRefine::getGapRefineModel()
    {
        //获取间隙模式
        return m_gapModel;
    }

    void FITKRegionGeometryRefine::setCellsInGapNum(int cells)
    {
        //设置网格在间隙的数量
        m_cellInGapNum = cells;
    }
    int FITKRegionGeometryRefine::getCellsInGapNum()
    {
        //获取网格在间隙的数量
        return m_cellInGapNum;
    }

    void FITKRegionGeometryRefine::setMaxGapLevel(int maxGap)
    {
        //设置最大间隙级别
        m_maxGapLevel = maxGap;
    }
    int FITKRegionGeometryRefine::getMaxGapLevel()
    {
        //获取最大间隙级别
        return m_maxGapLevel;
    }
}
