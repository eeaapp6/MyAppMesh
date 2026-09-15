/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file AI_Properties.cpp
 * @brief AI工具封装实现，组合材料与属性基础能力
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-18
 */

#include "AI_Properties.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKAbstractRadMaterial.h"
#include "FITK_Component/FITKRadiossData/FITKPropAbstract.hpp"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

namespace
{
    QString materialTypeToString(Radioss::FITKAbstractRadMaterial::RadMatType type)
    {
        switch (type)
        {
        case Radioss::FITKAbstractRadMaterial::LAW0:
            return "Void";
        case Radioss::FITKAbstractRadMaterial::LAW1:
            return "Elastic";
        case Radioss::FITKAbstractRadMaterial::LAW2:
            return "PlasJohns";
        case Radioss::FITKAbstractRadMaterial::LAW6:
            return "Hydro";
        case Radioss::FITKAbstractRadMaterial::LAW27:
            return "PlasBrit";
        case Radioss::FITKAbstractRadMaterial::LAW36:
            return "PlasTab";
        case Radioss::FITKAbstractRadMaterial::None:
        default:
            return "";
        }
    }

    QString propertyTypeToString(Radioss::FITKPropAbstract::RadPropType type)
    {
        switch (type)
        {
        case Radioss::FITKPropAbstract::Beam:
            return "Beam";
        case Radioss::FITKPropAbstract::Shell:
            return "Shell";
        case Radioss::FITKPropAbstract::Solid:
            return "Solid";
        case Radioss::FITKPropAbstract::Sandwich:
            return "Sandwich";
        case Radioss::FITKPropAbstract::BeamSPR:
            return "BeamSPR";
        case Radioss::FITKPropAbstract::SPH:
            return "SPH";
        case Radioss::FITKPropAbstract::Unknown:
        default:
            return "";
        }
    }

    QString failureModelTypeToString(Radioss::FITKAbstractFailureModel::FailureModelType type)
    {
        switch (type)
        {
        case Radioss::FITKAbstractFailureModel::BIQUAD:
            return "BIQUAD";
        case Radioss::FITKAbstractFailureModel::JOHNSON:
            return "JOHNSON";
        default:
            return "";
        }
    }

    QString equationOfStateTypeToString(Radioss::FITKAbstractEquationOfState::EOS_Type type)
    {
        switch (type)
        {
        case Radioss::FITKAbstractEquationOfState::Polynomial:
            return "Polynomial";
        case Radioss::FITKAbstractEquationOfState::Unknown:
        default:
            return "";
        }
    }

    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

QString AI_Properties::getMaterialType(const QString& name)
{
    Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (dataCase == nullptr) return "";

    Interface::FITKMaterialManager* materialManager = dataCase->getMaterialManager();
    if (materialManager == nullptr) return "";

    Interface::FITKAbstractMaterial* material = materialManager->getDataByName(name);
    Radioss::FITKAbstractRadMaterial* radMaterial = dynamic_cast<Radioss::FITKAbstractRadMaterial*>(material);
    if (!radMaterial) return "";

    return materialTypeToString(radMaterial->getRadMatType());
}

QString AI_Properties::getPropertyType(const QString& name)
{
    Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (dataCase == nullptr) return "";

    Interface::FITKAbstractSectionManager* propertyManager = dataCase->getSectionManager();
    if (propertyManager == nullptr) return "";

    Interface::FITKAbstractSection* property = propertyManager->getDataByName(name);
    Radioss::FITKPropAbstract* radProperty = dynamic_cast<Radioss::FITKPropAbstract*>(property);
    if (!radProperty) return "";

    return propertyTypeToString(radProperty->getRadPropType());
}

QString AI_Properties::getFailureModelType(const QString& name)
{
    Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (dataCase == nullptr) return "";

    Radioss::FITKRadiossFailureModelManager* failureModelManager = dataCase->getFailureModelManager();
    if (failureModelManager == nullptr) return "";

    Radioss::FITKAbstractFailureModel* failureModel = failureModelManager->getDataByName(name);
    if (!failureModel) return "";

    return failureModelTypeToString(failureModel->getFailureModelType());
}

QString AI_Properties::getEquationOfStateType(const QString& name)
{
    Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (dataCase == nullptr) return "";

    Radioss::FITKEquationOfStateManager* equationOfStateManager = dataCase->getEquationOfState();
    if (equationOfStateManager == nullptr) return "";

    Radioss::FITKAbstractEquationOfState* equationOfState = equationOfStateManager->getDataByName(name);
    if (equationOfState == nullptr) return "";

    return equationOfStateTypeToString(equationOfState->getEOSType());
}

AI_Properties* AI_PropertiesWapper::new_AI_Properties()
{
    return new AI_Properties();
}

void AI_PropertiesWapper::delete_AI_Properties(AI_Properties* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

QString AI_PropertiesWapper::getMaterialType(AI_Properties* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }
    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getMaterialType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! Material not found!");
    }
    return sendMessageToServer(QString("The material exists, and the material type is %1").arg(result));
}

QString AI_PropertiesWapper::getPropertyType(AI_Properties* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }
    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getPropertyType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! Property not found!");
    }
    return sendMessageToServer(QString("The property exists, and the property type is %1").arg(result));
}

QString AI_PropertiesWapper::getFailureModelType(AI_Properties* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }
    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getFailureModelType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! Failure model not found!");
    }
    return sendMessageToServer(QString("The failure model exists, and the failure model type is %1").arg(result));
}

QString AI_PropertiesWapper::getEquationOfStateType(AI_Properties* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Error! Object destroyed.");
    }
    if (name.isEmpty())
    {
        return sendMessageToServer("Error! Name is empty.");
    }

    QString result = obj->getEquationOfStateType(name);
    if (result.isEmpty())
    {
        return sendMessageToServer("Error! Equation of state not found!");
    }
    return sendMessageToServer(QString("The equation of state exists, and the equation of state type is %1").arg(result));
}
