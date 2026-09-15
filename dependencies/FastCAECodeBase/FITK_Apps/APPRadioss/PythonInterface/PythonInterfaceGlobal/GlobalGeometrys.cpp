/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GlobalGeometrys.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCylinder.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSphere.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossGeomPart.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include <QDebug>

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

Geometrys* GeometrysWapper::new_Geometrys()
{
    return new Geometrys;
}

void GeometrysWapper::delete_Geometrys(Geometrys* obj)
{
    if (obj) delete obj;
}

Geometrys* GeometrysWapper::static_Geometrys_GetGeometry(const QString& name)
{
    if (name.isEmpty()) return nullptr;
    //通过名称获取几何对象
    Geometrys* geo = new Geometrys(name);
    return geo;
}

QString GeometrysWapper::createBox(Geometrys* obj, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l)
{
    if (!obj) return sendMessageToServer(tr("Failed, object is null"));
    //创建长方体
    bool result = obj->createBox("Box-1", x_p, y_p, z_p, x_l, y_l, z_l);
    if (!result) {
        delete obj->_geometryData;
        AppFrame::FITKMessageError("Create Box failed !");
        return sendMessageToServer(tr("Failed, unable to create box geometry"));
    }
    this->updateGraphAndTreeEvent(obj->_geometryData->getGeoPartID());

    return sendMessageToServer(tr("Success, created box named %1").arg(obj->_geometryData->getDataObjectName()));
}

QString GeometrysWapper::createSphere(Geometrys* obj, double x_p, double y_p, double z_p, double radius)
{
    if (!obj) return sendMessageToServer(tr("Failed, object is null"));
    //创建球体
    bool result = obj->createSphere("Sphere-1", x_p, y_p, z_p, radius);
    if (!result) {
        delete obj->_geometryData;
        AppFrame::FITKMessageError("Create Sphere failed !");
        return sendMessageToServer(tr("Failed, unable to create sphere geometry"));
    }
    this->updateGraphAndTreeEvent(obj->_geometryData->getGeoPartID());
    return sendMessageToServer(tr("Success, created sphere named %1").arg(obj->_geometryData->getDataObjectName()));
}

QString GeometrysWapper::createCylinder(Geometrys* obj, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length)
{
    if (!obj) return sendMessageToServer(tr("Failed, object is null"));
    //创建圆柱体
    bool result = obj->createCylinder("Cylinder-1", x_c, y_c, z_c, x_d, y_d, z_d, radius, length);
    if (!result) {
        delete obj->_geometryData;
        AppFrame::FITKMessageError("Create Cylinder failed !");
        return sendMessageToServer(tr("Failed, unable to create cylinder geometry"));
    }
    this->updateGraphAndTreeEvent(obj->_geometryData->getGeoPartID());
    return sendMessageToServer(tr("Success, created cylinder named %1").arg(obj->_geometryData->getDataObjectName()));
}


QString GeometrysWapper::getName(Geometrys* obj)
{
    //获取名称
    if (!obj) return QString();
    return obj->getName();
}

QString GeometrysWapper::setName(Geometrys* obj, const QString& name)
{
    //设置名称
    if (!obj) return sendMessageToServer(tr("Failed, object is null"));
    bool ok = obj->setName(name);
    if (!ok) {
        AppFrame::FITKMessageError("Set Name failed !");
        return sendMessageToServer(tr("Failed, geometry name %1 may already exist or geometry is invalid").arg(name));
    }
    this->updateAssemblyTreeEvent();
    return sendMessageToServer(tr("Success, name set to %1").arg(name));
}

void GeometrysWapper::updateAssemblyTreeEvent()
{
    //通过树形菜单事件处理器刷新树
    QStringList listOpertor = { "TreeAssemblyEvent" };
    for (int i = 0; i < listOpertor.size(); i++)
    {
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr) oper->updateTree();
    }
}

void GeometrysWapper::updateGraphAndTreeEvent(int dataObjId)
{
    Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
    if (cmdList == nullptr) return;
    bool isGrapFitView = cmdList->getDataCount() == 1 ? true : false;
    //更新渲染
    EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph)
    {
        operGraph->updateGraph(dataObjId, true, isGrapFitView);
    }
    //通过树形菜单事件处理器刷新树
    QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
    for (int i = 0; i < listOpertor.size(); i++)
    {
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr) oper->updateTree();
    }
}

Geometrys::Geometrys(const QString& name) : Geometrys()
{
    //通过名称获取几何对象
    if (_radiossGeomPartManager)
        _geometryData = _radiossGeomPartManager->getDataByName(name);
}

