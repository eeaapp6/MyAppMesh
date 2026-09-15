/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKGeometryMeshSize.h
 * @brief 几何网格尺寸类声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-22
 */
#ifndef __FITKGEOMETRYMESHSIZE_H__
#define __FITKGEOMETRYMESHSIZE_H__

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    class FITKGeoComponent;
    class FITKAbsGeoCommand;
    /**
     * @brief 几何划分网格尺寸类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-22
     */
    class FITKInterfaceMeshGenAPI FITKGeometryMeshSize :
        public Core::FITKAbstractNDataObject, public Core::FITKVarientParams
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKGeometryMeshSize);
    public:
        /**
         * @brief Construct a new FITKGeometryMeshSize object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        explicit FITKGeometryMeshSize() = default;
        /**
         * @brief Destroy the FITKGeometryMeshSize object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        virtual ~FITKGeometryMeshSize() = default;
        /**
         * @brief 设置几何组件Id
         * @param[i] geoComponentId 几何组件Id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        void setGeoGroupComponentId(int geoComponentId);
        /**
         * @brief 获取几何组件Id
         * @return int 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        int getGeoGroupComponentId();
        /**
         * @brief 获取几何组件数据对象
         * @return Interface::FITKGeoComponent*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-26
         */
        Interface::FITKGeoComponent* getGeoComponent();
        /**
         * @brief 获取几何模型数据对象
         * @return Interface::FITKAbsGeoCommand*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-26
         */
        Interface::FITKAbsGeoCommand* getGeoModel();
        /**
         * @brief 设置层数
         * @param[i] number 个数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        void setLayerNumber(int number);
        /**
         * @brief 获取层数
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        int getLayerNumber();
        /**
         * @brief 设置膨胀率
         * @param[i] expansionRatio
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        void setExpansionRatio(double expansionRatio);
        /**
         * @brief 获取膨胀率
         * @return double
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        double getExpansionRatio();
        /**
         * @brief 设置层厚度
         * @param[i] layerThickness
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        void setLayerThickness(double layerThickness);
        /**
         * @brief 获取层厚度
         * @return double
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        double getLayerThickness();
        
    private:
        /**
         * @brief 几何组件Id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        int _geoGroupComponentId{ 0 };
        /**
         * @brief 层数个数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        int _layerNum{ 5 };
        /**
         * @brief 膨胀率
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        double _expansionRatio{ 1.25 };
        /**
         * @brief 层厚度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        double _layerThickness{ 0.2 };
    };
}

namespace Interface
{
    /**
     * @brief 几何划分网格管理类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-22
     */
    class FITKInterfaceMeshGenAPI FITKGeometryMeshSizeManager :
        public Core::FITKAbstractDataManager<FITKGeometryMeshSize>
    {
    public:
        /**
         * @brief Construct a new FITKGeometryMeshSizeManager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        explicit FITKGeometryMeshSizeManager() = default;
        /**
         * @brief Destroy the FITKGeometryMeshSizeManager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-22
         */
        virtual ~FITKGeometryMeshSizeManager() = default;
    };
}

#endif