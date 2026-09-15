/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorBehaviorPlasticity.h
 * @brief  连接器截面行为塑性
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORBEHAVIORPLASTICITY_H
#define FITKABACONNECTORBEHAVIORPLASTICITY_H
#include "FITKAbaConnectorBehavior.h"
#include <array>

namespace Interface {

class FITKInterfaceStructuralAPI PlasticityIsoHardenTabData
{
public:
    double getYieldForceMoment() const;
    void setYieldForceMoment(double newYieldForceMoment);
    double getPlasticMotion() const;
    void setPlasticMotion(double newPlasticMotion);
    double getRate() const;
    void setRate(double newRate);

private:
    double YieldForceMoment{0.0};
    double PlasticMotion{0.0};
    double Rate{0.0};
};

class FITKInterfaceStructuralAPI FITKAbaConnectorBehaviorPlasticity: public FITKAbaConnectorBehavior
{
public:
    FITKAbaConnectorBehaviorPlasticity()=default;
    ~FITKAbaConnectorBehaviorPlasticity()=default;
    FITKAbaConnectorBehaviorEnum::ConnectorBehaviorType getBehaviorType() const;
    FITKAbaConnectorBehaviorEnum::Coupling getCoupling() const;
    std::array<bool, 6> getForceMoment() const;
    bool getSpecifyHardening() const;
    bool getSpecifyKinematicHardening() const;
    FITKAbaConnectorBehaviorEnum::PlacticityIsoTropicHardeningDefinition getIsoHardenDefinition() const;
    void setCoupling(const FITKAbaConnectorBehaviorEnum::Coupling &newCoupling);
    void setForceMoment(const std::array<bool, 6> &newForceMoment);
    void setSpecifyHardening(bool newSpecifyHardening);
    void setSpecifyKinematicHardening(bool newSpecifyKinematicHardening);
    void setIsoHardenDefinition(const FITKAbaConnectorBehaviorEnum::PlacticityIsoTropicHardeningDefinition &newIsoHardenDefinition);
    int getIsoHardenTabDataRowCount() const;
    void setIsoHardenTabDataRowCount(int num);
    void insertHardenTabDataRowCount(int index);
    void removeHardenTabDataRowCount(int index);
    PlasticityIsoHardenTabData& getIsoHardenTabDataRow(int index);
    const PlasticityIsoHardenTabData& getIsoHardenTabDataRow(int index) const;
private:
    FITKAbaConnectorBehaviorEnum::Coupling m_coupling{FITKAbaConnectorBehaviorEnum::Coupling::Uncoupled};
    std::array<bool, 6> m_forceMoment{false, false, false, false, false, false};
    bool m_specifyHardening{true};
    bool m_specifyKinematicHardening{false};
    FITKAbaConnectorBehaviorEnum::PlacticityIsoTropicHardeningDefinition m_isoHardenDefinition{FITKAbaConnectorBehaviorEnum::PlacticityIsoTropicHardeningDefinition::Tabular};
    QVector<PlasticityIsoHardenTabData> m_isoHardenTabDatas{PlasticityIsoHardenTabData(),};
};
}
#endif // FITKABACONNECTORBEHAVIORPLASTICITY_H
