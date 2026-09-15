/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorEOSPolynomial.h
 * @brief  多项式状态方程读取适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-10-29
 *********************************************************************/
#ifndef __FITKRadiossAdaptorEOSPolynomial_H__
#define __FITKRadiossAdaptorEOSPolynomial_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKEquationOfStatePolynomial;
    /**
     * @brief  多项式状态方程读取适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-10-24
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorEOSPolynomial : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorEOSPolynomial() = default;
        ~FITKRadiossAdaptorEOSPolynomial() = default;
        /**
         * @brief
         * @return   QString
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取信息
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readEOS(FITKEquationOfStatePolynomial* eos);
        /**
         * @brief    读取参数
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        void readParam(const QStringList& lines, Radioss::FITKEquationOfStatePolynomial* eos);

    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKEquationOfStatePolynomial, FITKRadiossAdaptorEOSPolynomial);
    RegRadiossRadAdaptor(/EOS/POLYNOMIAL, FITKRadiossAdaptorEOSPolynomial);

}
#endif //!__FITKRADIOSSADAPTORPROPSHELL_H__
