/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MaterialElasticWapper.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

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
    Radioss::FITKMaterialElastic* getElasticMaterial(Elastic* obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Radioss::FITKMaterialElastic*>(obj->getMaterial());
    }
}

Elastic* MaterialElasticWapper::new_Elastic()
{
    return new Elastic();
}
void MaterialElasticWapper::delete_Elastic(Elastic* obj)
{
    if (obj) delete obj;
}
Elastic* MaterialElasticWapper::static_Elastic_GetElastic(const QString& name)
{
    return new Elastic(name);
}
QString MaterialElasticWapper::getName(Elastic* obj)
{
    if (obj)
        return obj->getName();
    return QString();
}
QString MaterialElasticWapper::setName(Elastic* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->setName(name);
    if (!ok)
    {
        QString errorMsg = tr("Failed, Material with name %1 may already exist").arg(name);
        return sendMessageToServer(errorMsg);
    }
    QString successMsg = tr("Success, Set name to %1").arg(name);
    return sendMessageToServer(successMsg);
}

QString MaterialElasticWapper::createMaterial(Elastic* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer("Failed, object is null");
    }

    bool ok = obj->createMaterial(name, (int)Radioss::FITKAbstractRadMaterial::LAW1);
    if (!ok)
    {
        QString errorMsg = tr("Creation failed! Material with name %1 may already exist").arg(name);
        return sendMessageToServer(errorMsg);
    }

    QString successMsg = tr("Success, Material %1 created").arg(name);
    return sendMessageToServer(successMsg);
}

QString MaterialElasticWapper::setDensity(Elastic* obj, double density)
{
    Radioss::FITKMaterialElastic* material = getElasticMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    if (density < 0)
    {
        return sendMessageToServer("Failed !Density invalid !");
    }

    material->setDensity(density);
    return sendMessageToServer("Set Density successfully");
}

QString MaterialElasticWapper::setYoungsModulus(Elastic* obj, double modulus)
{
    Radioss::FITKMaterialElastic* material = getElasticMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found");
    }

    if (modulus < 0)
    {
        return sendMessageToServer("Failed !Young's Modulus invalid !");
    }

    material->setYoungsModulus(modulus);
    return sendMessageToServer("Set Youngs Modulus successfully");
}

QString MaterialElasticWapper::setPoissonsRatio(Elastic* obj, double ratio)
{
    Radioss::FITKMaterialElastic* material = getElasticMaterial(obj);
    if (material == nullptr)
    {
        return sendMessageToServer("Failed !No material found!");
    }

    if (ratio < 0 || ratio >= 0.5)
    {
        return sendMessageToServer("Failed !Poisson's Ratio invalid !");
    }

    material->setPoissonsRatio(ratio);
    return sendMessageToServer("Set Poissons Ratio successfully");
}

/************************************************************************************/
Elastic::Elastic(const QString& name):
    PyInterface::PyMaterialsBase(name)
{
}
Elastic::Elastic():
    PyInterface::PyMaterialsBase()
{
}