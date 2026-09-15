/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractInertia.h
 * @brief  惯性元素基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-05-10
 * 
 */
#ifndef FITKABAABSTRACTINERTIA_H
#define FITKABAABSTRACTINERTIA_H
#include "FITK_Interface/FITKInterfacePhysics/FITKBoundaryLocation.h"
#include "FITKInterfaceStructuralAPI.h"

namespace Interface {

    /**
     * @brief  惯性元素基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-05-10
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractInertia: public FITKBoundaryLocation
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaAbstractInertia);
    public:
        /**
         * @brief  惯性元素类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        enum FITKAbaInertiaType
        {
            PointMassInertia,
            NonstructuralMass,
            HeatCapacitance,
        };
        Q_ENUM(FITKAbaInertiaType);

    public:
        /**
         * @brief  默认构造
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        FITKAbaAbstractInertia() = default;
        /**
         * @brief  析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        ~FITKAbaAbstractInertia() = default;
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取惯性元素类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        virtual FITKAbaInertiaType getInertiaType() const = 0;
        /**
         * @brief  获取相互作用的激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-28
         */
        bool getActiveState() const;
        /**
         * @brief  设置相互作用的激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-28
         */
        void setActiveState(bool stat);
    protected:
        /**
         * @brief  激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-28
         */
        bool m_activeState{ true };
    };
}


#endif // FITKABAABSTRACTINERTIA_H
