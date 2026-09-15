/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractInteractionProperty.h
 * @brief  相互作用属性抽象类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAABSTRACTINTERACTIONPROPERTY_H
#define FITKABAABSTRACTINTERACTIONPROPERTY_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"


namespace Interface {
    
    /**
     * @brief  相互作用属性抽象类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractInteractionProperty: public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  相互作用属性枚举类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        enum FITKAbaInteractionPropertyType
        {
            Contact,
        };
        Q_ENUM(FITKAbaInteractionPropertyType);

    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaAbstractInteractionProperty() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaAbstractInteractionProperty() = default;
        /**
         * @brief  获取相互作用类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        virtual FITKAbaInteractionPropertyType getInteractionPropertyType() const = 0;
    };
}


#endif // FITKABAABSTRACTINTERACTIONPROPERTY_H
