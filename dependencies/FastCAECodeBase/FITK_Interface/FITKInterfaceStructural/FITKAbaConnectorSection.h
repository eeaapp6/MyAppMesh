/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorSection.h
 * @brief  连接器截面类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORSECTION_H
#define FITKABACONNECTORSECTION_H
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaConnectorBehavior.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

namespace Interface {

class FITKInterfaceStructuralAPI FITKAbaConnectorSection: public Core::FITKAbstractNDataObject
{
    Q_OBJECT
    FITKCLASS(Interface, FITKAbaConnectorSection)
public:
    enum Category
    {
        AssembledComplex,
        Basic,
        MPC,
    };
    Q_ENUM(Category)

    enum AssembledComplexType
    {
        Beam,
        Cylindrical,
        Hinge,
    };
    Q_ENUM(AssembledComplexType)

    enum TranslationalType
    {
        NoneTranslational,
        Axial,
        Cartesian
    };
    Q_ENUM(TranslationalType)

    enum RotationalType
    {
        NoneRotational,
        Cardan,
    };
    Q_ENUM(RotationalType)

    FITKAbaConnectorSection() = default;
    ~FITKAbaConnectorSection();
    bool copy(Core::FITKAbstractDataObject* obj) override;
    Category getCategory() const;
    void setCategory(Category type);
    const QList<FITKAbaConnectorBehavior*> getAllBehaviors() const;
    FITKAbaConnectorBehavior* addBehavior(FITKAbaConnectorBehaviorEnum::ConnectorBehaviorType type);
    void addBehavior(FITKAbaConnectorBehavior* bh);
    void removeBehavior(FITKAbaConnectorBehavior* bh);
    void clearAllBehaviors();
    AssembledComplexType getAssembledComplexType() const;
    void setAssebledComplexType(AssembledComplexType newAssebledComplexType);
    TranslationalType getTranslationalType() const;
    void setTranslationalType(TranslationalType newTranslationalType);
    RotationalType getRotationalType() const;
    void setRotationalType(RotationalType newRotationalType);
    QString getTypeString();
protected:
    QList<FITKAbaConnectorBehavior*> m_behaviorOptions{};
    Category m_category{Category::AssembledComplex};
    AssembledComplexType m_assebledComplexType{AssembledComplexType::Beam};
    TranslationalType m_translationalType{TranslationalType::NoneTranslational};
    RotationalType m_rotationalType{RotationalType::NoneRotational};
};
}
#endif // FITKABACONNECTORSECTION_H
