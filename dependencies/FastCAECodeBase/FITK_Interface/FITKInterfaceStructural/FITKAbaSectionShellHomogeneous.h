/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSectionShellHomogeneous.h
 * @brief  均匀材质壳截面属性
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABASECTIONSHELLHOMOGENEOUS_H
#define FITKABASECTIONSHELLHOMOGENEOUS_H
#include "FITKAbaAbstractSection.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"


namespace Interface {
    /**
     * @brief  均匀材料壳截面属性类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaSectionShellHomogeneous: public FITKAbaAbastractSection
    {
        FITKCLASS(Interface, FITKAbaSectionShellHomogeneous);
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
         * @brief  获取壳厚度
         * @param  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double getThickness() const;
        /**
         * @brief  设置壳厚度
         * @param  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void setThickness(double value);
        /**
         * @brief  获取壳厚度方向积分点个数
         * @param  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        int getThicknessIntergrationPoints() const;
        /**
         * @brief  设置壳厚度方向积分点个数
         * @param  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void setThicknessIntergrationPoints(int num);

        /**
          * @brief  获取截面类型字符串，用于UI显式
          * @return QString
          * @author libaojun (libaojunqd@foxmail.com)
          * @date   2024-03-27
          */
        virtual QString getTypeString() override;

    private:
    
        /**
         * @brief  壳厚度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double m_shellThickness{0.0};
        /**
         * @brief  壳厚度方向的积分点个数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        int m_thickIntergrationPoints{5};
    };

}

#endif // FITKABASECTIONSHELLHOMOGENEOUS_H
