/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialElastic.h
 * @brief  材料弹性属性行为。
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALELASTIC_H
#define FITKABAMATERIALELASTIC_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaMaterialBehavior.h"
#include "FITKAbaMaterialElasticData.h"
#include <QVector>


namespace Interface {
   
    /**
     * @brief  材料弹性属性行为类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialElastic: public FITKAbaMaterialBehavior
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaMaterialElastic() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaMaterialElastic() = default;
        /**
         * @brief  获取材料行为类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType getMaterialBehaviorType() override;
        /**
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() override { 
            return "Interface::FITKAbaMaterialElastic"; }

        /**
         * @brief  获取弹性属性类型。如各向异性、各向同性等
         * @param[in]  void
         * @return 材料弹性属性类型枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialElasticType getMaterialElasticType() const;
        /**
         * @brief  设置弹性属性类型。如各项同性、各项异性等
         * @param[in]  材料弹性属性类型枚举类型值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setMaterialElasticType(FITKAbaMaterialBehaviorEnum::FITKAbaMaterialElasticType type);
        /**
         * @brief  获取属性值是否随温度变化
         * @param[in]  void
         * @return bool值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool getUseTemperatureDependentData() const;
        /**
         * @brief  设置属性值是否随温度变化
         * @param[in] stat bool值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setUseTemperatureDependentData(bool stat);
        /**
         * @brief  获取材料不可压缩状态
         * @param[in]  void
         * @return bool值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool getNoCompression() const;
        /**
         * @brief  设置材料不可压缩状态
         * @param[in] stat bool值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setNoCompression(bool stat);
        /**
         * @brief  获取材料不可拉伸状态
         * @param[in]  void
         * @return bool值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool getNoTension() const;
        /**
         * @brief  设置材料不可拉伸状态
         * @param[in] stat  bool值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setNoTension(bool stat);
        /**
         * @brief  获取各向同性数据行数（当设置随温度变化时，数据为多行）
         * @param[in]  void
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        int getIsotropicNumberOfRows() const;
        /**
         * @brief  设置各向同性数据行数
         * @param[in] rows 行数  
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsotropicNumberOfRows(int rows);
        /**
         * @brief  插入各向同性数据行
         * @param[in] index插入行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void insertIsotropicRow(int index);
        /**
         * @brief  移除各向同性数据行
         * @param[in] index数据行索引  
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void removeIsoTropicRow(int index);
        /**
         * @brief  获取各向同性数据温度值
         * @param[in] index数据行索引 
         * @return double型温度值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getIsotropicTemperature(int index = 0) const;
        /**
         * @brief  设置各向同性数据温度值
         * @param[in] temperature 温度值
         * @param[in] index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsotropicTemperature(double temperature, int index = 0);
        /**
         * @brief  获取各向同性杨氏模量
         * @param[in] index 数据行索引  
         * @return double型杨氏模量值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getIsotropicYoungsModulus(int index = 0) const;
        /**
         * @brief  设置各向同性杨氏模量
         * @param[in] moudulus 杨氏模量值 
         * @param[in] index数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsotropicYoungsModulus(double moudulus, int index = 0);
        /**
         * @brief  获取各向同性泊松比
         * @param[in] index 数据行索引  
         * @return double型泊松比
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getIsotropicPoisssonRatio(int index = 0) const;
        /**
         * @brief  设置各向同性泊松比
         * @param[in] poisson 泊松比
         * @param[in] index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsotropicPoissonRatio(double poisson, int index = 0);
        /**
         * @brief  获取工程常数数据行数
         * @param[in]  void
         * @return int型行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        int getEngineeringConstantsNumberOfRows() const;
        /**
         * @brief  设置工程常数数据行数，只有当设置随温度变化为true时有效
         * @param[in] rows 行数  
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setEngineeringConstantsNumberOfRows(int rows);
        /**
         * @brief  插入工程常数数据行
         * @param[in] index 要插入的行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void insertEngineeringConstantsRow(int index);
        /**
         * @brief  移除工程常数数据行
         * @param[in] index 要移除的数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void removeEngineeringConstantsRow(int index);
        /**
         * @brief  获取工程常数数值E1
         * @param[in] index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getEngineeringConstantsE1(int index = 0) const;
        /**
         * @brief  获取工程常数数值E2
         * @param[in] index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getEngineeringConstantsE2(int index = 0) const;
        /**
         * @brief  获取工程常数数值E3
         * @param[in] index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getEngineeringConstantsE3(int index = 0) const;
        /**
         * @brief  获取工程常数数值Nu12
         * @param[in] index 数据行索引  
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getEngineeringConstantsNu12(int index = 0) const;
        /**
         * @brief  获取工程常数数值Nu13
         * @param[in] index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getEngineeringConstantsNu13(int index = 0) const;
        /**
         * @brief  获取工程常数数值Nu23
         * @param[in]  index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getEngineeringConstantsNu23(int index = 0) const;
        /**
         * @brief  获取工程常数数值G12
         * @param[in]  index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getEngineeringConstantsG12(int index = 0) const;
        /**
         * @brief  获取工程常数数值G13
         * @param[in]  index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getEngineeringConstantsG13(int index = 0) const;
        /**
         * @brief  获取工程常数数值G23
         * @param[in]  index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getEngineeringConstantsG23(int index = 0) const;
        /**
         * @brief  获取工程常数温度数值
         * @param[in]  index 数据行索引
         * @return double型温度值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getEngineeringConstantsTemperature(int index = 0) const;
        /**
         * @brief  设置工程常数数值E1
         * @param[in] value 数值
         * @param[in] index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setEngineeringConstantsE1(double value, int index = 0);
        /**
         * @brief  设置工程常数数值E2
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsE2(double value, int index = 0);
        /**
         * @brief  设置工程常数数值E3
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsE3(double value, int index = 0);
        /**
         * @brief  设置工程常数数值Nu12
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsNu12(double value, int index = 0);
        /**
         * @brief  设置工程常数数值Nu13
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsNu13(double value, int index = 0);
        /**
         * @brief  设置工程常数数值Nu23
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsNu23(double value, int index = 0);
        /**
         * @brief  设置工程常数数值G12
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsG12(double value, int index = 0);
        /**
         * @brief  设置工程常数数值G13
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsG13(double value, int index = 0);
        /**
         * @brief  设置工程常数数值G23
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsG23(double value, int index = 0);
        /**
         * @brief  设置工程常数温度值
         * @param[in]  value 数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setEngineeringConstantsTemperature(double value, int index = 0);
        /**
         * @brief  获取Lamina数据行数
         * @param[in]  void
         * @return int型数据行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        int getLaminaNumberOfRows() const;
        /**
         * @brief  设置Lamina数据行数
         * @param[in]  rows 数据行数
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setLaminaNumberOfRows(int rows);
        /**
         * @brief  插入Lamina数据行
         * @param[in]  index 要插入的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void insertLaminaRow(int index);
        /**
         * @brief  移除Lamina数据行
         * @param[in]  index 要移除的数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void removeLaminaRow(int index);
        /**
         * @brief  获取Lamina参数E1
         * @param[in]  index 数据行索引
         * @return double型参数数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getLaminaE1(int index = 0) const;
        /**
         * @brief  获取Lamina参数E2
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getLaminaE2(int index = 0) const;
        /**
         * @brief  获取Lamina参数Nu12
         * @param[in]  index 数据行索引
         * @return double型参数数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getLaminaNu12(int index = 0) const;
        /**
         * @brief  获取Lamina参数G12
         * @param[in]  index 数据行索引
         * @return double型参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getLaminaG12(int index = 0) const;
        /**
         * @brief  获取Lamina参数G13
         * @param[in]  index 数据行索引
         * @return double型参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getLaminaG13(int index = 0) const;
        /**
         * @brief  获取Lamina参数G23
         * @param[in]  index 数据行索引
         * @return double型参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getLaminaG23(int index = 0) const;
        /**
         * @brief  获取Lamina参数温度
         * @param[in]  index数据行索引
         * @return double型参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getLaminaTemperature(int index = 0) const;
        /**
         * @brief  设置Lamina参数E1
         * @param[in]  value 参数值
         * @param[in]  数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setLaminaE1(double value, int index = 0);
        /**
         * @brief  设置Lamina参数E2
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setLaminaE2(double value, int index = 0);
        /**
         * @brief  设置Lamina参数Nu12
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setLaminaNu12(double value, int index = 0);
        /**
         * @brief  设置Lamina参数G12
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setLaminaG12(double value, int index = 0);
        /**
         * @brief  设置Lamina参数G13
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setLaminaG13(double value, int index = 0);
        /**
         * @brief  设置Lamina参数G23
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setLaminaG23(double value, int index = 0);
        /**
         * @brief  设置Lamina参数温度
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setTemperature(double value, int index = 0);


    private:
        /**
         * @brief  弹性行为类型，如各向同性、工程常数、各向异性等
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialElasticType m_elasticType{ FITKAbaMaterialBehaviorEnum::FITKAbaMaterialElasticType::IsoTropic};
        /**
         * @brief  是否使用温度相关的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool m_useTemperatureDependentData{false};
        /**
         * @brief  不可压缩状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool m_noCompression{false};
        /**
         * @brief  不可拉伸状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool m_noTension{false};
        /**
         * @brief  各向同性数据数组，数组长度表示行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<ElasticIsoTropicData> m_isotropicDatas{ElasticIsoTropicData(),};
        /**
         * @brief  工程常数数据数组，数组长度表示行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<EngineeringConstantsData> m_engineeringConstantsDatas{EngineeringConstantsData(),};
        /**
         * @brief  Lamina属性数据数组，数组长度表示行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        QVector<ElasticLaminaData> m_laminaDatas{ElasticLaminaData(),};
    };
}


#endif // FITKABAMATERIALELASTIC_H
