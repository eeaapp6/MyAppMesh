/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaEngineeringFeature.h
 * @brief 部件与集合工程特征定义
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-05-27
 *
 */

#ifndef _FITKABAENGINEERINGFEATURE_H___
#define _FITKABAENGINEERINGFEATURE_H___

#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface
{
    class FITKAbaInertiaManager;
    class FITKAbaSpringDashpotManager;
/**
 * @brief 部件工程特征管理器
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-05-27
 */
class FITKInterfaceStructuralAPI FITKAbaPartEngineeringFeature :
    public Core::FITKAbstractDataObject
{
public:
    /**
     * @brief Construct a new FITKAbaPartEngineeringFeature object
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-05-27
     */
    explicit FITKAbaPartEngineeringFeature();
    /**
     * @brief Destroy the FITKAbaPartEngineeringFeature object
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-05-27
     */
    virtual ~FITKAbaPartEngineeringFeature();
 
    /**
     * @brief 获取惯量管理器
     * @return FITKAbaInertiaManager*
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-05-27
     */
    FITKAbaInertiaManager* getInertiaManager();

    /**
     * @brief 获取弹簧管理器
     * @return FITKAbaSpringDashpotManager*
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    FITKAbaSpringDashpotManager* getSpringDashManager();
 
    /***
   * @brief 设置数据对象ID，需谨慎使用
   * @param ID 数据对象id
   * @author LiBaojun (libaojunqd@foxmail.com)
   * @date 2024-06-18
   */
   virtual void setParentDataID(const int id) override;


   /**
    * @brief 清空数据
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2024-11-20
    */
   virtual void clear();

protected:
    /**
     * @brief 惯量管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-05-27
     */
    FITKAbaInertiaManager* _inertiaManager{};
    /**
     * @brief 弹簧管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-06-20
     */
    FITKAbaSpringDashpotManager* _springDashManager{};
};
    /**
     * @brief 装配体工程特征管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-05-27
     */
    class FITKInterfaceStructuralAPI FITKAbaAssemblyEngineeringFeature :
         public FITKAbaPartEngineeringFeature
    {
    public:
        /**
         * @brief Construct a new FITKAbaAssemblyEngineeringFeature object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-27
         */
        explicit FITKAbaAssemblyEngineeringFeature() = default;
        /**
         * @brief Destroy the FITKAbaAssemblyEngineeringFeature object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-05-27
         */
        virtual ~FITKAbaAssemblyEngineeringFeature() = default;
 
        /***
       * @brief 设置数据对象ID，需谨慎使用
       * @param ID 数据对象id
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-06-18
       */
        virtual void setParentDataID(const int id) override;
    };
}


#endif