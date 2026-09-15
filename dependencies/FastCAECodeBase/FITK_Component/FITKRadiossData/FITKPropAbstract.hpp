/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_RAD__PROP_ABSTRACT_H__
#define __FITK_RAD__PROP_ABSTRACT_H__

#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKRadiossDataAPI.h"

namespace Radioss
{
    /**
     * @brief 属性值基类
     * @details 此类用于描述属性值的基类，所有属性值都需要继承此类。 
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-04-24
     */
    class FITKRadiossDataAPI FITKPropValueAbstract : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKPropValueAbstract() = default;
        virtual ~FITKPropValueAbstract() = 0 {};
    };
    
    class FITKRadiossDataAPI FITKRadiossPropValueManager : public Core::FITKAbstractDataManager<FITKPropValueAbstract>
    {
    public:
        /**
         * @brief Construct a new FITKRadiossPropValueManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-24
         */
        explicit FITKRadiossPropValueManager() = default;   
        /**
         * @brief Destroy the FITKRadiossPropValueManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-24
         */
        virtual ~FITKRadiossPropValueManager() = default;
    };

        
 

    /**
     * @brief 属性基类
     * @details 此类用于描述单元属性的基类，所有单元属性都需要继承此类。
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-04-24
     */
    class FITKRadiossDataAPI FITKPropAbstract : public Interface::FITKAbstractSection     
    {
        Q_OBJECT
    public:
        enum RadPropType
        {
            Unknown = -1,
            Beam,
            Shell,
            Solid,
            Sandwich,
            BeamSPR,
            SPH,
        };
        Q_ENUM(RadPropType);
    public:
         explicit FITKPropAbstract() = default;
         virtual ~FITKPropAbstract() = 0{};

         //获取属性类型
         virtual RadPropType getRadPropType() = 0;

         /**
         * @brief 设置材料的数据ID
         * @param[i]  id            材料id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void setMaterial(int id, int index = 0) = delete;
        /**
         * @brief 设置材料
         * @param[i]  mat            材料指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void setMaterial(Core::FITKAbstractDataObject* mat, int index = 0) = delete;
        /**
         * @brief 获取材料ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        int getMaterialID( int index = 0) const = delete;
        /**
         * @brief 获取材料对象
         * @return FITKAbaMaterial*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        Interface::FITKAbstractMaterial* getMaterial(int index = 0) const = delete;
        /**
         * @brief 获取材料名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        QString getMaterialName( int index = 0) const = delete;
        /**
         * @brief 获取材料数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        int getMaterialCount() = delete;
        /**
         * @brief 移除第index个材料
         * @param[i] index 索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void removeMaterial(int index) = delete;
  
    };
}

#endif // __FITK_RAD__PROP_ABSTRACT_H__