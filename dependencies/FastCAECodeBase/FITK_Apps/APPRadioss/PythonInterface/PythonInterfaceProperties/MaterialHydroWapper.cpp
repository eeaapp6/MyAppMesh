/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MaterialHydroWapper.h"

#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

Hydro* MaterialHydroWapper::new_Hydro()
{
    return new Hydro();
}

void MaterialHydroWapper::delete_Hydro(Hydro* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

Hydro* MaterialHydroWapper::static_Hydro_GetHydro(const QString& name)
{
    return new Hydro(name);
}

QString MaterialHydroWapper::getName(Hydro* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString MaterialHydroWapper::setName(Hydro* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->setName(name);
    if (!ok)
    {
        return sendMessageToServer(tr("Failed, Material with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString MaterialHydroWapper::createMaterial(Hydro* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->createMaterial(name, static_cast<int>(Radioss::FITKAbstractRadMaterial::LAW6));
    if (!ok)
    {
        return sendMessageToServer(tr("Creation failed! Material with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Hydro material created with name %1").arg(name));
}

QString MaterialHydroWapper::setDensity(Hydro* obj, double density)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    Radioss::FITKMaterialHydro* material = dynamic_cast<Radioss::FITKMaterialHydro*>(obj->getMaterial());
    if (material == nullptr || density < 0.0)
    {
        if (material == nullptr)
        {
            return sendMessageToServer("Failed !No material found");
        }

        return sendMessageToServer("Failed !Density invalid !");
    }

    material->getMaterialData().m_density = density;
    return sendMessageToServer("Set Density successfully");
}

QString MaterialHydroWapper::setReferenceDensity(Hydro* obj, double rDensity)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    Radioss::FITKMaterialHydro* material = dynamic_cast<Radioss::FITKMaterialHydro*>(obj->getMaterial());
    if (material == nullptr || rDensity < 0.0)
    {
        if (material == nullptr)
        {
            return sendMessageToServer("Failed !No material found");
        }

        return sendMessageToServer("Failed !Reference Density invalid !");
    }

    material->getMaterialData().m_R_density = rDensity;
    return sendMessageToServer("Set Reference Density successfully");
}

QString MaterialHydroWapper::setKinematicViscosity(Hydro* obj, double viscosity)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    Radioss::FITKMaterialHydro* material = dynamic_cast<Radioss::FITKMaterialHydro*>(obj->getMaterial());
    if (material == nullptr || viscosity < 0.0)
    {
        if (material == nullptr)
        {
            return sendMessageToServer("Failed !No material found");
        }

        return sendMessageToServer("Failed !Kinematic Viscosity invalid !");
    }

    material->getMaterialData().m_kinematicViscosity = viscosity;
    return sendMessageToServer("Set Kinematic Viscosity successfully");
}

QString MaterialHydroWapper::setPressureCut(Hydro* obj, double pressureCut)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    Radioss::FITKMaterialHydro* material = dynamic_cast<Radioss::FITKMaterialHydro*>(obj->getMaterial());
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    material->getMaterialData().m_pressureCut = pressureCut;
    return sendMessageToServer("Set Pressure Cut successfully");
}

QString MaterialHydroWapper::setEOS(Hydro* obj, const QString& eosName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }
    Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (dataCase == nullptr)
    {
        return sendMessageToServer("Failed !No Radioss case found!");
    }
    Radioss::FITKEquationOfStateManager* Mgr = dataCase->getEquationOfState();
    if (Mgr == nullptr)
    {
        return sendMessageToServer("Failed !No equation of state manager found!");
    }
    Radioss::FITKMaterialHydro* material = dynamic_cast<Radioss::FITKMaterialHydro*>(obj->getMaterial());

    auto eosData = Mgr->getDataByName(eosName);
    if (eosData == nullptr|| material == nullptr)
    {
        if (material == nullptr)
        {
            return sendMessageToServer("Failed !No material found!");
        }

        return sendMessageToServer(tr("Failed !EOS with name %1 not found!").arg(eosName));
    }
    eosData->addMaterialID(material->getDataObjectID(), Mgr);
    material->setEOSID(eosData->getDataObjectID());
    return sendMessageToServer(tr("Set EOS to %1 successfully").arg(eosName));
}

Hydro::Hydro(const QString& name)
    : PyInterface::PyMaterialsBase(name)
{
}

Hydro::Hydro()
    : PyInterface::PyMaterialsBase()
{
}