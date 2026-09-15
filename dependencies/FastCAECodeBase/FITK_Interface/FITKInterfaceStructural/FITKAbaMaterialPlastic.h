/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialPlastic.h
 * @brief  材料塑性属性行为
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALPLASTIC_H
#define FITKABAMATERIALPLASTIC_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaMaterialBehavior.h"
#include "FITKAbaMaterialPlasticData.h"
#include <QVector>


namespace Interface {

    /**
     * @brief  材料塑性行为类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialPlastic: public FITKAbaMaterialBehavior
    {
    public:
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
            return "Interface::FITKAbaMaterialPlastic";
        }
        /**
         * @brief  获取塑性属性类型。如各向同性、JohnsonCook等
         * @param[in] 
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-21
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialPlasticType getPlasticType() const;
        /**
         * @brief  设置塑性属性类型
         * @param[in] type 塑性属性类型枚举类型值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setPlasticType(FITKAbaMaterialBehaviorEnum::FITKAbaMaterialPlasticType type);
        /**
         * @brief  获取参数是否随温度变化
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        bool getUseTemperatureDependentData() const;
        /**
         * @brief  设置是否使用温度相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setUseTemperatureDependentData(bool stat);
        /**
         * @brief  是否使用应变率相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        bool getUseStrainRateDependentData() const;
        /**
         * @brief  设置是否使用应变率相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setUseStrainRateDependentData(bool stat);
        /**
         * @brief  获取JohnsonCook属性数据A
         * @param[in]  void
         * @return double型A值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getJohnsonCookA() const;
        /**
         * @brief  获取JohnsonCook属性数据B
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getJohnsonCookB() const;
        /**
         * @brief  获取JohnsonCook属性数据n
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getJohnsonCookn() const;
        /**
         * @brief  获取JohnsonCook属性数据m
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getJohnsonCookm() const;
        /**
         * @brief  获取JohnsonCook属性数据融化温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getJohnsonCookMeltingTemp() const;
        /**
         * @brief  获取JohnsonCook属性数据过渡温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getJohnsonCookTransitionTemp() const;
        /**
         * @brief  设置JohsonCook属性数据A
         * @param[in] 数值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setJohnsonCookA(double value);
        /**
         * @brief  设置JohnsonCook属性数据B
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setJohnsonCookB(double value);
        /**
         * @brief  设置JohnsonCook属性数据m
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setJohnsonCookm(double value);
        /**
         * @brief  设置JohnsonCook属性数据n
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setJohnsonCookn(double value);
        /**
         * @brief  设置JohnsonCook属性数据融化温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setJohnsonCookMeltingTemp(double value);
        /**
         * @brief  设置JohnsonCook属性数据过渡温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setJohnsonCookTransitionTemp(double value);
        /**
         * @brief  获取各向同性数据行数
         * @param[in]  void
         * @return int型行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        int getNumberOfIsotropicDataRows() const;
        /**
         * @brief  设置各向同性塑性数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setNumberOfIsoTropicDataRows( int rows);
        /**
         * @brief  插入各向同性塑性数据行数
         * @param[in]  index 要插入的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void insertIsotropicDataRow(int index);
        /**
         * @brief  移除各向同性数据行
         * @param[in]  要移除的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void removeIsoTropicDataRow(int index);
        /**
         * @brief  获取各向同性屈服应力
         * @param[in] index 数据行索引
         * @return double型屈服应力
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getIsotropicYieldStress(int index = 0);
        /**
         * @brief  设置各向同性屈服应力
         * @param[in] value 屈服应力值
         * @param[in] index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsotropicYieldStess(double value, int index = 0);
        /**
         * @brief  获取各向同性塑性属性数据应变
         * @param[in]  index 要获取的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getIsotropicPlasticStrain(int index = 0);
        /**
         * @brief  设置各向同性塑性属性数据应变
         * @param[in]  value 数据
         * @param[in]  index 要设置数据的行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setIsotropicPlasticStrain(double value, int index = 0);
        /**
         * @brief  获取各向同性塑性属性数据应变率
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getIsotropicRate(int index = 0);
        /**
         * @brief  设置各向同性塑性属性数据应变率
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setIsotropicRate(double value, int index = 0);
        /**
         * @brief  获取各向同性属性数据温度
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        double getIsotropicTemperature(int index = 0);
        /**
         * @brief  设置各向同性属性数据温度
         * @param[in]  value 温度值
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-29
         */
        void setIsotropicTemperature(double value, int index = 0);

    private:
        /**
         * @brief  塑性属性类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialPlasticType m_plasticType{ FITKAbaMaterialBehaviorEnum::FITKAbaMaterialPlasticType::PlasticIsoTropic};
        /**
         * @brief  属性是否随温度变化
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool m_useTemperatureDependentData{false};
        /**
         * @brief  是否使用应变率相关数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool m_useStrainRateDependentData{false};
        /**
         * @brief  各向同性数据数组。数组长度对应数据行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<PlasticIsotropic> m_isotropicDatas{PlasticIsotropic(),};
        /**
         * @brief  JohsonCook塑性属性数据。该数据不存在多行的情况
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        PlasticJohnsonCook m_johnsonCook;
    };
}


#endif // FITKABAMATERIALPLASTIC_H
