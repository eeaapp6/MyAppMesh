/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInertiaPointMassInertia.h
 * @brief  集中质量、惯量类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-05-10
 * 
 */
#ifndef FITKABAINERTIAPOINTMASSINERTIA_H
#define FITKABAINERTIAPOINTMASSINERTIA_H
#include "FITKAbaAbstractInertia.h"


namespace Interface {

    /**
     * @brief  集中质量、惯量类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-05-10
     */
    class FITKInterfaceStructuralAPI FITKAbaInertiaPointMassInertia: public FITKAbaAbstractInertia
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaInertiaPointMassInertia);
    public:
        /**
         * @brief  质量类型。各向同性、各向异性
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        enum MassType
        {
            Isotropic,
            Anisotropic,
        };
        Q_ENUM(MassType);
    public:
        /**
         * @brief  构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        FITKAbaInertiaPointMassInertia() = default;
        /**
         * @brief  析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        ~FITKAbaInertiaPointMassInertia() = default;
        bool copy(FITKAbstractDataObject* obj);
        /**
         * @brief  获取惯性元素类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        FITKAbaInertiaType getInertiaType() const override;
        /**
         * @brief  获取质量类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        MassType getMassType() const;
        /**
         * @brief  设置质量类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        void setMassType(MassType newMassType);
        /**
         * @brief  获取各向同性质量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        double getIsoTropicMass() const;
        /**
         * @brief  设置各向同性质量
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        void setIsoTropicMass(double newIsoTropicMass);
        /**
         * @brief  获取M11
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        double getM11() const;
        /**
         * @brief  设置M11
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        void setM11(double newM11);
        /**
         * @brief  获取M22
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        double getM22() const;
        /**
         * @brief  设置M22
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        void setM22(double newM22);
        double getM33() const;
        void setM33(double newM33);
        bool getSpecifyOffDiagnalTerms() const;
        void setSpecifyOffDiagnalTerms(bool newSpecifyOffDiagnalTerms);
        double getI11() const;
        void setI11(double newI11);
        double getI12() const;
        void setI12(double newI12);
        double getI13() const;
        void setI13(double newI13);
        double getI22() const;
        void setI22(double newI22);
        double getI23() const;
        void setI23(double newI23);
        double getI33() const;
        void setI33(double newI33);
        double getAlpha() const;
        void setAlpha(double newAlpha);
        double getComposite() const;
        void setComposite(double newComposite);
        int getCsysId() const;
        void setCsysId(int newCsysId);

    private:
        MassType m_massType{MassType::Isotropic};
        double m_isoTropicMass{0.0};
        double m_M11{0.0};
        double m_M22{0.0};
        double m_M33{0.0};
        bool m_SpecifyOffDiagnalTerms{false};
        double m_I11{0.0};
        double m_I12{0.0};
        double m_I13{0.0};
        double m_I22{0.0};
        double m_I23{0.0};
        double m_I33{0.0};
        double m_alpha{0.0};
        double m_composite{0.0};
        int m_csysId{0};
    };
}


#endif // FITKABAINERTIAPOINTMASSINERTIA_H
