/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaProfileBox.h
 * @brief  截面形状Box
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAPROFILEBOX_H
#define FITKABAPROFILEBOX_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaProfile.h"


namespace Interface {
   
    /**
     * @brief  Box型截面类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaProfileBox: public FITKAbaProfile
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaProfileBox);
    public:
        /**
        * @brief  Box型截面厚度类型
        * @author LiJin (1003680421@qq.com)
        * @date   2024-04-15
        */
        enum BoxThickness
        {
            Uniform,
            Individual
        };
        Q_ENUM(BoxThickness);
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
         * @brief  获取宽度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getWidth() const;
        /**
         * @brief  设置宽度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setWidth(double value);
        /**
         * @brief  获取高度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getHeight() const;
        /**
         * @brief  设置高度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setHeight(double value);
        /**
         * @brief  获取厚度类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        BoxThickness getThicknessType() const;
        /**
         * @brief  设置厚度类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setThicknessType(BoxThickness type);
        /**
         * @brief  获取统一厚度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getThickness() const;
        /**
         * @brief  设置统一厚度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setThickness(double value);
        /**
         * @brief  获取厚度T1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getT1() const;
        /**
         * @brief  设置厚度T1
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setT1(double value);
        /**
         * @brief  获取厚度T2
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getT2() const;
        /**
         * @brief  设置厚度T2
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setT2(double value);
        /**
         * @brief  获取厚度T3
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getT3() const;
        /**
         * @brief  设置厚度T3
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setT3(double value);
        /**
         * @brief  获取厚度T4
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getT4() const;
        /**
         * @brief  设置厚度T4
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setT4(double value);
    private:
        /**
         * @brief  宽度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_width{0.0};
        /**
         * @brief  高度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_height{0.0};
        /**
         * @brief  厚度类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        BoxThickness m_thicknessType{BoxThickness::Uniform};
        /**
         * @brief  统一厚度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_thickness{0.0};
        /**
         * @brief  厚度t1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_t1{0.0};
        /**
         * @brief  厚度t2
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_t2{0.0};
        /**
         * @brief  厚度t3
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_t3{0.0};
        /**
         * @brief  厚度t4
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_t4{0.0};
    };
}


#endif // FITKABAPROFILEBOX_H
