/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorBehaviorDamping.h
 * @brief  连接器截面行为阻尼
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORBEHAVIORDAMPING_H
#define FITKABACONNECTORBEHAVIORDAMPING_H
#include "FITKAbaConnectorBehavior.h"
#include <array>


namespace Interface {

class FITKInterfaceStructuralAPI ConnectorBHDampingData
{
public:
    double getC11() const;
    void setC11(double newC11);
    double getC12() const;
    void setC12(double newC12);
    double getC22() const;
    void setC22(double newC22);
    double getC13() const;
    void setC13(double newC13);
    double getC23() const;
    void setC23(double newC23);
    double getC33() const;
    void setC33(double newC33);
    double getC14() const;
    void setC14(double newC14);
    double getC24() const;
    void setC24(double newC24);
    double getC34() const;
    void setC34(double newC34);
    double getC44() const;
    void setC44(double newC44);
    double getC15() const;
    void setC15(double newC15);
    double getC25() const;
    void setC25(double newC25);
    double getC35() const;
    void setC35(double newC35);
    double getC45() const;
    void setC45(double newC45);
    double getC55() const;
    void setC55(double newC55);
    double getC16() const;
    void setC16(double newC16);
    double getC26() const;
    void setC26(double newC26);
    double getC36() const;
    void setC36(double newC36);
    double getC46() const;
    void setC46(double newC46);
    double getC56() const;
    void setC56(double newC56);
    double getC66() const;
    void setC66(double newC66);
    double getForM() const;
    void setForM(double newForM);
    double getVorVR() const;
    void setVorVR(double newVorVR);
    double getF1() const;
    void setF1(double newF1);
    double getF2() const;
    void setF2(double newF2);
    double getF3() const;
    void setF3(double newF3);
    double getM1() const;
    void setM1(double newM1);
    double getM2() const;
    void setM2(double newM2);
    double getM3() const;
    void setM3(double newM3);
    double getV1() const;
    void setV1(double newV1);
    double getV2() const;
    void setV2(double newV2);
    double getV3() const;
    void setV3(double newV3);
    double getVR1() const;
    void setVR1(double newVR1);
    double getVR2() const;
    void setVR2(double newVR2);
    double getVR3() const;
    void setVR3(double newVR3);
    double getCP1() const;
    void setCP1(double newCP1);
    double getCP2() const;
    void setCP2(double newCP2);
    double getCP3() const;
    void setCP3(double newCP3);
    double getCPR1() const;
    void setCPR1(double newCPR1);
    double getCPR2() const;
    void setCPR2(double newCPR2);
    double getCPR3() const;
    void setCPR3(double newCPR3);
    double getCCU1() const;
    void setCCU1(double newCCU1);
    double getCCU2() const;
    void setCCU2(double newCCU2);
    double getCCU3() const;
    void setCCU3(double newCCU3);
    double getCCUR1() const;
    void setCCUR1(double newCCUR1);
    double getCCUR2() const;
    void setCCUR2(double newCCUR2);
    double getCCUR3() const;
    void setCCUR3(double newCCUR3);

private:
    double C11{0.0};
    double C12{0.0};
    double C22{0.0};
    double C13{0.0};
    double C23{0.0};
    double C33{0.0};
    double C14{0.0};
    double C24{0.0};
    double C34{0.0};
    double C44{0.0};
    double C15{0.0};
    double C25{0.0};
    double C35{0.0};
    double C45{0.0};
    double C55{0.0};
    double C16{0.0};
    double C26{0.0};
    double C36{0.0};
    double C46{0.0};
    double C56{0.0};
    double C66{0.0};
    double ForM{0.0};
    double VorVR{0.0};
    double F1{0.0};
    double F2{0.0};
    double F3{0.0};
    double M1{0.0};
    double M2{0.0};
    double M3{0.0};
    double V1{0.0};
    double V2{0.0};
    double V3{0.0};
    double VR1{0.0};
    double VR2{0.0};
    double VR3{0.0};
    double CP1{0.0};
    double CP2{0.0};
    double CP3{0.0};
    double CPR1{0.0};
    double CPR2{0.0};
    double CPR3{0.0};
    double CCU1{0.0};
    double CCU2{0.0};
    double CCU3{0.0};
    double CCUR1{0.0};
    double CCUR2{0.0};
    double CCUR3{0.0};
};

class FITKInterfaceStructuralAPI FITKAbaConnectorBehaviorDamping: public FITKAbaConnectorBehavior
{
public:
    FITKAbaConnectorBehaviorDamping() = default;
    ~FITKAbaConnectorBehaviorDamping() = default;
    FITKAbaConnectorBehaviorEnum::ConnectorBehaviorType getBehaviorType() const;
    FITKAbaConnectorBehaviorEnum::Definition getDefinition() const;
    void setDefinition(const FITKAbaConnectorBehaviorEnum::Definition &newDefinition);
    std::array<bool, 6> getForceMoment() const;
    void setForceMoment(const std::array<bool, 6> &newForceMoment);
    std::array<bool, 6> getIndependentComps() const;
    void setIndependentComps(const std::array<bool, 6> &newIndependentComps);
    FITKAbaConnectorBehaviorEnum::Coupling getCoupling() const;
    void setCoupling(const FITKAbaConnectorBehaviorEnum::Coupling &newCoupling);
    int getDataRowCount() const;
    void setDataRowCount(int num);
    void insertDataRow(int index);
    void removeDataRow(int index);
    ConnectorBHDampingData& getRowData(int index = 0);
    const ConnectorBHDampingData& getRowData(int index = 0) const;
private:
    FITKAbaConnectorBehaviorEnum::Definition m_definition{FITKAbaConnectorBehaviorEnum::Definition::Linear};
    std::array<bool, 6> m_forceMoment{false, false, false, false, false, false};
    std::array<bool, 6> m_independentComps{false, false, false, false, false, false};
    FITKAbaConnectorBehaviorEnum::Coupling m_coupling{FITKAbaConnectorBehaviorEnum::Coupling::Uncoupled};
    QVector<ConnectorBHDampingData> m_datas{ConnectorBHDampingData(),};
};
}
#endif // FITKABACONNECTORBEHAVIORDAMPING_H
