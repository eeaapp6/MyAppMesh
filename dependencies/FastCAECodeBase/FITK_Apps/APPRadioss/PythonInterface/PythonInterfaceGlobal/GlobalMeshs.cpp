/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GlobalMeshs.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"

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

Meshs* MeshsWapper::new_Meshs()
{
    return new Meshs;
}

void MeshsWapper::delete_Meshs(Meshs* obj)
{
    if (obj) delete obj;
}

Meshs* MeshsWapper::static_Meshs_GetMesh(const QString& name)
{
    if (name.isEmpty()) return nullptr;
    //通过名称获取网格部件对象
    Meshs* mesh = new Meshs(name);
    return mesh;
}

QString MeshsWapper::static_Meshs_MeshToSPHParticleConversion(const QString& name)
{
    if (name.isEmpty()) return nullptr;
    //通过名称获取网格部件对象
    Meshs* mesh = new Meshs(name);
    bool ok = mesh->toSPHParticle();
    if (!ok) {
        AppFrame::FITKMessageError("Mesh to SPH Particle Conversion failed !");
        return sendMessageToServer(tr("Failed, mesh %1 may be invalid").arg(name));
    }
    this->updateGraphAndTreeEvent(mesh->_meshPart->getDataObjectID());
    return sendMessageToServer(tr("Success, mesh %1 converted to SPH particles").arg(name));
}

QString MeshsWapper::getName(Meshs* obj)
{
    //获取名称
    if (!obj) return QString();
    return obj->getName();
}

QString MeshsWapper::setName(Meshs* obj, const QString& name)
{
    //设置名称
    if (!obj) return sendMessageToServer(tr("Failed, object is null"));
    bool ok = obj->setName(name);
    if (!ok) {
        AppFrame::FITKMessageError("Set Name failed !");
        return sendMessageToServer(tr("Failed, mesh name %1 may already exist or mesh is invalid").arg(name));
    }
    this->updateAssemblyTreeEvent();
    return sendMessageToServer(tr("Success, name set to %1").arg(name));
}

QString MeshsWapper::setProperty(Meshs* obj, const QString& propertyName)
{
    //设置属性
    if (!obj) return sendMessageToServer(tr("Failed, object is null"));
    bool ok = obj->setProperty(propertyName);
    if (!ok) {
        AppFrame::FITKMessageError("Set Property failed !");
        return sendMessageToServer(tr("Failed, property %1 was not found or mesh is invalid").arg(propertyName));
    }
    this->updateAssemblyTreeEvent();
    return sendMessageToServer(tr("Success, property set to %1").arg(propertyName));
}

QString MeshsWapper::setMaterial(Meshs* obj, const QString& materialName)
{
    //设置材料
    if (!obj) return sendMessageToServer(tr("Failed, object is null"));
    bool ok = obj->setMaterial(materialName);
    if (!ok) {
        AppFrame::FITKMessageError("Set Material failed !");
        return sendMessageToServer(tr("Failed, material %1 was not found or mesh is invalid").arg(materialName));
    }
    this->updateAssemblyTreeEvent();
    return sendMessageToServer(tr("Success, material set to %1").arg(materialName));
}

void MeshsWapper::updateAssemblyTreeEvent()
{
    //更新装配树形事件
    EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator("TreeAssemblyEvent"));
    if (oper) oper->updateTree();
}

void MeshsWapper::updateGraphAndTreeEvent(int dataObjId)
{
    //更新图形和树形事件
    bool isGrapFitView = false;
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

Meshs::Meshs(const QString& name) : Meshs()
{
    //通过名称获取网格部件对象
    if (_radiossPartManager)
        _meshPart = _radiossPartManager->getDataByName(name);
}

Meshs::Meshs()
{
    //初始化成员变量
    Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr) return;
    _meshData = radiossCase->getMeshModel();
    _materialManager = radiossCase->getMaterialManager();
    _sectionManager = radiossCase->getSectionManager();
    if (_meshData)
        _radiossPartManager = _meshData->getPartsManager();
}

QString Meshs::getName()
{
    //获取名称
    if (!_meshPart)
        return QString();
    return _meshPart->getDataObjectName();
}

bool Meshs::setName(const QString& name)
{
    //设置名称
    if (!_meshPart || !_radiossPartManager)
        return false;
    if (_meshPart->getDataObjectName() == name) return true;
    if (_radiossPartManager->getDataByName(name)) {
        return false;
    }
    _meshPart->setDataObjectName(name);
    return true;
}

bool Meshs::setProperty(const QString& propertyName)
{
    //设置属性
    if (!_meshPart || !_sectionManager) return false;
    Interface::FITKAbstractSection* prop = _sectionManager->getDataByName(propertyName);
    if (!prop) return false;
    int propID = prop->getDataObjectID();
    _meshPart->setPropertyID(propID);
    return true;
}

bool Meshs::setMaterial(const QString& materialName)
{
    //设置材料
    if (!_meshPart || !_materialManager) return false;
    Interface::FITKAbstractMaterial* material = _materialManager->getDataByName(materialName);
    if (!material) return false;
    int matID = material->getDataObjectID();
    _meshPart->setMaterialID(matID);
    return true;
}

bool Meshs::toSPHParticle()
{
    if (!_meshPart) return false;
    _meshPart->transformationSPHModel();
    return true;
}
