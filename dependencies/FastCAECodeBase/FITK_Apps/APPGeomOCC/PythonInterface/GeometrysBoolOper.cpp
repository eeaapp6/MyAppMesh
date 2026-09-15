/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GeometrysBoolOper.h"

#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "PythonInterfaceCommons.h"

#include <QDebug>

BoolOper* BoolOperWapper::new_BoolOper()
{
    PythonInterfaceCommons::sendMessageToHttpServer("Create BoolOperWapper success!");
    return new BoolOper();
}

void BoolOperWapper::delete_BoolOper(BoolOper* obj)
{
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Delete BoolOper failed!");
        return;
    }

    delete obj;
    PythonInterfaceCommons::sendMessageToHttpServer("Delete BoolOper success!");
}
BoolOper* BoolOperWapper::static_BoolOper_GetGeometry(const QString& name)
{
    if (name.isEmpty())
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get BoolOper geometry failed, name is empty!");
        return nullptr;
    }

    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get BoolOper geometry success: %1").arg(name));
    return new BoolOper(name);
}

QString BoolOperWapper::createGBTAdd(BoolOper* obj, const QString& targetName, const QString& toolName)
{
    if (obj == nullptr)
    {
        QString resultText = tr("Failed");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    // 求和
    bool result = obj->createGBTAdd(targetName, toolName);
    if (result == false)
    {
        AppFrame::FITKMessageError("Create GBTAdd failed !");
        QString resultText = "Create GBTAdd failed!";
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    QString resultText = tr("Success, created GBTAdd named %1").arg(obj->getName());
    PythonInterfaceCommons::sendMessageToHttpServer(resultText);
    return resultText;
}
QString BoolOperWapper::createGBTCut(BoolOper* obj, const QString& targetName, const QString& toolName)
{
    if (obj == nullptr)
    {
        QString resultText = tr("Failed");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    // 求差
    bool result = obj->createGBTCut(targetName, toolName);
    if (result == false)
    {
        AppFrame::FITKMessageError("Create GBTCut failed !");
        QString resultText = "Create GBTCut failed!";
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    QString resultText = tr("Success, created GBTCut named %1").arg(obj->getName());
    PythonInterfaceCommons::sendMessageToHttpServer(resultText);
    return resultText;
}
QString BoolOperWapper::createGBTCommon(BoolOper* obj, const QString& targetName, const QString& toolName)
{
    if (obj == nullptr)
    {
        QString resultText = tr("Failed");
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    // 求交
    bool result = obj->createGBTCommon(targetName, toolName);
    if (result == false)
    {
        AppFrame::FITKMessageError("Create GBTCommon failed !");
        QString resultText = "Create GBTCommon failed!";
        PythonInterfaceCommons::sendMessageToHttpServer(resultText);
        return resultText;
    }

    QString resultText = tr("Success, created GBTCommon named %1").arg(obj->getName());
    PythonInterfaceCommons::sendMessageToHttpServer(resultText);
    return resultText;
}

QString BoolOperWapper::getName(BoolOper* obj)
{
    //获取名称
    if (obj == nullptr)
    {
        PythonInterfaceCommons::sendMessageToHttpServer("Get BoolOper name failed!");
        return QString();
    }

    QString result = obj->getName();
    PythonInterfaceCommons::sendMessageToHttpServer(QString("Get BoolOper name success: %1").arg(result));
    return result;
}

QString BoolOperWapper::setName(BoolOper* obj, const QString& name)
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

BoolOper::BoolOper(const QString& name) : PyGeometryBase(name)
{
}

BoolOper::BoolOper() : PyGeometryBase()
{
}

bool BoolOper::createGBTAdd(const QString& targetName, const QString& toolName)
{
    if (_cmdList == nullptr) return false;
    if (targetName.isEmpty() || toolName.isEmpty()) return false;
    // 获取几何命令
    Interface::FITKAbsGeoCommand* targetCmd = _cmdList->getDataByName(targetName);
    Interface::FITKAbsGeoCommand* toolCmd = _cmdList->getDataByName(toolName);
    if (!toolCmd || !targetCmd) return false;
    if (targetCmd->getDataObjectID() == toolCmd->getDataObjectID()) return false;
    // 获取虚形状
    Interface::VirtualShape targetShape = this->getVirtualShapeFromCommand(targetCmd);
    Interface::VirtualShape toolShape = this->getVirtualShapeFromCommand(toolCmd);
    if (toolShape.isNull() || targetShape.isNull())return false;
    QString boolName = _cmdList->checkName("GBTAdd-1");
    //创建几何命令
    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;
    Interface::FITKAbsGeoOperBool* boolCmd = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool));
    if (!boolCmd) return false;
    //设置参数
    boolCmd->setDataObjectName(boolName);
    boolCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GeoBoolOperType::GBTAdd);
    boolCmd->setTarget(targetShape);
    boolCmd->setTool(toolShape);
    if (boolCmd->update() == false) {
        AppFrame::FITKMessageError("Bool operation update failed!");
        return false;
    }
    // 移除命令引用并添加新命令引用。
    boolCmd->clearReference();
    boolCmd->addReferenceCmdID(targetShape.CmdId);
    boolCmd->addReferenceCmdID(toolShape.CmdId);
    _cmdList->appendDataObj(boolCmd);
    _command = boolCmd;

    PyGeometryBase::updateGraphAndTreeEvent();
    return true;
}
bool BoolOper::createGBTCut(const QString& targetName, const QString& toolName)
{
    if (_cmdList == nullptr) return false;
    if (targetName.isEmpty() || toolName.isEmpty()) return false;
    // 获取几何命令
    Interface::FITKAbsGeoCommand* targetCmd = _cmdList->getDataByName(targetName);
    Interface::FITKAbsGeoCommand* toolCmd = _cmdList->getDataByName(toolName);
    if (!toolCmd || !targetCmd) return false;
    if (targetCmd->getDataObjectID() == toolCmd->getDataObjectID()) return false;
    // 获取虚形状
    Interface::VirtualShape targetShape = this->getVirtualShapeFromCommand(targetCmd);
    Interface::VirtualShape toolShape = this->getVirtualShapeFromCommand(toolCmd);
    if (toolShape.isNull() || targetShape.isNull())return false;
    QString boolName = _cmdList->checkName("GBTAdd-1");
    //创建几何命令
    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;
    Interface::FITKAbsGeoOperBool* boolCmd = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool));
    if (!boolCmd) return false;
    //设置参数
    boolCmd->setDataObjectName(boolName);
    boolCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GeoBoolOperType::GBTCut);
    boolCmd->setTarget(targetShape);
    boolCmd->setTool(toolShape);
    if (boolCmd->update() == false) {
        AppFrame::FITKMessageError("Bool operation update failed!");
        return false;
    }
    // 移除命令引用并添加新命令引用。
    boolCmd->clearReference();
    boolCmd->addReferenceCmdID(targetShape.CmdId);
    boolCmd->addReferenceCmdID(toolShape.CmdId);
    _cmdList->appendDataObj(boolCmd);
    _command = boolCmd;

    PyGeometryBase::updateGraphAndTreeEvent();
    return true;
}
bool BoolOper::createGBTCommon(const QString& targetName, const QString& toolName)
{
    if (_cmdList == nullptr) return false;
    if (targetName.isEmpty() || toolName.isEmpty()) return false;
    // 获取几何命令
    Interface::FITKAbsGeoCommand* targetCmd = _cmdList->getDataByName(targetName);
    Interface::FITKAbsGeoCommand* toolCmd = _cmdList->getDataByName(toolName);
    if (!toolCmd || !targetCmd) return false;
    if (targetCmd->getDataObjectID() == toolCmd->getDataObjectID()) return false;
    // 获取虚形状
    Interface::VirtualShape targetShape = this->getVirtualShapeFromCommand(targetCmd);
    Interface::VirtualShape toolShape = this->getVirtualShapeFromCommand(toolCmd);
    if (toolShape.isNull() || targetShape.isNull())return false;
    QString boolName = _cmdList->checkName("GBTAdd-1");
    //创建几何命令
    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;
    Interface::FITKAbsGeoOperBool* boolCmd = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool));
    if (!boolCmd) return false;
    //设置参数
    boolCmd->setDataObjectName(boolName);
    boolCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GeoBoolOperType::GBTCommon);
    boolCmd->setTarget(targetShape);
    boolCmd->setTool(toolShape);
    if (boolCmd->update() == false) {
        AppFrame::FITKMessageError("Bool operation update failed!");
        return false;
    }
    // 移除命令引用并添加新命令引用。
    boolCmd->clearReference();
    boolCmd->addReferenceCmdID(targetShape.CmdId);
    boolCmd->addReferenceCmdID(toolShape.CmdId);
    _cmdList->appendDataObj(boolCmd);
    _command = boolCmd;

    PyGeometryBase::updateGraphAndTreeEvent();
    return true;
}

Interface::VirtualShape BoolOper::getVirtualShapeFromCommand(Interface::FITKAbsGeoCommand* cmd)
{
    if (!cmd) return Interface::VirtualShape();
    // 获取几何虚拟拓扑管理器
    Interface::FITKShapeVirtualTopoManager* vSolidMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSSolid);
    if (!vSolidMgr) return Interface::VirtualShape();
    // 获取虚拓扑
    Interface::FITKAbsVirtualTopo* vTopo = vSolidMgr->getDataByIndex(0);
    if (!vTopo) return Interface::VirtualShape();
    // 创建虚拓扑形状
    Interface::VirtualShape vShape;
    vShape.CmdId = cmd->getDataObjectID();
    vShape.VirtualTopoId = vTopo->getDataObjectID();
    vShape.VirtualTopoIndex = 0;
    vShape.Type = Interface::FITKGeoEnum::VSSolid;
    return vShape;
}
