/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKRadiossGeomPart.h
 * @brief  Radioss几何部件类声明
 * @author libaojun
 * @date 2025-11-11
 */
#ifndef __FITKRADIOSSGEOMPART_H__
#define __FITKRADIOSSGEOMPART_H__

#include "FITKRadiossDataAPI.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    class FITKAbsGeoPart;
}

namespace Radioss
{
    /**
     * @brief Radioss几何部件类
     * @details 此类用于描述Radioss几何部件，继承自抽象部件类
     * @author libaojun
     * @date 2025-11-11
     */
    class FITKRadiossDataAPI FITKRadiossGeomPart : public Interface::FITKAbstractPart
    {
        FITKCLASS(Radioss, FITKRadiossGeomPart);
        FITKSafeDownCastA(FITKRadiossGeomPart, Interface::FITKAbstractPart);
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         * @date 2025-10-15
         */
        explicit FITKRadiossGeomPart();
        
        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2025-10-15
         */
        virtual ~FITKRadiossGeomPart();
        /**
         * @brief 获取抽象模型类型
         * @return 抽象模型类型枚举值
         * @author libaojun
         * @date 2025-11-11
         */
        Interface::FITKModelEnum::AbsModelType getAbsModelType()  override;
       
        /**
         * @brief 获取抽象类型网格
         * @return 网格数据指针
         * @author libaojun
         * @date 2025-11-11
         */
        virtual Core::FITKAbstractNDataObject* getAbstractMesh() override;

        /**
         * @brief 获取抽象类型几何
         * @return 几何数据指针
         * @author libaojun
         * @date 2025-11-11
         */
        virtual Core::FITKAbstractNDataObject* getAbstractGeom() override;
        /**
         * @brief    通过Index获取几何部件
         * @param[i] index
         * @return   Interface::FITKAbsGeoPart *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-22
         */
        Interface::FITKAbsGeoPart* getGeoPart();
        /**
         * @brief    判断几何是否被引用
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-22
         */
        bool isGeometryReferencedCmd();
        /**
         * @brief    
         * @param[i] geoPartID 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-22
         */
        void setGeoPartID(int geoPartID);
        /**
         * @brief    
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-22
         */
        int getGeoPartID();

    private:
        /**
         * @brief    初始化几何部件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-22
         */
        void initGeoPart();
        /**
         * @brief    移除几何部件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-22
         */
        void removeGeoPart();

    private:
        /**
         * @brief 几何命令ID
         * @author libaojun
         * @date 2025-11-11
         */
        int _geomCommandID{-1};
    };
    /**
     * @brief Radioss几何部件管理器类
     * @details 此类用于管理Radioss几何部件，继承自抽象数据管理器模板类
     * @author libaojun
     * @date 2025-11-11
     */
    class FITKRadiossDataAPI FITKRadiossGeomPartManager :
        public Core::FITKAbstractDataManager<FITKRadiossGeomPart>
    {   
    public:
        explicit FITKRadiossGeomPartManager() = default;
        virtual ~FITKRadiossGeomPartManager() = default;

    };

} // namespace Radioss


#endif // __FITKRADIOSSGEOMPART_H__