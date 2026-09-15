/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialHyperelasticData.h
 * @brief  超弹性属性行为数据类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-29
 * 
 */
#ifndef FITKABAMATERIALHYPERELASTICDATA_H
#define FITKABAMATERIALHYPERELASTICDATA_H


namespace Interface {
    /**
     * @brief  超弹性MooneyRivlin模型数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-29
     */
    class MooneyRivlinData
    {
    public:
        /**
         * @brief  获取参数C10
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getC10() const;
        /**
         * @brief  获取参数C01
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getC01() const;
        /**
         * @brief  获取参数CD1
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getD1() const;
        /**
         * @brief  获取参数温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getTemperature() const;
        /**
         * @brief  设置参数C10
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setC10(double value);
        /**
         * @brief  设置参数C01
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setC01(double value);
        /**
         * @brief  设置参数D1
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setD1(double value);
        /**
         * @brief  设置参数温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTemperature(double value);
    private:
        /**
         * @brief  参数C10
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_C10{0.0};
        /**
         * @brief  参数C01
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_C01{0.0};
        /**
         * @brief  参数D1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_D1{0.0};
        /**
         * @brief  参数温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_temperature{0.0};
    };
}


#endif // FITKABAMATERIALHYPERELASTICDATA_H
