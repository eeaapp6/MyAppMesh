/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPropSPH.h
 * @brief  SPH平滑粒子流体动力学 属性声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-28
 *********************************************************************/
#ifndef FITK_PROP_SPH_H
#define FITK_PROP_SPH_H

#include "FITKPropAbstract.hpp"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Radioss
{
    /**
     * @brief   SPH单元属性值类
     * @details 该类用于描述Radioss中SPH粒子单元的属性值信息，继承自FITKPropValueAbstract
     *          对应Radioss关键字/PROP/TYPE34（SPH），用于定义SPH粒子单元的各种属性参数
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     * @date    2025-10-23
     */
    class FITKRadiossDataAPI FITKPropSPHValue : public FITKPropValueAbstract,
        public FITKAbstractRadiossData
    {
    public:
        explicit FITKPropSPHValue() = default;
        virtual ~FITKPropSPHValue() = default;
        /**
         * @brief    Get Mass of the particles.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getMP() const;
        /**
         * @brief    Set Mass of the particles.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setMP(double value);
        /**
         * @brief    Get Quadratic bulk viscosity.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getQA() const;
        /**
         * @brief    Set Quadratic bulk viscosity.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setQA(double value);
        /**
         * @brief    Get Linear bulk viscosity.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getQB() const;
        /**
         * @brief    Set Linear bulk viscosity.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setQB(double value);
        /**
         * @brief    Get Conservative smoothing coefficient.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getACS() const;
        /**
         * @brief    Set Conservative smoothing coefficient.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setACS(double value);
        /**
         * @brief    Get skew_ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        int getSkewID() const;
        /**
         * @brief    Set skew_ID
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setSkewID(int value);
        /**
         * @brief    Get Smoothing length change based on volume.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        int getHID() const;
        /**
         * @brief    Set Smoothing length change based on volume.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setHID(int value);
        /**
         * @brief    Get SPH correction order.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        int getOrderID() const;
        /**
         * @brief    Set SPH correction order.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setOrderID(int value);
        /**
         * @brief    Get Smoothing length.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getH() const;
        /**
         * @brief    Set Smoothing length.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setH(double value);
        /**
         * @brief    Get Coefficient for solving tensile instability.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getStab() const;
        /**
         * @brief    Set Coefficient for solving tensile instability.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setStab(double value);

    private:
        /**
         * @brief  Mass of the particles.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_mp{ 0.0 };
        /**
         * @brief  Quadratic bulk viscosity.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_qa{ 2.0 };
        /**
         * @brief  Linear bulk viscosity.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_qb{ 1.0 };
        /**
         * @brief  Conservative smoothing coefficient.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_acs{ 0.0 };
        /**
         * @brief  Skew identifier to define the initial orthotropic directions in the case of skew_ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        int m_skew_ID{ 0 };
        /**
         * @brief  	Smoothing length change based on volume.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        int m_h_ID{ 0 };
        /**
         * @brief  	SPH correction order.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        int m_order{ 0 };
        /**
         * @brief  	Smoothing length.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_h{ 0.0 };
        /**
         * @brief  	Coefficient for solving tensile instability.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_stab{ 0.0 };
    };

    /**
     * @brief   SPH单元属性类
     * @details 该类用于描述Radioss中SPH粒子单元的属性信息，继承自FITKPropAbstract和FITKAbstractRadiossData
     *          对应Radioss关键字/PROP/TYPE34（SPR_BEAM），用于定义梁单元的各种属性参数
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     * @date    2025-10-23
     */
    class FITKRadiossDataAPI FITKPropSPH : public FITKPropAbstract,
        public FITKAbstractRadiossData 
    {
        FITKCLASS(Radioss, FITKPropSPH); //!< FITK类标识
        RadiossKeyWord(FITKPropSPH,/PROP/TYPE34) //!< Radioss关键字标识
    public:
        explicit FITKPropSPH();
        virtual ~FITKPropSPH();

        //获取属性类型
        virtual FITKPropAbstract::RadPropType getRadPropType();
        /**
         * @brief    获取属性
         * @return   FITKPropSPHValue *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-23
         */
        FITKPropSPHValue* getValue();

    private:
        /**
         * @brief  属性值
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-23
         */
        Core::FITKAbstractObjectContainer<FITKPropSPHValue> _value{}; //!< 属性值
    };





}

#endif