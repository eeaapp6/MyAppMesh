/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file   FITKAbaSection.h
 * @brief  结构模板（Abaqus）截面属性。
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 *
 */
#ifndef FITKABASECTION_H
#define FITKABASECTION_H

#include <QString>
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"


namespace Interface
{
    class FITKAbaMaterial;
  
    /**
     * @brief  截面属性抽象类型
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaAbastractSection: public FITKAbstractSection
    {
        Q_OBJECT
    public:
        /**
         * @brief  截面属性枚举类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        enum FITKAbaSectionType
        {
            None,
            SolidHomogeneous,
            ShellHomogeneous,
            Beam,
            Truss,
        };
        Q_ENUM(FITKAbaSectionType);

        enum TemperatureVariation
        {
            LinearByGradients,
            InterpolatedFromTemperaturePoints
        };
        Q_ENUM(TemperatureVariation)

    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        explicit FITKAbaAbastractSection() = default;
        /**
         * @brief  抽象虚析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        virtual ~FITKAbaAbastractSection() = 0;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取截面属性类型。便于动态类型转换  
         * @return 截面属性枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        virtual FITKAbaSectionType getSectionType() = 0;
        /**
          * @brief  获取截面类型字符串，用于UI显式
          * @return QString
          * @author libaojun (libaojunqd@foxmail.com)
          * @date   2024-03-27
          */
        virtual QString getTypeString() override;
        
    };
}


#endif // FITKABASECTION_H
