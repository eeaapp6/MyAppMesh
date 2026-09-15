/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFailureModelBIQUAD.h
 * @brief  失效模型BIQUAD类声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-10
 *********************************************************************/
#ifndef FITK_FailureModel_BIQUAD_H
#define FITK_FailureModel_BIQUAD_H

#include "FITKAbstractFailureModel.h"

namespace Radioss
{
    /**
     * @brief  失效模型BIQUAD(/FAIL/BIQUAD)
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-11-10
     */
    class FITKRadiossDataAPI FITKFailureModelBIQUAD : public FITKAbstractFailureModel
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKFailureModelBIQUAD); //!< FITK类标识
        RadiossKeyWord(FITKFailureModelBIQUAD, /FAIL/BIQUAD) //!< Radioss关键字标识

    public:
        explicit FITKFailureModelBIQUAD();
        virtual ~FITKFailureModelBIQUAD();
        /**
         * @brief    获取失效模型类型
         * @return   FailureModelType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-10
         */
        FailureModelType getFailureModelType();

        // 设置单轴压缩失效塑性应变
        void setC1(const double value);
        // 获取单轴压缩失效塑性应变
        double getC1() const;
        // 设置剪切失效塑性应变
        void setC2(const double value);
        // 获取剪切失效塑性应变
        double getC2() const;
        // 设置单轴拉伸失效塑性应变
        void setC3(const double value);
        // 获取单轴拉伸失效塑性应变
        double getC3() const;
        // 设置平面应变拉伸失效塑性应变
        void setC4(const double value);
        // 获取平面应变拉伸失效塑性应变
        double getC4() const;
        // 设置双轴拉伸失效应变
        void setC5(const double value);
        // 获取双轴拉伸失效应变
        double getC5() const;
        // 设置壳单元厚度方向失效积分点比例
        void setPthickfail(const double value);
        // 获取壳单元厚度方向失效积分点比例
        double getPthickfail() const;
        // 设置材料选择标志
        void setMFlag(const int flag);
        // 获取材料选择标志
        int getMFlag() const;
        // 设置特定行为标志
        void setSFlag(const int flag);
        // 获取特定行为标志
        int getSFlag() const;
        // 设置局部缩颈失稳起始值
        void setInststart(const double value);
        // 获取局部缩颈失稳起始值
        double getInststart()const;
        // 设置单元尺寸因子函数编号
        void setFctIDel(const int id);
        // 获取单元尺寸因子函数编号
        int getFctIDel() const;
        // 设置参考单元尺寸
        void setElref(const double value);
        // 获取参考单元尺寸
        double getElref()const;
        // 设置 M-Flag=99 时的失效塑性应变比 R1
        void setR1(const double value);
        // 获取 M-Flag=99 时的失效塑性应变比 R1
        double getR1()const;
        // 设置 M-Flag=99 时的失效塑性应变比 R2
        void setR2(const double value);
        // 获取 M-Flag=99 时的失效塑性应变比 R2
        double getR2()const;
        // 设置 M-Flag=99 时的失效塑性应变比 R4
        void setR4(const double value);
        // 获取 M-Flag=99 时的失效塑性应变比 R4
        double getR4()const;
        // 设置 M-Flag=99 时的失效塑性应变比 R5
        void setR5(const double value);
        // 获取 M-Flag=99 时的失效塑性应变比 R5
        double getR5()const;

    private:
        /**
         * @brief  Failure plastic strain at uniaxial compression.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_c1{ 0.0 };
        /**
         * @brief  Failure plastic strain at shear.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_c2{ 0.0 };
        /**
         * @brief  Failure plastic strain in uniaxial tension.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_c3{ 0.0 };
        /**
         * @brief  Failure plastic strain at plain strain tension.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_c4{ 0.0 };
        /**
         * @brief  Failure strain at biaxial tension.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_c5{ 0.0 };
        /**
         * @brief  	Percentage of through thickness integration points that must fail before the element is deleted (shells only).
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_P_thickfail{ 1.0 };
        /**
         * @brief  Material selector flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        int m_M_Flag{ 0 };
        /**
         * @brief  Specific behavior flag. 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        int m_S_Flag{ 1 };
        /**
         * @brief  Instability start value for localized necking
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_Inst_start{ 0.0 };
        /**
         * @brief  	Element size factor function identifier
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        int m_fct_IDel{ -1 };
        /**
         * @brief  Reference element size.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_El_ref{ 1.0 };
        /**
         * @brief  Failure plastic strain ratio,  M-Flag=99
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_r1{ 0.0 };
        /**
         * @brief  Failure plastic strain ratio,  M-Flag=99
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_r2{ 0.0 };
        /**
         * @brief  Failure plastic strain ratio,  M-Flag=99
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_r4{ 0.0 };
        /**
         * @brief  Failure plastic strain ratio,  M-Flag=99
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-10
         */
        double m_r5{ 0.0 };
    };
    
} // namespace Radioss

#endif // FITK_ABSTRACT_BCS_H
