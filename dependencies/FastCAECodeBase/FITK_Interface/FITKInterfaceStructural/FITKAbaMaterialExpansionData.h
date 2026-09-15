/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialExpansionData.h
 * @brief  材料热膨胀属性数据。包括各向同性、正交各向同性、各向异性等类型属性数据类。
 * 这些类不对外暴露接口。
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALEXPANSIONDATA_H
#define FITKABAMATERIALEXPANSIONDATA_H


namespace Interface {
    /**
     * @brief  热膨胀各向同性数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-29
     */
    class ExpansionIsotropicData
    {
    public:
        /**
         * @brief  获取膨胀系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getExpansionCoeff() const;
        /**
         * @brief  设置膨胀系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setExpansionCoeff(double value);
        /**
         * @brief  获取温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getTemperature() const;
        /**
         * @brief  设置温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTemperature(double value);
    private:
        /**
         * @brief  热膨胀系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double m_expansionCoeff{0.0};
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double m_temperature{0.0};
    };
    /**
     * @brief  热膨胀正交各向异性数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-29
     */
    class ExpansionOrthotropicData
    {
    public:
        /**
         * @brief  获取Alpha11
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha11() const;
        /**
         * @brief  获取Alpha22
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha22() const;
        /**
         * @brief  获取Alpha33
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha33() const;
        /**
         * @brief  获取温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getTemperature() const;
        /**
         * @brief  设置Alpha11
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha11(double value);
        /**
         * @brief  设置Alpha22
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha22(double value);
        /**
         * @brief  设置Alpha33
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha33(double value);
        /**
         * @brief  设置温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTemperature(double value);
    private:
        /**
         * @brief  alpha11
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha11{0.0};
        /**
         * @brief  alpha22
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha22{0.0};
        /**
         * @brief  alpha33
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha33{0.0};
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_temperature{0.0};
    };
    /**
     * @brief  热膨胀各向异性数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-29
     */
    class ExpansionAnisotropicData
    {
    public:
        /**
         * @brief  获取Alpha11
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha11() const;
        /**
         * @brief  获取Alpha22
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha22() const;
        /**
         * @brief  获取Alpha33
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha33() const;
        /**
         * @brief  获取Alpha12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha12() const;
        /**
         * @brief  获取Alpha13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha13() const;
        /**
         * @brief  获取Alpha23
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getAlpha23() const;
        /**
         * @brief  获取温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getTemperature() const;
        /**
         * @brief  设置Alpha11
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha11(double value);
        /**
         * @brief  设置Alpha22
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha22(double value);
        /**
         * @brief  设置Alpha33
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha33(double value);
        /**
         * @brief  设置Alpha12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha12(double value);
        /**
         * @brief  设置Alpha13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha13(double value);
        /**
         * @brief  设置Alpha23
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setAlpha23(double value);
        /**
         * @brief  设置温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setTemperature(double value);
    private:
        /**
         * @brief  Alpha11
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha11{0.0};
        /**
         * @brief  Alpha22
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha22{0.0};
        /**
         * @brief  Alpha33
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha33{0.0};
        /**
         * @brief  Alpha12
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha12{0.0};
        /**
         * @brief  Alpha13
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha13{0.0};
        /**
         * @brief  Alpha23
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_alpha23{0.0};
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double m_temperature{0.0};
    };

}

#endif // FITKABAMATERIALEXPANSIONDATA_H
