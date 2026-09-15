/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKEquationOfStatePolynomial.h
 * @brief  线性多项式状态方程声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-28
 *********************************************************************/
#ifndef __FITK_EQUATIONOFSTATE_POLYNOMIAL_H__
#define __FITK_EQUATIONOFSTATE_POLYNOMIAL_H__

#include "FITKAbstractEquationOfState.h"

namespace Radioss
{
     /**
      * @brief   线性多项式状态方程声明
      * @details 该类用于描述Radioss中线性多项式状态方程，继承自FITKAbstractEquationOfState
      *          对应Radioss关键字/EOS/POLYNOMIAL，用于定义线性多项式状态方程
      * @author liuzhonghua (liuzhonghuaszch@163.com)
      * @date   2025-10-28
      */
    class FITKRadiossDataAPI FITKEquationOfStatePolynomial : public FITKAbstractEquationOfState
    {
        FITKCLASS(Radioss, FITKEquationOfStatePolynomial); //!< FITK类标识
        RadiossKeyWord(FITKEquationOfStatePolynomial, /EOS/POLYNOMIAL); //!< Radioss关键字标识
    public:
        explicit FITKEquationOfStatePolynomial();
        virtual ~FITKEquationOfStatePolynomial();
        
        virtual FITKAbstractEquationOfState::EOS_Type getEOSType()override;
        /**
         * @brief    Set Hydrodynamic pressure law coefficient
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setC0(double value);
        /**
         * @brief    Get Hydrodynamic pressure law coefficient
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getC0();
        /**
         * @brief    Set Hydrodynamic pressure law coefficient
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setC1(double value);
        /**
         * @brief    Get Hydrodynamic pressure law coefficient
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getC1();
        /**
         * @brief    Set Hydrodynamic pressure law coefficient
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setC2(double value);
        /**
         * @brief    Get Hydrodynamic pressure law coefficient
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getC2();
        /**
         * @brief    Set Hydrodynamic pressure law coefficient
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setC3(double value);
        /**
         * @brief    Get Hydrodynamic pressure law coefficient
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getC3();
        /**
         * @brief    Set Energy pressure law coefficient
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setC4(double value);
        /**
         * @brief    Get Energy pressure law coefficient
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getC4();
        /**
         * @brief    Set Energy pressure law coefficient
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setC5(double value);
        /**
         * @brief    Get Energy pressure law coefficient
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getC5();
        /**
         * @brief    Set Initial internal energy per unit reference volume
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setE0(double value);
        /**
         * @brief    Get Initial internal energy per unit reference volume
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getE0();
        /**
         * @brief    Set Pressure shift
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setPsh(double value);
        /**
         * @brief    Get Pressure shift
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getPsh();

    private:
        /**
         * @brief  	Hydrodynamic pressure law coefficient
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_c0{ 0.0 };
        /**
         * @brief  Hydrodynamic pressure law coefficient
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_c1{ 0.0 };
        /**
         * @brief  Hydrodynamic pressure law coefficient
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_c2{ 0.0 };
        /**
         * @brief  Hydrodynamic pressure law coefficient
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_c3{ 0.0 };
        /**
         * @brief  Energy pressure law coefficient
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_c4{ 0.0 };
        /**
         * @brief  Energy pressure law coefficient
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_c5{ 0.0 };
        /**
         * @brief  Initial internal energy per unit reference volume
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_e0{ 0.0 };
        /**
         * @brief  Pressure shift
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_psh{ 0.0 };
    };
}


#endif