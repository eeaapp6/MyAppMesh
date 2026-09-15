/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaLoadGravity.h
 * @brief  重力载荷
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-09
 * 
 */
#ifndef FITKABALOADGRAVITY_H
#define FITKABALOADGRAVITY_H
#include "FITKAbaLoadBodyForce.h"


namespace Interface {
    /**
     * @brief  重力载荷类，与体力BodyForce遵循相同的接口
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-09
     */
    class FITKInterfaceStructuralAPI FITKAbaLoadGravity:public FITKAbaLoadBodyForce
    {
        FITKCLASS(Interface, FITKAbaLoadGravity)
    public:
        /**
         * @brief  构造函数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaLoadGravity() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaLoadGravity() = default;
        /**
         * @brief  获取载荷类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaLoadType getLoadType() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  是否作用于整个模型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        bool isWholeModel() const;
        /**
         * @brief  设置是否作用于整个模型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        void setWholeModel(bool state);
    private:
        /**
         * @brief  是否作用于整个模型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        bool m_isWholeModel{ true };
    };
}


#endif // FITKABALOADGRAVITY_H
