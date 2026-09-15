/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKNasMaterialMAT1.h
 * @brief NastranBDF 中定义的材料 MAT1
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-02-27
 * 
 */
#ifndef _FITKNASTRAN_MATERIAL_MAT1_H___
#define _FITKNASTRAN_MATERIAL_MAT1_H___
 
#include "FITKAbaMaterialBehavior.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractParam.hpp"

namespace Interface
{ 
    /**
     * @brief NastranBDF 中定义的材料 MAT1
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-02-27
     */
    class FITKInterfaceStructuralAPI FITKNasMaterialMAT1 : public FITKAbaMaterialBehavior
    {
    public:
        /**
         * @brief Construct a new FITKNasMaterialMAT1 object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        explicit FITKNasMaterialMAT1();
        /**
         * @brief Destroy the FITKNasMaterialMAT1 object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual ~FITKNasMaterialMAT1();

        /**
         * @brief  获取属性行为类型，便于对对象的动态类型转换
         * @param[in]  void
         * @return 属性行为枚举类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        virtual FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType getMaterialBehaviorType() override;
        /**
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() override {
            return "Interface::FITKNasMaterialMAT1";
        }

        /**
         * @brief Set the Youngs Modulus 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setYoungsModulus(const double & value);
        double getYoungsModulus() const;
        /**
         * @brief Set the Shear Modules object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setShearModules(const double& value);
        double getShearModules() const;
        /**
         * @brief Set the Poission Ratio 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setPoissionRatio(const double& value);
        double getPoissionRatio() const;
        /**
         * @brief Set the Density object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setDensity(const double& value);
        double getDensity() const;
        /**
         * @brief Set the Thermal Exp Coeff object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setThermalExpCoeff(const double& value);
        double getThermalExpCoeff() const;
       /**
        * @brief Set the Ref Temp object
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2025-02-27
        */
        void setRefTemp(const double& value);
        double getRefTemp() const;
        /**
         * @brief Set the Damping Coeff object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setDampingCoeff(const double& value);
        double getDampingCoeff() const;
         /**
         * @brief Set the Stress Limit Tension object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressLimitTension(const double & value);
        double getStressLimitTension() const;
        /**
         * @brief Set the Stress Limit Compress 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressLimitCompress(const double & value);
        double getStressLimitCompress() const;
        /**
         * @brief Set the Stress Limit Shear object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressLimitShear(const double & value);
        double getStressLimitShear() const;
        /**
         * @brief Set the Coordinate Sys object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setCoordinateSys(const int & id);
        int getCoordinateSys() const;
        /**
         * @brief 获取参数
         * @param[in]  param 参数名称 上面函数中set后面的字符串
         * @param[out] res   是否存在
         * @return 参数值
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */ 
        const Core::FITKAbstractParamValueDouble& getParam(const QString & param, bool& res);
        
    private:
        /**
         * @brief 杨氏模量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _youngsModulus{};
        /**
         * @brief 剪切模量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _shearModulus{};
        /**
         * @brief 泊松比
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _poissionRatio{};
        /**
         * @brief 密度
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _density{};
        /**
         * @brief 热膨胀系数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _thermalExpCoeff{};
        /**
         * @brief 参考温度
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _refTemp{};
        /**
         * @brief Structural element damping coefficien
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _dampingCoeff{};
        /**
         * @brief Stress limits for tension, compression, and shear 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _stessLimits[3];
        /**
         * @brief 坐标系ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValue<int>  _coodSysID;
    };
}


#endif
