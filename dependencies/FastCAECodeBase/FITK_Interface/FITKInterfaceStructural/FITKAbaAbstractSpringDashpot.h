/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractSpringDashpot.h
 * @brief  抽象弹簧/阻尼类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-19
 * 
 */
#ifndef FITKABAABSTRACTSPRINGDASHPOT_H
#define FITKABAABSTRACTSPRINGDASHPOT_H
#include "FITKAbaWire.h"
#include "FITKInterfaceStructuralAPI.h"

namespace Interface {

    class FITKInterfaceStructuralAPI FITKAbaAbstractSpringDashpot : //public FITKModelWire
        public FITKAbaWire
    {
        Q_OBJECT
    public:
        enum SpringDashpotType
        {
            ConnectTwoPoints,
            //ConnectPointsToGround //尚未支持
        };
        Q_ENUM(SpringDashpotType);

        FITKAbaAbstractSpringDashpot() = default;
        ~FITKAbaAbstractSpringDashpot() = default;
        virtual SpringDashpotType getSpringDashpotType() const = 0;
        bool copy(Core::FITKAbstractDataObject* obj) override;

        double getSpringStiffness() const;
        void setSpringStiffness(double newSpringStiffness);
        double getDaspotCoefficent() const;
        void setDaspotCoefficent(double newDaspotCoefficent);
        bool getStiffnessState() const;
        bool getDaspotState() const;
        void setStiffnessState(bool state);
        void setDaspotState(bool state);
    protected:
        /**
         * @brief  弹簧刚度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-06-19
         */
        double m_springStiffness{ 0.0 };
        /**
         * @brief  阻尼系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-06-19
         */
        double m_daspotCoefficent{ 0.0 };
        bool m_stiffnessState{ true };
        bool m_daspotState{ false };
};
}
#endif // FITKABAABSTRACTSPRINGDASHPOT_H
