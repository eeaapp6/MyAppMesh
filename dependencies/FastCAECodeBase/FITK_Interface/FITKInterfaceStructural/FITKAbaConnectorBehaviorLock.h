/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorBehaviorLock.h
 * @brief  连接器截面行为Lock
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORBEHAVIORLOCK_H
#define FITKABACONNECTORBEHAVIORLOCK_H
#include "FITKAbaConnectorBehavior.h"
#include <array>

namespace Interface {

class FITKInterfaceStructuralAPI FITKAbaConnectorBehaviorLock: public FITKAbaConnectorBehavior
{
public:

    FITKAbaConnectorBehaviorLock() = default;
    ~FITKAbaConnectorBehaviorLock() = default;
    FITKAbaConnectorBehaviorEnum::ConnectorBehaviorType getBehaviorType() const override;
    std::array<bool, 6> getComponents() const;
    void setComponents(const std::array<bool, 6> &newComponents);
    FITKAbaConnectorBehaviorEnum::LockType getLockType() const;
    void setLockType(const FITKAbaConnectorBehaviorEnum::LockType &newLockType);
    FITKAbaConnectorBehaviorEnum::LockSpecify getLockSpecify() const;
    void setLockSpecify(const FITKAbaConnectorBehaviorEnum::LockSpecify &newLockSpecify);
    double getPosCriteriaLower() const;
    void setPosCriteriaLower(double newPosCriteriaLower);
    double getPosCriteriaUpper() const;
    void setPosCriteriaUpper(double newPosCriteriaUpper);
    double getFMCriteriaLower() const;
    void setFMCriteriaLower(double newFMCriteriaLower);
    double getFMCriteriaUpper() const;
    void setFMCriteriaUpper(double newFMCriteriaUpper);
    bool getUsePosLower() const;
    void setUsePosLower(bool newUsePosLower);
    bool getUsePosUpper() const;
    void setUsePosUpper(bool newUsePosUpper);
    bool getUseFMLower() const;
    void setUseFMLower(bool newUseFMLower);
    bool getUseFMUpper() const;
    void setUseFMUpper(bool newUseFMUpper);

private:
    std::array<bool, 6> m_components{false, false, false, false, false, false};
    FITKAbaConnectorBehaviorEnum::LockType m_lockType{FITKAbaConnectorBehaviorEnum::LockType::All};
    FITKAbaConnectorBehaviorEnum::LockSpecify m_lockSpecify{FITKAbaConnectorBehaviorEnum::LockSpecify::None};
    double m_posCriteriaLower{0.0};
    double m_posCriteriaUpper{0.0};
    double m_FMCriteriaLower{0.0};
    double m_FMCriteriaUpper{0.0};
    bool m_usePosLower{ false };
    bool m_usePosUpper{ false };
    bool m_useFMLower{ false };
    bool m_useFMUpper{ false };
};
}
#endif // FITKABACONNECTORBEHAVIORLOCK_H
