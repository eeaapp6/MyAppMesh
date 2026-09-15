/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITK_PROP_BEAM_SPR_H
#define FITK_PROP_BEAM_SPR_H

#include "FITKPropAbstract.hpp"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Radioss
{

    //维度枚举：123代表平移力相关，456代表扭转力相关
    enum DOF_SPR
    {
        None = 0,
        Tension_Compression = 1,    // 压缩拉伸
        Shear_XY,                   // XY平面剪切力
        Shear_XZ,                   // XZ平面剪切力
        Torsion = 4,                // 扭转
        Bending_Y,                  // y弯曲
        Bending_Z,                  // z弯曲
    };
    //梁单元中不同维度的属性值
    struct ValueDOF
    {
    public:
        ValueDOF(DOF_SPR Dof = None) { dof = Dof; }

        DOF_SPR dof = None;    //维度标志
        double K = 0.0;    // If fct_ID1 = 0: Linear loading and unloading stiffness.If fct_ID1 ≠ 0 : Only used as unloading stiffness for elasto-plasticsprings.
        double C = 0.0;    // 阻尼
        double A = 1.0;    //Nonlinear stiffness function scale factor
        double B = 0.0;    //Scale factor for logarithmic rate effects
        double D = 1.0;    //Scale factor for logarithmic rate effects

        int fct_ID1 = 0;   //Function identifier defining nonlinear stiffness f()
        int H = 0;         //Spring Hardening flag for nonlinear spring.
        int fct_ID2 = 0;   //Function identifier defining force or moment as a function ofspring velocity g()
        int fct_ID3 = 0;   //Function identifier
        int fct_ID4 = 0;   //Function identifier for nonlinear damping h()
        double min = -1e-30; //dof=1,2,3时对应δmin,  dof=4,5,6时对应θ‌min,
        double max = -1e-30; //dof=1,2,3时对应δmax,  dof=4,5,6时对应θ‌max,

        double F = 1.0;      //abscissa scale factor for the damping functions for the g() and h() 
        double E = 0.0;      //ordinate scale factor for the damping function g(). 
        double Ascale = 1.0; //Abscissa scale factor for the stiffness function f()
        double Hscale = 1.0; //Ordinate scale factor for the damping function h()

        double vc = 0.0;     //相对速度系数
        double n = 0.0;      //相对速度指数
        double a = 1.0;      //故障比例系数α
        double e = 2.0;      //指数β
    };
    /**
     * @brief 梁单元属性值类
     * @details 该类用于描述Radioss中梁单元的属性值信息，继承自FITKPropValueAbstract
     *          对应Radioss关键字/PROP/TYPE13（SPR_BEAM），用于定义梁单元的各种属性参数
     * @author libaojun
     * @date 2025-09-17
     */
    class FITKRadiossDataAPI FITKPropValueBeamSPR : public FITKPropValueAbstract,
        public FITKAbstractRadiossData
    {
    public:
        explicit FITKPropValueBeamSPR();
        virtual ~FITKPropValueBeamSPR() = default;

        /**
         * @brief 设置/获取质量
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setMass(double mass) { _mass = mass; }
        double getMass() { return _mass; }

        /**
         * @brief 设置/获取弹簧惯量
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setInertia(double inertia) { _inertia = inertia; }
        double getInertia() { return _inertia; }

        /**
        * @brief 设置/获取坐标ID
        * @author wangning (2185896382@qq.com)
        * @date 2025-09-18
        */
        void setSkewID(int skewID) { _skewID = skewID; }
        int getSkewID() { return _skewID; }

        /**
         * @brief 设置/获取传感器ID
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setSensID(int sensID) { _sensID = sensID; }
        int getSensID() { return _sensID; }

        /**
         * @brief 设置/获取传感器标志
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setIsflag(int Isflag) { _Isflag = Isflag; }
        int getIsflag() { return _Isflag; }

        /**
         * @brief 设置/获取失效标准
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setIfail(int Ifail) { _Ifail = Ifail; }
        int getIfail() { return _Ifail; }

        /**
         * @brief 设置/获取 输入每单位长度标志
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setIleng(int Ileng) { _Ileng = Ileng; }
        int getIleng() { return _Ileng; }

        /**
         * @brief 设置/获取 故障模型标志
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setIfail2(int Ifail2) { _Ifail2 = Ifail2; }
        int getIfail2() { return _Ifail2; }

        /**
        * @brief 设置/获取 参考速度
        * @author wangning (2185896382@qq.com)
        * @date 2025-09-18
        */
        void setV0(double v0) { _v0 = v0; }
        double getV0() { return _v0; }

        /**
        * @brief 设置/获取 参考角速度
        * @author wangning (2185896382@qq.com)
        * @date 2025-09-18
        */
        void setW0(double w0) { _w0 = w0; }
        double getW0() { return _w0; }

        /**
        * @brief 设置/获取 应变率切割频率
        * @author wangning (2185896382@qq.com)
        * @date 2025-09-18
        */
        void setFcut(double Fcut) { _Fcut = Fcut; }
        double getFcut() { return _Fcut; }

        /**
         * @brief 设置/获取 平滑应变率标志
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        void setFsmooth(int Fsmooth) { _Fsmooth = Fsmooth; }
        int getFsmooth() { return _Fsmooth; }

        /**
         * @brief 根据维度获取对应的属性值
         * @author wangning (2185896382@qq.com)
         * @date 2025-09-18
         */
        ValueDOF& getValueDOF(DOF_SPR dof);

    private:
        double _mass = 0.0;     //质量
        double _inertia = 0.0;  //弹簧惯性
        int _skewID = 0;        //坐标ID
        int _sensID = 0;        //传感器ID 0代表Spring is active
        int _Isflag = 0;        //传感器标志
        int _Ifail = 0;         //失效标准
        int _Ileng = 0;         //Input per unit length flag
        int _Ifail2 = 0;        //Failure model flag
        double _v0 = 1.0;       //参考速度
        double _w0 = 1.0;       //参考角速度
        double _Fcut = 1e30;    //应变率切割频率
        int _Fsmooth = 0;       //平滑应变率标志

        //梁单元中不同维度的属性值
        ValueDOF _TensionCompression;
        ValueDOF _ShearXY;
        ValueDOF _ShearXZ;
        ValueDOF _Torsion;
        ValueDOF _BendingY;
        ValueDOF _BendingZ;
    };

    /**
     * @brief 梁弹簧单元属性类
     * @details 该类用于描述Radioss中梁弹簧单元的属性信息，继承自FITKPropAbstract和FITKAbstractRadiossData
     *          对应Radioss关键字/PROP/TYPE13（SPR_BEAM），用于定义梁弹簧单元的各种属性参数
     * @author libaojun
     * @date 2025-09-17
     */
    class FITKRadiossDataAPI FITKPropBeamSPR : public FITKPropAbstract,
        public FITKAbstractRadiossData 
    {
        FITKCLASS(Radioss, FITKPropBeamSPR); //!< FITK类标识
        RadiossKeyWord(FITKPropBeamSPR,/PROP/TYPE13) //!< Radioss关键字标识
    public:
        explicit FITKPropBeamSPR();
        virtual ~FITKPropBeamSPR();

        //获取属性类型
        virtual FITKPropAbstract::RadPropType getRadPropType();

        FITKPropValueBeamSPR* getValue();

        //通过传入枚举获取对应字符串
        QString getStrDOF(DOF_SPR dof);

    private:
        Core::FITKAbstractObjectContainer<FITKPropValueBeamSPR> _value{}; //!< 属性值
    };





}

#endif