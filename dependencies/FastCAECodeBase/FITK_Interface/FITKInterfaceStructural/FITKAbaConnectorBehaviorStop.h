/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorBehaviorStop.h
 * @brief  连接器截面行为Stop
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORBEHAVIORSTOP_H
#define FITKABACONNECTORBEHAVIORSTOP_H
#include "FITKAbaConnectorBehavior.h"
#include <array>

namespace Interface {

    class FITKInterfaceStructuralAPI FITKAbaConnectorBehaviorStop: public FITKAbaConnectorBehavior
    {
    public:
        FITKAbaConnectorBehaviorStop() = default;
        ~FITKAbaConnectorBehaviorStop() = default;
        FITKAbaConnectorBehaviorEnum::ConnectorBehaviorType getBehaviorType() const;
        std::array<bool, 6> getComponents() const;
        void setComponents(const std::array<bool, 6>& states);
        double getLowerBound() const;
        void setLowerBound(double value);
        double getUpperBound() const;
        void setUpperBound(double value);
        bool getUseLowerBound() const;
        void setUseLowerBound(bool newUseLowerBound);
        bool getUseUpperBound() const;
        void setUseUpperBound(bool newUseUpperBound);

    private:
        std::array<bool, 6> m_components{false, false, false, false, false, false};
        double m_lowerBound{0.0};
        double m_upperBound{0.0};
        bool m_useLowerBound{ false };
        bool m_useUpperBound{ false };
    };
}
#endif // FITKABACONNECTORBEHAVIORSTOP_H
