/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialPlasticData.h
 * @brief  材料塑性数据类。包括各向同性、JohsonCook等类型。这些类型不对外暴露接口。
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALPLASTICDATA_H
#define FITKABAMATERIALPLASTICDATA_H

namespace Interface {
    /**
     * @brief  塑性各向同性参数类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-21
     */
    class PlasticIsotropic
    {
    public:
        /**
         * @brief  获取屈服应力
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getYieldStress() const;
        /**
         * @brief  获取塑性应变
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getPlasticStrain() const;
        /**
         * @brief  获取温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getTemperature() const;
        /**
         * @brief  获取应变率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getRate() const;
        /**
         * @brief  设置屈服应力
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setYieldStress(double value);
        /**
         * @brief  设置塑性应变
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setPlasticStrain(double value);
        /**
         * @brief  设置温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTemperature(double temperature);
        /**
         * @brief  设置应变率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setRate(double value);
    private:
        /**
         * @brief  屈服应力
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_yeildStress{0.0};
        /**
         * @brief  塑性应变
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_plasticStrain{0.0};
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_temperature{0.0};
        /**
         * @brief  应变率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_rate{0.0};
    };
    /**
     * @brief  塑性JohnsonCook本构参数类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-21
     */
    class PlasticJohnsonCook
    {
    public:
        /**
         * @brief  获取参数A
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getA() const;
        /**
         * @brief  获取参数B
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getB() const;
        /**
         * @brief  获取参数N
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getN() const;
        /**
         * @brief  获取参数M
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getM() const;
        /**
         * @brief  获取融化温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getMeltingTemp() const;
        /**
         * @brief  获取过渡温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getTransitionTemp() const;
        /**
         * @brief  设置参数A
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setA(double value);
        /**
         * @brief  设置参数B
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setB(double value);
        /**
         * @brief  设置参数N
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setN(double value);
        /**
         * @brief  设置参数M
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setM(double value);
        /**
         * @brief  设置融化温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setMeltingTemp(double value);
        /**
         * @brief  设置过渡温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTransitionTemp(double value);
    private:
        /**
         * @brief  参数A
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_a{0.0};
        /**
         * @brief  参数B
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_b{0.0};
        /**
         * @brief  参数n
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_n{0.0};
        /**
         * @brief  参数m
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_m{0.0};
        /**
         * @brief  融化温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_meltingTemp{0.0};
        /**
         * @brief  过渡温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_transitionTemp{0.0};
    };

}
#endif // FITKABAMATERIALPLASTICDATA_H
