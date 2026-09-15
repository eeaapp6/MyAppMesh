/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialElasticData.h
 * @brief  各种弹性属性数据类，如各向同性数据、工程常数数据等。该文件中定义的类型不对外暴露接口。
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALELASTICDATA_H
#define FITKABAMATERIALELASTICDATA_H


namespace Interface {
    /**
     * @brief  弹性各向同性数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-29
     */
    class ElasticIsoTropicData
    {
    public:
        /**
         * @brief  获取杨氏模量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getYoungsModulus() const;
        /**
         * @brief  获取泊松比
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getPoissonRatio() const;
        /**
         * @brief  获取温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getTemperature() const;
        /**
         * @brief  设置杨氏模量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setYoungsModulus(double modulus);
        /**
         * @brief  设置泊松比
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setPoissonRatio(double ratio);
        /**
         * @brief  设置温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setTemperature(double temperature);
    private:
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double m_temperature{0.0};
        /**
         * @brief  杨氏模量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double m_youngsModulus{0.0};
        /**
         * @brief  泊松比
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double m_poissonRatio{0.0};
    };
    /**
     * @brief  弹性工程常数数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-29
     */
    class EngineeringConstantsData
    {
    public:
        /**
         * @brief  获取E1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getE1() const;
        /**
         * @brief  获取E2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getE2() const;
        /**
         * @brief  获取E3
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getE3() const;
        /**
         * @brief  获取Nu12
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getNu12() const;
        /**
         * @brief  获取Nu13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getNu13() const;
        /**
         * @brief  获取Nu23
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getNu23() const;
        /**
         * @brief  获取G12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getG12() const;
        /**
         * @brief  获取Nu13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getG13() const;
        /**
         * @brief  获取G23
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getG23() const;
        /**
         * @brief  获取温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getTemperature() const;
        /**
         * @brief  设置E1
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setE1(double e1);
        /**
         * @brief  设置E2
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setE2(double e2);
        /**
         * @brief  设置E3
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setE3(double e3);
        /**
         * @brief  设置Nu12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setNu12(double nu12);
        /**
         * @brief  设置Nu13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setNu13(double nu13);
        /**
         * @brief  设置Nu23
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setNu23(double nu23);
        /**
         * @brief  设置G12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setG12(double g12);
        /**
         * @brief  设置G13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setG13(double g13);
        /**
         * @brief  设置G23
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setG23(double g23);
        /**
         * @brief  设置温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setTemperature(double temperature);
    private:
        /**
         * @brief  E1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_E1{0.0};
        /**
         * @brief  E2
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_E2{0.0};
        /**
         * @brief  E3
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_E3{0.0};
        /**
         * @brief  Nu12
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_Nu12{0.0};
        /**
         * @brief  Nu13
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_Nu13{0.0};
        /**
         * @brief  Nu23
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_Nu23{0.0};
        /**
         * @brief  G12
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_G12{0.0};
        /**
         * @brief  G13
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_G13{0.0};
        /**
         * @brief  G23
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_G23{0.0};
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_temperature{0.0};
    };
    /**
     * @brief  弹性Lamina数据类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-29
     */
    class ElasticLaminaData
    {
    public:
        /**
         * @brief  获取E1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getE1() const;
        /**
         * @brief  获取E2
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getE2() const;
        /**
         * @brief  获取Nu12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getNu12() const;
        /**
         * @brief  获取G12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getG12() const;
        /**
         * @brief  获取G13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getG13() const;
        /**
         * @brief  获取G23
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getG23() const;
        /**
         * @brief  获取温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double getTemperature() const;
        /**
         * @brief  设置E1
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setE1(double value);
        /**
         * @brief  设置E2
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setE2(double value);
        /**
         * @brief  设置Nu12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setNu12(double value);
        /**
         * @brief  设置G12
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setG12(double value);
        /**
         * @brief  设置G13
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setG13(double value);
        /**
         * @brief  设置G23
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setG23(double value);
        /**
         * @brief  设置温度
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        void setTemperature(double value);

    private:
        /**
         * @brief  E1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_E1{0.0};
        /**
         * @brief  E2
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_E2{0.0};
        /**
         * @brief  Nu12
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_Nu12{0.0};
        /**
         * @brief  Nu12
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_G12{0.0};
        /**
         * @brief  G13
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_G13{0.0};
        /**
         * @brief  G23
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_G23{0.0};
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        double m_temperature{0.0};
    };

}

#endif // FITKABAMATERIALELASTICDATA_H
