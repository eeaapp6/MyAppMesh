/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorBehaviorElasticity.h
 * @brief  连接器截面行为弹性
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORBEHAVIORELASTICITY_H
#define FITKABACONNECTORBEHAVIORELASTICITY_H
#include "FITKAbaConnectorBehavior.h"
#include <array>
#include <QVector>


namespace Interface {

    class FITKInterfaceStructuralAPI ConnectorBHElasticityData
{
public:
    double getD11() const;
    void setD11(double newD11);
    double getD12() const;
    void setD12(double newD12);
    double getD22() const;
    void setD22(double newD22);
    double getD13() const;
    void setD13(double newD13);
    double getD23() const;
    void setD23(double newD23);
    double getD33() const;
    void setD33(double newD33);
    double getD14() const;
    void setD14(double newD14);
    double getD24() const;
    void setD24(double newD24);
    double getD34() const;
    void setD34(double newD34);
    double getD44() const;
    void setD44(double newD44);
    double getD15() const;
    void setD15(double newD15);
    double getD25() const;
    void setD25(double newD25);
    double getD35() const;
    void setD35(double newD35);
    double getD45() const;
    void setD45(double newD45);
    double getD55() const;
    void setD55(double newD55);
    double getD16() const;
    void setD16(double newD16);
    double getD26() const;
    void setD26(double newD26);
    double getD36() const;
    void setD36(double newD36);
    double getD46() const;
    void setD46(double newD46);
    double getD56() const;
    void setD56(double newD56);
    double getD66() const;
    void setD66(double newD66);
    double getForM() const;
    void setForM(double newForM);
    double getUorUR() const;
    void setUorUR(double newUorUR);
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
        double D11{0.0};
        double D12{0.0};
        double D22{0.0};
        double D13{0.0};
        double D23{0.0};
        double D33{0.0};
        double D14{0.0};
        double D24{0.0};
        double D34{0.0};
        double D44{0.0};
        double D15{0.0};
        double D25{0.0};
        double D35{0.0};
        double D45{0.0};
        double D55{0.0};
        double D16{0.0};
        double D26{0.0};
        double D36{0.0};
        double D46{0.0};
        double D56{0.0};
        double D66{0.0};
        double ForM{0.0};
        double UorUR{0.0};
        double F1{0.0};
        double F2{0.0};
        double F3{0.0};
        double M1{0.0};
        double M2{0.0};
        double M3{0.0};
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

    class FITKInterfaceStructuralAPI FITKAbaConnectorBehaviorElasticity: public FITKAbaConnectorBehavior
    {
    public:
        FITKAbaConnectorBehaviorElasticity() = default;
        ~FITKAbaConnectorBehaviorElasticity() = default;
        FITKAbaConnectorBehaviorEnum::ConnectorBehaviorType getBehaviorType() const override;
        std::array<bool, 6> getForceMomentStates() const;
        void setForceMomentStates(const std::array<bool, 6>& states);
        FITKAbaConnectorBehaviorEnum::Definition getDefinition() const;
        void setDefinition(FITKAbaConnectorBehaviorEnum::Definition newDefinition);
        FITKAbaConnectorBehaviorEnum::Coupling getCoupling() const;
        void setCoupling(FITKAbaConnectorBehaviorEnum::Coupling newCoupling);
        int getDataRowCount() const;
        void setDataRowCount(int num);
        void insertDataRow(int index);
        void removeDataRow(int index);
        ConnectorBHElasticityData& getRowData(int index = 0);
        const ConnectorBHElasticityData& getRowData(int index = 0) const;
        std::array<bool, 6> getComponentsStates() const;
        void setComponentsStates(const std::array<bool, 6>& states);
        std::array<bool, 6> getIndepenentComponents() const;
        void setIndependentComponents(const std::array<bool, 6>& states);

    private:
        std::array<bool, 6> m_forceMoment{false, false, false, false, false, false};
        std::array<bool, 6> m_independentCompoments{false, false, false, false, false, false};
        std::array<bool, 6> m_components{false, false, false, false, false, false};
        FITKAbaConnectorBehaviorEnum::Definition m_definition{FITKAbaConnectorBehaviorEnum::Definition::Linear};
        FITKAbaConnectorBehaviorEnum::Coupling m_coupling{FITKAbaConnectorBehaviorEnum::Coupling::Uncoupled};
        QVector<ConnectorBHElasticityData> m_datas{ConnectorBHElasticityData(),};
    };
}


#endif // FITKABACONNECTORBEHAVIORELASTICITY_H
