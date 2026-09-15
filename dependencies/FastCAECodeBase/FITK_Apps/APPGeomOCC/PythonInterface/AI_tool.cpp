/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "AI_tool.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "PythonInterfaceCommons.h"

#include <QMetaEnum>

namespace {

QString geometryTypeToString(Interface::FITKGeoEnum::FITKGeometryComType type)
{
    switch (type) {
    case Interface::FITKGeoEnum::FGTBox:
        return QStringLiteral("Box");
    case Interface::FITKGeoEnum::FGTCylinder:
        return QStringLiteral("Cylinder");
    case Interface::FITKGeoEnum::FGTSphere:
        return QStringLiteral("Sphere");
    case Interface::FITKGeoEnum::FGTBool:
        return QStringLiteral("Bool");

    default:
        return QStringLiteral("Unknown");
    }
    
}

}

QString AI_tool::isGeometryExist(const QString &name)
{
    Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
    if (name.isEmpty()) 
        return QString("The geometry name is empty");

    Interface::FITKAbsGeoCommand* command = cmdList->getDataByName(name);
    QString result = command ? QString("The geometry exists") : QString("The geometry does not exist");
    return result;
}

QString AI_tool::getGeometryType(const QString &name)
{
    Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
    if (name.isEmpty()) return QString("The geometry name is empty");

    Interface::FITKAbsGeoCommand* command = cmdList->getDataByName(name);
    if (command == nullptr) return QString("The geometry does not exist");

    return QString("The geometry type is %1").arg(geometryTypeToString(command->getGeometryCommandType()));
}

AI_tool* AI_toolWrapper::new_AI_tool()
{
    PythonInterfaceCommons::sendMessageToHttpServer("Create AI_tool success!");
    return new AI_tool;
}

void AI_toolWrapper::delete_AI_tool(AI_tool* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Delete AI_tool failed!");
        return;
    }

    delete obj;
    PythonInterfaceCommons::sendMessageToHttpServer("Delete AI_tool success!");
}

QString AI_toolWrapper::isGeometryExist(AI_tool* obj, const QString& name)
{
    if (obj == nullptr)
    {
        QString result = "AI_tool is null";
        PythonInterfaceCommons::sendMessageToHttpServer(result);
        return result;
    }

    QString result = obj->isGeometryExist(name);
    PythonInterfaceCommons::sendMessageToHttpServer(result);
    return result;
}

QString AI_toolWrapper::getGeometryType(AI_tool* obj, const QString& name)
{
    if (obj == nullptr)
    {
        QString result = "AI_tool is null";
        PythonInterfaceCommons::sendMessageToHttpServer(result);
        return result;
    }

    QString result = obj->getGeometryType(name);
    PythonInterfaceCommons::sendMessageToHttpServer(result);
    return result;
}
