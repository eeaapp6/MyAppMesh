/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFailureModelJOHNSON.h
 * @brief  失效模型JOHNSON
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-10
 *********************************************************************/
#ifndef FITK_FailureModel_JOHNSON_H
#define FITK_FailureModel_JOHNSON_H

#include "FITKAbstractFailureModel.h"

namespace Radioss
{
    /**
     * @brief  失效模型JOHNSON(/FAIL/JOHNSON)
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-11-10
     */
    class FITKRadiossDataAPI FITKFailureModelJOHNSON : public FITKAbstractFailureModel
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKFailureModelJOHNSON); //!< FITK类标识
        RadiossKeyWord(FITKFailureModelJOHNSON, /FAIL/JOHNSON) //!< Radioss关键字标识
    public:
        explicit FITKFailureModelJOHNSON();
        virtual ~FITKFailureModelJOHNSON();
        /**
         * @brief    获取失效模型类型
         * @return   FailureModelType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-10
         */
        FailureModelType getFailureModelType();

        // 设置参数 D1
        void setD1(const double value);
        // 获取参数 D1
        double getD1() const;
        // 设置参数 D2
        void setD2(const double value);
        // 获取参数 D2
        double getD2() const;
        // 设置参数 D3
        void setD3(const double value);
        // 获取参数 D3
        double getD3() const;
        // 设置参数 D4
        void setD4(const double value);
        // 获取参数 D4
        double getD4() const;
        // 设置参数 D5
        void setD5(const double value);
        // 获取参数 D5
        double getD5() const;
        // 设置参考应变率 Eps0
        void setEps0(const double value);
        // 获取参考应变率 Eps0
        double getEps0() const;
        // 设置壳失效标志 Ifailsh
        void setIfailsh(const int flag);
        // 获取壳失效标志 Ifailsh
        int getIfailsh() const;
        // 设置实体失效标志 Ifailso
        void setIfailso(const int flag);
        // 获取实体失效标志 Ifailso
        int getIfailso() const;
        // 设置裂纹扩展判据 Dadv
        void setDadv(const double value);
        // 获取裂纹扩展判据 Dadv
        double getDadv() const;
        // 设置 XFEM 标志 Ixfem
        void setIxfem(const int flag);
        // 获取 XFEM 标志 Ixfem
        int getIxfem() const;

    private:
        /**
         * @brief  1th parameter
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_d1{ 0.0 };
        /**
         * @brief  2th parameter
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_d2{ 0.0 };
        /**
         * @brief  3th parameter
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_d3{ 0.0 };
        /**
         * @brief  4th parameter
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_d4{ 0.0 };
        /**
         * @brief  5th parameter
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_d5{ 0.0 };
        /**
         * @brief  Shell failure flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_eps_0{ 0.0 };
        /**
         * @brief  Shell failure flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        int m_Ifail_sh{ 1 };
        /**
         * @brief  Solid failure flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        int m_Ifail_so{ 1 };
        /**
         * @brief  Criterion for the crack advancement (Only active if with Ixfem =1). between 0 and 1
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_Dadv{ 1 };
        /**
         * @brief  	XFEM flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        int m_Ixfem{ 0 };
    };
    
} // namespace Radioss

#endif // FITK_ABSTRACT_BCS_H
