/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialDensity.h
 * @brief  材料密度属性行为
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALDENSITY_H
#define FITKABAMATERIALDENSITY_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaMaterialBehavior.h"
#include <QVector>


namespace Interface {
    /**
     * @brief  材料密度类型。用于定义密度属性行为
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI  FITKAbaMaterialDensity: public FITKAbaMaterialBehavior
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaMaterialDensity() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaMaterialDensity() = default;
        /**
         * @brief  获取材料属性为类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType getMaterialBehaviorType() override;
        /**
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() override { return "Interface::FITKAbaMaterialDensity"; }
        /**
         * @brief  获取密度值
         * @param[in]  index 数据行索引（当考虑密度值随温度变化时，密度由多行数据表示）
         * @return double类型密度值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getMassDensity(int index = 0) const;
        /**
         * @brief  设置密度值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setMassDensity(double massDensity, int index = 0);
        /**
         * @brief  获取是否考虑随温度变化的状态
         * @param[in]  void
         * @return bool。true则随温度变化；false则不随温度变化
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool getUseTemperatureDependentData() const;
        /**
         * @brief  设置是否随温度变化。当设置false时，数据行数将只保留第一行数据
         * @param[in]  stat bool值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setUseTemperatureDependentData(bool stat);
        /**
         * @brief  获取温度值
         * @param[in]  index 数据行索引
         * @return double 类型温度值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getTemperature(int index = 0) const;
        /**
         * @brief  设置温度值
         * @param[in] temperature 温度值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setTemperature(double temperature, int index = 0);
        /**
         * @brief  获取数据行数
         * @param[in]  void
         * @return int类型行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        int getNumberOfRows() const;
        /**
         * @brief  设置数据行数。只有当设置随温度变化为true时有效
         * @param[in]  rows数据行数
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setNumberOfRows(int rows);
    private:
        /**
         * @brief  密度值数组。数组长度表示行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<double> m_massDensities{0.0,};
        /**
         * @brief  是否随温度变化
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool m_useTemperatureDependentData{false};
        /**
         * @brief  温度值数组。数组长度表示行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<double> m_temperatures{0.0,};
    };

}

#endif // FITKABAMATERIALDENSITY_H
