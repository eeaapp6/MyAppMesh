/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK__RAD_PROP_SOLID_H__
#define __FITK__RAD_PROP_SOLID_H__

#include "FITKPropAbstract.hpp"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace  Radioss
{
    /**
     * @brief 实体单元属性值类
     * @details 该类用于描述Radioss中实体单元的属性值信息，继承自FITKPropValueAbstract
     *          对应Radioss关键字/PROP/TYPE14（SOLID），用于定义实体单元的各种属性参数
     * @author libaojun
     * @date 2025-04-25
     */
    class FITKRadiossDataAPI FITKPropSolidValue : public FITKPropValueAbstract,
        public FITKAbstractRadiossData
    {
        FITKCLASS(Radioss, FITKPropSolidValue); //!< FITK类标识
        RadiossKeyWord(FITKPropSolidValue,/DEF_SOLID) //!< Radioss关键字标识
    public:
        explicit FITKPropSolidValue() = default;
        virtual ~FITKPropSolidValue() = default;

        // Getter and Setter declarations
        int getIsolid() const;
        void setIsolid(int val);

        int getIsmstr() const;
        void setIsmstr(int val);

        int getIcpre() const;
        void setIcpre(int val);

        int getItetra4() const;
        void setItetra4(int val);

        int getItetra10() const;
        void setItetra10(int val);

        int getImas() const;
        void setImas(int val);

        int getIFrame() const;
        void setIFrame(int val);

    private:
        int m_Isolid{0};      //!< solid element formulation flag
        int m_Ismstr{4};      //!< small strain formulation flag
        int m_Icpre{3};       //!< contact pressure formulation flag
        int m_Itetra4{0};     //!< 4 nodes tetrahedral element formulation flag
        int m_Itetra10{0};    //!< 10 nodes tetrahedral element formulation flag
        int m_Imas{2};        //!< nodal mass distribution (per element) flag
        int m_IFrame{1};      //!< element coordinate system formulation flag
    };

    /**
     * @brief 实体单元属性类
     * @details 该类用于描述Radioss中实体单元的属性信息，继承自FITKPropAbstract和FITKAbstractRadiossData
     *          对应Radioss关键字/PROP/TYPE14（SOLID），用于定义实体单元的各种属性参数
     * @author libaojun
     * @date 2025-04-25
     */
    class FITKRadiossDataAPI FITKPropSolid : public FITKPropAbstract,
        public FITKAbstractRadiossData 
    {
        FITKCLASS(Radioss, FITKPropSolid); //!< FITK类标识
        RadiossKeyWord(FITKPropSolid,/PROP/TYPE14) //!< Radioss关键字标识
    public:
        /**
         * @brief 默认构造函数 
         * @author libaojun  
         * @date 2025-04-25
         */
        explicit FITKPropSolid();

        /**
         * @brief 虚析构函数
         * @author libaojun
         * @date 2025-04-25
         */
        virtual ~FITKPropSolid();

        //获取属性类型
        virtual FITKPropAbstract::RadPropType getRadPropType();

        /**
         * @brief 获取属性值
         * @return FITKPropSolidValue* 属性值指针
         * @author libaojun
         * @date 2025-04-25
         */
        FITKPropSolidValue* getValue();
       

        // Getter and Setter declarations
        double getDn() const;
        void setDn(double val);

        double getQa() const;
        void setQa(double val);

        double getQb() const;
        void setQb(double val);

        double getH() const;
        void setH(double val);

        double getLambdaV() const;
        void setLambdaV(double val);

        double getMuV() const;
        void setMuV(double val);

        double getMinTimeStep() const;
        void setMinTimeStep(double val);

        int getNdir() const;
        void setNdir(int val);

        int getSPHPartID() const;
        void setSPHPartID(int val);

    private:

        Core::FITKAbstractObjectContainer<FITKPropSolidValue> _value{}; //!< 属性值

        // Radioss /PROP/TYPE14 (SOLID) 参数补充
        double  m_dn{0.0};        //!< numical damping for stabilization
        double  m_qa{1.10};       //!< quadratic bulk viscosity
        double  m_qb{0.0};        //!< linear bulk viscosity
        double  m_h{0.10};        //!< hourglass viscosity coefficient
        double  m_lambdaV{0.0};       //!< Numerical Navier Stokes viscosity λv
        double  m_muV{0.0};       //!< Numerical Navier Stokes viscosity μv
        double  m_minTimeStep{0.0};      //!< minimum time step for solid elements
        int     m_Ndir{1};      //!< Number of particle/direction for each solid element
        int     m_SPHPartID{0};      //!< SPH particle ID for solid elements
    };

} // namespace  Radioss

#endif