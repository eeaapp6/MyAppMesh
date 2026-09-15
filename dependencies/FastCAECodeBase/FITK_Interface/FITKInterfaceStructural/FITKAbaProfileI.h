/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaProfileI.h
 * @brief  截面形状I类型
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-21
 * 
 */
#ifndef FITKABAPROFILEI_H
#define FITKABAPROFILEI_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITKAbaProfile.h"


namespace Interface {
    /**
     * @brief  I型截面形状类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-21
     */
    class FITKInterfaceStructuralAPI FITKAbaProfileI: public FITKAbaProfile
    {
        FITKCLASS(Interface, FITKAbaProfileI);
    public:
        /**
         * @brief  获取截面形状类型
         * @param[in]  
         * @return 
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
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief  获取I型截面形状参数I
         * @param[in]  void
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double getI() const;
        /**
         * @brief  获取I型截面形状参数h
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double geth() const;
        /**
         * @brief  获取I型截面形状参数b1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double getb1() const;
        /**
         * @brief  获取I型截面形状参数b2
         * @param [in] 
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double getb2() const;
        /**
         * @brief  获取I型截面形状参数t1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double gett1() const;
        /**
         * @brief  获取I型截面形状参数t2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double gett2() const;
        /**
         * @brief  获取I型截面形状参数t3
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        double gett3() const;
        /**
         * @brief  设置I型截面参数I
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void setI(double value);
        /**
         * @brief  设置I型截面参数h
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void seth(double value);
        /**
         * @brief  设置I型截面参数b1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void setb1(double value);
        /**
         * @brief  设置I型截面参数b2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void setb2(double value);
        /**
         * @brief  设置I型截面参数t1
         * @param[in] 
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void sett1(double value);
        /**
         * @brief  设置I型截面参数t2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void sett2(double value);
        /**
         * @brief  设置I型截面参数t3
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        void sett3(double value);
    private:
        /**
         * @brief  I型截面参数I
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_I{0.0};
        /**
         * @brief  I型截面参数h
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_h{0.0};
        /**
         * @brief  I型截面参数b1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_b1{0.0};
        /**
         * @brief  I型截面参数b2
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_b2{0.0};
        /**
         * @brief  I型截面参数t1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_t1{0.0};
        /**
         * @brief  I型截面参数t2
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_t2{0.0};
        /**
         * @brief  I型截面参数t3
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-27
         */
        double m_t3{0.0};
    };
}


#endif // FITKABAPROFILEI_H
