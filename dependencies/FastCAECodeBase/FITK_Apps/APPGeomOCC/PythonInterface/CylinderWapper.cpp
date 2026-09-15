/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CylinderWapper.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCylinder.h"
#include "PythonInterfaceCommons.h"

Cylinder* CylinderWapper::new_Cylinder()
{
    PythonInterfaceCommons::sendMessageToHttpServer("Create CylinderWapper success!");
    return new Cylinder;
}

void CylinderWapper::delete_Cylinder(Cylinder* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Delete Cylinder failed!");
        return;
    }

    delete obj;
    PythonInterfaceCommons::sendMessageToHttpServer("Delete Cylinder success!");
}

Cylinder* CylinderWapper::static_Cylinder_GetGeometry(const QString& name)
{
    if (name.isEmpty())
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get Cylinder geometry failed, name is empty!");
        return nullptr;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get Cylinder geometry success: %1").arg(name));
    return new Cylinder(name);
}

// PythonQt decorator entry that forwards cylinder creation parameters to the wrapped geometry object.
QString CylinderWapper::createGeometry(Cylinder* obj, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length)
{
    if (obj == nullptr)
    {
        QString resultText = tr("Failed");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    bool result = obj->createGeometry("Cylinder-1", x_c, y_c, z_c, x_d, y_d, z_d, radius, length);
    if (result == false)
    {
        AppFrame::FITKMessageError("Create Cylinder failed !");
        QString resultText = "Create Cylinder failed!";
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    QString resultText = tr("Success, created cylinder named %1").arg(obj->getName());
    PythonInterfaceCommons::sendMessageToHttpServer(resultText);
    return resultText;
}

QString CylinderWapper::getName(Cylinder* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get Cylinder name failed!");
        return QString();
    }

    QString result = obj->getName();
    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get Cylinder name success: %1").arg(result));
    return result;
}

QString CylinderWapper::setName(Cylinder* obj, const QString& name)
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

Cylinder::Cylinder(const QString& name) : PyGeometryBase(name)
{
}

Cylinder::Cylinder() : PyGeometryBase()
{
}

// Build and execute the underlying geometry command, then sync the graph/tree views.
bool Cylinder::createGeometry(const QString& name, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length)
{
    if (_cmdList == nullptr) return false;
    QString uniqueName = _cmdList->checkName(name);

    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;

    Interface::FITKAbsGeoModelCylinder* obj = dynamic_cast<Interface::FITKAbsGeoModelCylinder*>(
        geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTCylinder));
    if (!obj) return false;

    double point1[3] = { x_c, y_c, z_c };
    double direction[3] = { x_d, y_d, z_d };
    obj->setDataObjectName(uniqueName);
    obj->setLocation(point1);
    obj->setDirection(direction);
    obj->setRadius(radius);
    obj->setLength(length);
    if (obj->update() == false) {
        return false;
    }

    _cmdList->appendDataObj(obj);
    _command = obj;
    PyGeometryBase::updateGraphAndTreeEvent();
    return true;
}