/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaProfilePipe.h
 * @brief  截面形状Pipe
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAPROFILEPIPE_H
#define FITKABAPROFILEPIPE_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaProfile.h"


namespace Interface {

    /**
     * @brief  Pipe型截面形状类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaProfilePipe: public FITKAbaProfile
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKAbaProfilePipe);
    public:
        /**
         * @brief  Pipe形截面方程式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        enum PipeFormulation
        {
            ThinWalled,
            ThickWalled
        };
        Q_ENUM(PipeFormulation);

    public:
        /**
         * @brief  获取截面形状类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaProfileShape getShape() const override;
        /**
         * @brief  数据拷贝
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取方程式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        PipeFormulation getFormulation() const;
        /**
         * @brief  设置方程式
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setFormulation(PipeFormulation type);
        /**
         * @brief  获取半径
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getRadius() const;
        /**
         * @brief  设置半径
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setRadius(double value);
        /**
         * @brief  获取厚度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getThickness() const;
        /**
         * @brief  设置厚度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setThickness(double value);
    private:
        /**
         * @brief  方程式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        PipeFormulation m_formulation{ PipeFormulation::ThinWalled };
        /**
         * @brief  截面半径
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_radius{ 0.0 };
        /**
         * @brief  厚度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_thickness{0.0};
    };
}


#endif // FITKABAPROFILEPIPE_H
