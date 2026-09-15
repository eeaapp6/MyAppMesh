/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_PROP_SHELL_H__
#define __FITK_PROP_SHELL_H__


#include "FITKPropAbstract.hpp"
#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Radioss 
{
    /**
     * @brief 壳单元属性值类
     * @details 此类用于描述壳单元的属性值信息，继承自FITKPropValueAbstract。
     *          对应Radioss关键字/PROP/TYPE1（SHELL），用于定义壳单元的各种属性参数。
     * @see FITKPropShell
     * @author LiBaojun (libaojunqd@foxmail.com)
     */
    class FITKRadiossDataAPI FITKPropShellValue : public FITKPropValueAbstract
    {
        FITKCLASS(Radioss, FITKPropShellValue); //!< FITK类标识
        RadiossKeyWord(FITKPropShellValue,/DEF_SHELL) //!< Radioss关键字标识
    public:
        explicit FITKPropShellValue() = default;
        virtual ~FITKPropShellValue() = default;
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getIshell() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setIshell(int value);
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getIsmstr() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setIsmstr(int value);
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getIthick() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setIthick(int value);
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getIplas() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setIplas(int value);
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getIsh3n() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setIsh3n(int value);
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getIdril() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setIdril(int value);
    private:
        /**
         * @brief  Shell element formulation flag
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int m_Ishell{ 0 };
        /**
         * @brief  Shell small strain formulation flag
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int m_Ismstr{ 0 };
        /**
         * @brief  Shell resultant stresses calculation flag
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int m_Ithick{ 0 };
        /**
         * @brief  Shell plane stress plasticity flag
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int m_Iplas{ 0 };
        /**
         * @brief  3-node shell element formulation flag
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int m_Ish3n{ 0 };
        /**
         * @brief  Drilling DOF stiffness flag
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int m_Idril{ 0 };
    };

    /**
     * @brief 壳单元属性类
     * @details 此类用于描述壳单元的属性，包括厚度等信息。
     * @see FITKAbstractSection, FITKAbstractRadiossData
     * @author LiBaojun (libaojun@foxmail.com)
     */
    class FITKRadiossDataAPI FITKPropShell : public FITKPropAbstract,
        public FITKAbstractRadiossData
    {
        FITKCLASS(Radioss, FITKPropShell); //!< FITK类标识
        RadiossKeyWord(FITKPropShell, /PROP/TYPE1); //!< Radioss关键字标识
        
    public:
        explicit FITKPropShell();
        virtual ~FITKPropShell();

        //获取属性类型
        virtual FITKPropAbstract::RadPropType getRadPropType();
        /**
         * @brief 获取壳单元属性值
         * @return FITKPropShellValue* 属性值指针
         * @details 此方法返回当前壳单元的属性值对象指针。
         *          属性值包含了壳单元的厚度等相关参数。
         */
        FITKPropShellValue* getValue();
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getIpinch() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setIpinch(int value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getPthickFail() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setPthickFail(double value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getHm() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setHm(double value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getHf() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setHf(double value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getHr() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setHr(double value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getDm() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setDm(double value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getDn() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setDn(double value);
        /**
         * @brief    
         * @return   int
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        int getN() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setN(int value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getThick() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setThick(double value);
        /**
         * @brief    
         * @return   double
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        double getAshear() const;
        /**
         * @brief    
         * @param[i] value 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-18
         */
        void setAshear(double value);
    private:
        /**
         * @brief  壳单元属性值对象指针
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        Core::FITKAbstractObjectContainer<FITKPropShellValue> _value{};
        /**
         * @brief  Pinching degree of freedom flag
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int    m_Ipinch{ 0 };
        /**
         * @brief  Ratio of through thickness integration points
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_PthickFail{ 0.0 };
        /**
         * @brief  Shell membrane hourglass coefficient
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_hm{ 0.01 };
        /**
         * @brief  Shell out-of-plane hourglass coefficient
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_hf{ 0.01 };
        /**
         * @brief  Shell rotation hourglass coefficient
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_hr{ 0.01 };
        /**
         * @brief  Shell membrane damping
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_dm{ 0.0 };
        /**
         * @brief  Shell numerical damping
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_dn{ 0.0001 };
        /**
         * @brief  Number of integration points through the thickness
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        int    m_N{ 0 };
        /**
         * @brief  Shell thickness
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_Thick{ 0.0 };
        /**
         * @brief  Shear factor (default is Reissner value 5/6)
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-18
         */
        double m_Ashear{ 5.0 / 6.0 };
    };

}

#endif // __FITK_PROP_SHELL_H__