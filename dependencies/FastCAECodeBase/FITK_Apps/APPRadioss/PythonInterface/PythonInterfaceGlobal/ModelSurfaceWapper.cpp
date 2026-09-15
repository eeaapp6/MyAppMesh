/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ModelSurfaceWapper.h"

#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"

#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include <QDebug>

ModelSurface* ModelSurfaceWapper::new_ModelSurface()
{
    //创建ModelSurface实例
    return new ModelSurface;
}

void ModelSurfaceWapper::delete_ModelSurface(ModelSurface* obj)
{
    //释放ModelSurface实例
    if (obj) delete obj;
}

ModelSurface* ModelSurfaceWapper::static_ModelSurface_GetModelSurface(const QString& name)
{
    //通过名称获取模型面对象
    if (name.isEmpty()) return nullptr;
    ModelSurface* modelSurface = new ModelSurface(name);
    return modelSurface;
}

QString ModelSurfaceWapper::getName(ModelSurface* obj)
{
    //获取名称
    if (!obj) return QString();
    return obj->getName();
}

void ModelSurfaceWapper::setName(ModelSurface* obj, const QString& name)
{
    //设置名称
    if (!obj) return;
    bool ok = obj->setName(name);
    if (!ok) {
        AppFrame::FITKMessageError("Set Name failed !");
    }
}

void ModelSurfaceWapper::updateGraphAndTreeEvent(int dataObjId)
{
    //更新图形和树形事件
}

ModelSurface::ModelSurface(const QString& name) : ModelSurface()
{
    //通过名称获取模型面数据
    if (_componentManager)
        _modelSurfaceData = dynamic_cast<Interface::FITKMeshSurface*>(_componentManager->getDataByName(name));
}

ModelSurface::ModelSurface()
{
    //初始化成员变量
    Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)  return;

    Radioss::FITKRadiossMeshModel* meshModel = caseData->getMeshModel();
    if (meshModel == nullptr)  return;
    _componentManager = meshModel->getComponentManager();
}

QString ModelSurface::getName()
{
    //获取名称
    if (!_modelSurfaceData)
        return QString();
    return _modelSurfaceData->getDataObjectName();
}

bool ModelSurface::setName(const QString& name)
{
    //设置名称
    if (!_modelSurfaceData || !_componentManager)
        return false;
    if (_modelSurfaceData->getDataObjectName() == name) return true;
    if (_componentManager->getDataByName(name)) {
        return false;
    }
    _modelSurfaceData->setDataObjectName(name);
    return true;
}