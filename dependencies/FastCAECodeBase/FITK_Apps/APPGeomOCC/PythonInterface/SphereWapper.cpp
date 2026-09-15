/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SphereWapper.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSphere.h"
#include "PythonInterfaceCommons.h"

Sphere* SphereWapper::new_Sphere()
{
    PythonInterfaceCommons::sendMessageToHttpServer("Create SphereWapper success!");
    return new Sphere;
}

void SphereWapper::delete_Sphere(Sphere* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Delete Sphere failed!");
        return;
    }

    delete obj;
    PythonInterfaceCommons::sendMessageToHttpServer("Delete Sphere success!");
}

Sphere* SphereWapper::static_Sphere_GetGeometry(const QString& name)
{
    if (name.isEmpty())
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get Sphere geometry failed, name is empty!");
        return nullptr;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get Sphere geometry success: %1").arg(name));
    return new Sphere(name);
}

// PythonQt decorator entry that forwards sphere creation parameters to the wrapped geometry object.
QString SphereWapper::createGeometry(Sphere* obj, double x_c, double y_c, double z_c, double radius)
{
    if (obj == nullptr)
    {
        QString resultText = tr("Failed");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    bool result = obj->createGeometry("Sphere-1", x_c, y_c, z_c, radius);
    if (result == false)
    {
        AppFrame::FITKMessageError("Create Sphere failed !");
        QString resultText = "Create Sphere failed!";
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    QString resultText = tr("Success, created sphere named %1").arg(obj->getName());
    PythonInterfaceCommons::sendMessageToHttpServer(resultText);
    return resultText;
}

QString SphereWapper::getName(Sphere* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get Sphere name failed!");
        return QString();
    }

    QString result = obj->getName();
    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get Sphere name success: %1").arg(result));
    return result;
}

QString SphereWapper::setName(Sphere* obj, const QString& name)
{
    if (obj == nullptr)
    {
        QString resultText = QObject::tr("Failed");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    if (name.isEmpty())
    {
        QString resultText = QObject::tr("Failed, Name cannot be empty !");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    bool ok = obj->setName(name);
    if (ok == false)
    {
        QString resultText = QObject::tr("Failed, Name already exists !");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    QString resultText = QObject::tr("Success, name set to %1").arg(name);
    PythonInterfaceCommons::sendMessageToHttpServer(resultText);
    return resultText;
}

Sphere::Sphere(const QString& name) : PyGeometryBase(name)
{
}

Sphere::Sphere() : PyGeometryBase()
{
}

// Build and execute the underlying geometry command, then sync the graph/tree views.
bool Sphere::createGeometry(const QString& name, double x_c, double y_c, double z_c, double radius)
{
    if (_cmdList == nullptr) return false;
    QString uniqueName = _cmdList->checkName(name);

    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;

    Interface::FITKAbsGeoModelSphere* obj = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(
        geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTSphere));
    if (!obj) return false;

    double center[3] = { x_c, y_c, z_c };
    obj->setDataObjectName(uniqueName);
    obj->setLocation(center);
    obj->setRadius(radius);
    if (obj->update() == false) {
        return false;
    }

    _cmdList->appendDataObj(obj);
    _command = obj;
    PyGeometryBase::updateGraphAndTreeEvent();
    return true;
}