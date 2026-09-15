/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPropSandwichShell.h
 * @brief  夹层壳属性声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-27
 *********************************************************************/
#ifndef __FITK_PROP_SANDWICHSHELL_H__
#define __FITK_PROP_SANDWICHSHELL_H__


#include "FITKPropAbstract.hpp"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Radioss 
{
    /**
     * @brief  夹层的层数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-27
     */
    struct SandwichShellLayerValue
    {
        double _angle{ 0.0 };        ///< 层的角度
        double _thickness{ 0.0 };    ///< 层的厚度
        double _positionZ{ 0.0 };    ///< 层的中间的位置
        int _matID{ 0 };           ///< 层的材料ID
        double _relFWeight{ 0.0 };   ///< 层的失效权重因子
    };
    /**
     * @brief   夹层壳单元属性值类
     * @details 此类用于描述夹层壳的属性值信息，继承自FITKPropValueAbstract。
     *          对应Radioss关键字/PROP/TYPE11（SH_SANDW），用于定义夹层壳单元的各种属性参数。
     * @see     FITKPropSandwichShell
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     */
    class FITKRadiossDataAPI FITKPropSandwichShellValue : public FITKPropValueAbstract
    {
    public:
        explicit FITKPropSandwichShellValue() = default;
        virtual ~FITKPropSandwichShellValue() = default;

        /**
         * @brief    Get Shell element formulation flag.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        int getIshell() const;
        /**
         * @brief    Set Shell element formulation flag.
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        void setIshell(int value);
        /**
         * @brief    Get Shell small strain fommulation flag. 
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        int getIsmstr() const;
        /**
         * @brief    Set Shell small strain fommulation flag.
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        void setIsmstr(int value);
        /**
         * @brief    Get 3 node shell element formulation flag.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        int getIsh3n() const;
        /**
         * @brief    Set 3 node shell element formulation flag.
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        void setIsh3n(int value);
        /**
         * @brief    Get Drilling DOF stiffness flag
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        int getIdril() const;
        /**
         * @brief    Set Drilling DOF stiffness flag
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        void setIdril(int value);
        /**
         * @brief    Get Percentage of layer thickness that must fail before the element isdeleted.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getPThickfail() const;
        /**
         * @brief    Set Percentage of layer thickness that must fail before the element isdeleted.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setPThickfail(double value);
        /**
         * @brief    Get Shell membrane hourglass coefficient.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getHm() const;
        /**
         * @brief    Set Shell membrane hourglass coefficient.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setHm(double value);
        /**
         * @brief    Get Shell out-of-plane hourglass.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getHf() const;
        /**
         * @brief    Set Shell out-of-plane hourglass.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setHf(double value);
        /**
         * @brief    Get Shell rotation hourglass coefficient.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getHr() const;
        /**
         * @brief    Set Shell rotation hourglass coefficient.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setHr(double value);
        /**
         * @brief    Get Shell Membrane Damping.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getDm() const;
        /**
         * @brief    Set Shell Membrane Damping.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setDm(double value);
        /**
         * @brief    Get Shell numerical damping.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getDn() const;
        /**
         * @brief    Set Shell numerical damping.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setDn(double value);
        /**
         * @brief    Get Number of layers.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        int getN() const;
        /**
         * @brief    Set Number of layers.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setN(int value);
        /**
         * @brief    Get Compute strains for post-processing flag.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        int getIstrain() const;
        /**
         * @brief    Set Compute strains for post-processing flag.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setIstrain(int value);
        /**
        * @brief    Set Shell thickness.
        * @author   liuzhonghua (liuzhonghuaszch@163.com)
        * @date     2025-10-27
        */
        void setThick(double thick);
        /**
         * @brief    Get Shell thickness.
         * @param[i] isAllLayer 是否获取各夹层累计厚度
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getThick(bool isAllLayer=false);
        /**
         * @brief    Get Shear factor.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getAshear() const;
        /**
         * @brief    Set Shear factor.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setAshear(double value);
        /**
         * @brief    Get Shell resultant stresses calculation flag.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        int getIthick() const;
        /**
         * @brief    Set Shell resultant stresses calculation flag.
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        void setIthick(int value);
        /**
         * @brief    Get Shell plane stress plasticity flag.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        int getIplas() const;
        /**
         * @brief    Set Shell plane stress plasticity flag.
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-18
         */
        void setIplas(int value);
        /**
         * @brief    Get X component for reference vector.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getVx() const;
        /**
         * @brief    Set X component for reference vector.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setVx(double value);
        /**
         * @brief    Get Y component for reference vector.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getVy() const;
        /**
         * @brief    Set Y component for reference vector.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setVy(double value);
        /**
         * @brief    Get Z component for reference vector.
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        double getVz() const;
        /**
         * @brief    Set Z component for reference vector.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setVz(double value);
        /**
         * @brief    Get Skew identifier for reference vector. 
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        int getSkewID() const;
        /**
         * @brief    Set Skew identifier for reference vector. 
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setSkewID(int value);
        /**
         * @brief    Get Orthotropic system formulation flag for reference vector.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        int getIorth() const;
        /**
         * @brief    Set Orthotropic system formulation flag for reference vector.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setIorth(int value);
        /**
         * @brief    Get Layer positioning flag for reference vector.
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        bool getIpos() const;
        /**
         * @brief    Set Layer positioning flag for reference vector.
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setIpos(bool value);
        /**
         * @brief    Get Value for Layer i.
         * @param[i] index 
         * @return   SandwichShellLayerValue
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        SandwichShellLayerValue getLayerValue(int index) const;
        /**
         * @brief    Set Value for Layer i.
         * @param[i] v 
         * @param[i] index 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-27
         */
        void setLayerValue(SandwichShellLayerValue& v, int index);

    private:
        /**
         * @brief  Shell element formulation flag.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Ishell{ 0 };
        /**
         * @brief  Shell small strain fommulation flag. 
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Ismstr{ 0 };
        /**
         * @brief  3 node shell element formulation flag.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Ish3n{ 0 };
        /**
         * @brief  Drilling DOF stiffness flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Idril{ 0 };
        /**
         * @brief  Percentage of layer thickness that must fail before the element isdeleted.
         *         0.0≤m_P_thickfail≤ 1.0
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_P_thickfail{ 0.0 };
        /**
         * @brief  Shell membrane hourglass coefficient.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_hm{ 0.1 };
        /**
         * @brief  Shell out-of-plane hourglass.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_hf{ 0.1 };
        /**
         * @brief  Shell rotation hourglass coefficient.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_hr{ 0.1 };
        /**
         * @brief  Shell Membrane Damping.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_dm{ 0.0 };
        /**
         * @brief  Shell numerical Damping.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_dn{ 0.0 };
        /**
         * @brief  compute strains for post-processing flag.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Istrain{ 0 };
        /**
        * @brief  compute strains for post-processing flag.
        * @author liuzhonghua (liuzhonghuaszch@163.com)
        * @date   2025-10-27
        */
        double m_Thick{ 0 };
        /**
         * @brief  Shear factor.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_Ashear{ 0.0 };
        /**
         * @brief  Shell resultant stresses calculation flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Ithick{ 0 };
        /**
         * @brief  Shell plane stress plasticity flag
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Iplas{ 0 };
        /**
         * @brief  X component for reference vector.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_vx{ 1.0 };
        /**
         * @brief  Y component for reference vector.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_vy{ 1.0 };
        /**
         * @brief  Z component for reference vector.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        double m_vz{ 1.0 };
        /**
         * @brief  Skew identifier for reference vector.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_skew_id{ 0 };
        /**
         * @brief  Orthotropic system formulation flag for reference vector.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Iorth{ 0 };
        /**
         * @brief  Layer positioning flag for reference vector.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        bool m_Ipos{ false };
        /**
         * @brief  Reference direction in shell plane.
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        int m_Ip{ 0 };
        /**
         * @brief  层的数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-27
         */
        QVector<SandwichShellLayerValue> _layerValues{ SandwichShellLayerValue() };
    };

    /**
     * @brief  夹层壳属性
     * @details 该类用于描述Radioss中夹层壳单元的属性信息，继承自FITKPropAbstract和FITKAbstractRadiossData
     *          对应Radioss关键字/PROP/TYPE11（SH_SANDW），用于定义夹层壳单元的各种属性参数.
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-27
     */
    class FITKRadiossDataAPI FITKPropSandwichShell : public FITKPropAbstract,
        public FITKAbstractRadiossData
    {
        FITKCLASS(Radioss, FITKPropSandwichShell); //!< FITK类标识
        RadiossKeyWord(FITKPropSandwichShell, /PROP/TYPE11); //!< Radioss关键字标识
        
    public:
        explicit FITKPropSandwichShell();
        virtual ~FITKPropSandwichShell();

        //获取属性类型
        virtual FITKPropAbstract::RadPropType getRadPropType();
        /**
         * @brief 获取壳单元属性值
         * @return FITKPropSandwichShellValue* 属性值指针
         * @details 此方法返回当前壳单元的属性值对象指针。
         *          属性值包含了壳单元的厚度等相关参数。
         */
        FITKPropSandwichShellValue* getValue();
    private:
        /**
         * @brief  夹层壳单元属性值对象指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-07-18
         */
        Core::FITKAbstractObjectContainer<FITKPropSandwichShellValue> _value{};
    };

}

#endif // __FITK_PROP_SHELL_H__