Geometrys::Geometrys()
{
    //初始化成员变量
    Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr) return;
    _cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
    _radiossGeomPartManager = radiossCase->getGeomPartManager();
}

QString Geometrys::getName()
{
    //获取名称
    if (!_geometryData)
        return QString();
    return _geometryData->getDataObjectName();
}

bool Geometrys::setName(const QString& name)
{
    //设置名称
    if (!_geometryData || !_radiossGeomPartManager)
        return false;
    if (_geometryData->getDataObjectName() == name) return true;
    if (_radiossGeomPartManager->getDataByName(name)) {
        return false;
    }
    _geometryData->setDataObjectName(name);
    return true;
}

bool Geometrys::createBox(const QString& name, double x_p, double y_p, double z_p, double x_l, double y_l, double z_l)
{
    if (_cmdList == nullptr || _radiossGeomPartManager == nullptr) return false;
    //检查名称是否有效并获取唯一名称
    QString uniqueName = _radiossGeomPartManager->checkName(name);
    //创建几何部件并添加到几何部件管理器中
    _geometryData = new Radioss::FITKRadiossGeomPart();
    _geometryData->setDataObjectName(uniqueName);
    _radiossGeomPartManager->appendDataObj(_geometryData);
    //获取几何部件
    Interface::FITKAbsGeoPart* geoPart = _geometryData->getGeoPart();
    if (!geoPart) return false;
    geoPart->setDataObjectName(uniqueName);
    //获取几何工厂创建长方体命令对象
    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;
    //创建长方体命令对象并设置名称
    Interface::FITKAbsGeoModelBox* obj = dynamic_cast<Interface::FITKAbsGeoModelBox*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTBox));
    if (!obj) return false;
    double point1[3] = { x_p, y_p, z_p };
    double length[3] = { x_l, y_l, z_l };
    obj->setDataObjectName(uniqueName);
    obj->setPoint1(point1);
    obj->setLength(length);
    if (geoPart->addAndUpdate(obj) == false)
    {
        return false;
    }
    return true;
}

bool Geometrys::createSphere(const QString& name, double x_c, double y_c, double z_c, double radius)
{
    if (_cmdList == nullptr || _radiossGeomPartManager == nullptr) return false;
    //检查名称是否有效并获取唯一名称
    QString uniqueName = _radiossGeomPartManager->checkName(name);
    //创建几何部件并添加到几何部件管理器中
    _geometryData = new Radioss::FITKRadiossGeomPart();
    _geometryData->setDataObjectName(uniqueName);
    _radiossGeomPartManager->appendDataObj(_geometryData);
    //获取几何部件
    Interface::FITKAbsGeoPart* geoPart = _geometryData->getGeoPart();
    if (!geoPart) return false;
    geoPart->setDataObjectName(uniqueName);
    //获取几何工厂创建长方体命令对象
    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;
    //创建长方体命令对象并设置名称
    Interface::FITKAbsGeoModelSphere* obj = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTSphere));
    if (!obj) return false;
    double center[3] = { x_c, y_c, z_c };
    obj->setDataObjectName(uniqueName);
    obj->setLocation(center);
    obj->setRadius(radius);
    if (geoPart->addAndUpdate(obj) == false)
    {
        return false;
    }
    return true;
}

bool Geometrys::createCylinder(const QString& name, double x_c, double y_c, double z_c, double x_d, double y_d, double z_d, double radius, double length)
{
    if (_cmdList == nullptr || _radiossGeomPartManager == nullptr) return false;
    //检查名称是否有效并获取唯一名称
    QString uniqueName = _radiossGeomPartManager->checkName(name);
    //创建几何部件并添加到几何部件管理器中
    _geometryData = new Radioss::FITKRadiossGeomPart();
    _geometryData->setDataObjectName(uniqueName);
    _radiossGeomPartManager->appendDataObj(_geometryData);
    //获取几何部件
    Interface::FITKAbsGeoPart* geoPart = _geometryData->getGeoPart();
    if (!geoPart) return false;
    geoPart->setDataObjectName(uniqueName);
    //获取几何工厂创建长方体命令对象
    Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (!geoFactory) return false;
    //创建长方体命令对象并设置名称
    Interface::FITKAbsGeoModelCylinder* obj = dynamic_cast<Interface::FITKAbsGeoModelCylinder*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTCylinder));
    if (!obj) return false;
    double point1[3] = { x_c, y_c, z_c };
    double direction[3] = { x_d, y_d, z_d };
    obj->setDataObjectName(uniqueName);
    obj->setLocation(point1);
    obj->setDirection(direction);
    obj->setRadius(radius);
    obj->setLength(length);
    if (geoPart->addAndUpdate(obj) == false)
    {
        return false;
    }
    return true;
}