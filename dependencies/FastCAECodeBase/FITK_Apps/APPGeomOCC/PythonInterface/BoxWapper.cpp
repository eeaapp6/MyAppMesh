/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "BoxWapper.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "PythonInterfaceCommons.h"

Box* BoxWapper::new_Box()
{
    PythonInterfaceCommons::sendMessageToHttpServer("Create BoxWapper success!");
    return new Box;
}

void BoxWapper::delete_Box(Box* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Delete Box failed!");
        return;
    }

    delete obj;
    PythonInterfaceCommons::sendMessageToHttpServer("Delete Box success!");
}

Box* BoxWapper::static_Box_GetGeometry(const QString& name)
{
    if (name.isEmpty())
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get Box geometry failed, name is empty!");
        return nullptr;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get Box geometry success: %1").arg(name));
    return new Box(name);
}

// PythonQt decorator entry that forwards box creation parameters to the wrapped geometry object.
QString BoxWapper::createGeometry(Box* obj, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l)
{
    if (obj == nullptr)
    {
        QString resultText = tr("Failed");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    bool result = obj->createGeometry("Box-1", x_p, y_p, z_p, x_l, y_l, z_l);
    if (result == false)
    {
        AppFrame::FITKMessageError("Create Box failed !");
        QString resultText = "Create Box failed!";
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    QString resultText = tr("Success, created box named %1").arg(obj->getName());
    PythonInterfaceCommons::sendMessageToHttpServer(resultText);
    return resultText;
}

QString BoxWapper::getName(Box* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get Box name failed!");
        return QString();
    }

    QString result = obj->getName();
    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get Box name success: %1").arg(result));
    return result;
}

QString BoxWapper::setName(Box* obj, const QString& name)
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

Box::Box(const QString& name) : PyGeometryBase(name)
{
}

Box::Box() : PyGeometryBase()
{
}

// Build and execute the underlying geometry command, then sync the graph/tree views.
bool Box::createGeometry(const QString& name, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l)
{
    if (_cmdList == nullptr) return false;
    QString uniqueName = _cmdList->checkName(name);

    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;

    Interface::FITKAbsGeoModelBox* obj = dynamic_cast<Interface::FITKAbsGeoModelBox*>(
        geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTBox));
    if (!obj) return false;

    double point1[3] = { x_p, y_p, z_p };
    double length[3] = { x_l, y_l, z_l };
    obj->setDataObjectName(uniqueName);
    obj->setPoint1(point1);
    obj->setLength(length);
    if (obj->update() == false) {
        return false;
    }

    _cmdList->appendDataObj(obj);
    _command = obj;
    PyGeometryBase::updateGraphAndTreeEvent();
    return true;
}