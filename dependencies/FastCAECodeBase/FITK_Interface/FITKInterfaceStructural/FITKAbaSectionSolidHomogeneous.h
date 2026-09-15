/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSectionSolidHomogeneous.h
 * @brief  均匀材质实体截面属性
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABASECTIONSOLIDHOMOGENEOUS_H
#define FITKABASECTIONSOLIDHOMOGENEOUS_H
#include "FITKAbaAbstractSection.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"


namespace Interface {
    /**
     * @brief  均匀材质实体截面属性类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaSectionSolidHomogeneous: public FITKAbaAbastractSection
    {
        FITKCLASS(Interface, FITKAbaSectionSolidHomogeneous);
    public:
        /**
         * @brief  获取截面类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaSectionType getSectionType() override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(FITKAbstractDataObject* obj) override;

        /**
          * @brief  获取截面类型字符串，用于UI显式
          * @return QString
          * @author libaojun (libaojunqd@foxmail.com)
          * @date   2024-03-27
          */
        virtual QString getTypeString() override;
    };

}

#endif // FITKABASECTIONSOLIDHOMOGENEOUS_H
