/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ModelSetWapper.h"

#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"

#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

#include <QDebug>

ModelSet* ModelSetWapper::new_ModelSet()
{
    //创建ModelSet实例
    return new ModelSet;
}

void ModelSetWapper::delete_ModelSet(ModelSet* obj)
{
    //释放ModelSet实例
    if (obj) delete obj;
}

ModelSet* ModelSetWapper::static_ModelSet_GetModelSet(const QString& name)
{
    //通过名称获取模型集对象
    if (name.isEmpty()) return nullptr;
    ModelSet* modelSet = new ModelSet(name);
    return modelSet;
}

void ModelSetWapper::createModelSet(ModelSet* obj, int type, const QVariantList& member)
{
    //创建模型集合对象
    if (!obj) return;
    bool res = obj->createModelSet(type, member);
    if (!res) {
        AppFrame::FITKMessageError("Create ModelSet failed !");
        return;
    }
    updateGraphAndTreeEvent(obj->_modelSetData->getDataObjectID());
}

void ModelSetWapper::createModelSet(ModelSet* obj, int type, int m_minID, int m_maxID, int m_interval)
{
    //创建模型集合对象
    if (!obj) return;
    QVariantList member;
    int id = m_minID;
    for (int i = 0; id < m_maxID; ++i)
    {
        id = m_minID + i * m_interval;
        member.append(id);
    }
    bool res = obj->createModelSet(type, member);
    if (!res) {
        AppFrame::FITKMessageError("Create ModelSet failed !");
        return;
    }
    updateGraphAndTreeEvent(obj->_modelSetData->getDataObjectID());
}

QString ModelSetWapper::getName(ModelSet* obj)
{
    //获取名称
    if (!obj) return QString();
    return obj->getName();
}

void ModelSetWapper::setName(ModelSet* obj, const QString& name)
{
    //设置名称并更新树形事件
    if (!obj) return;
    bool ok = obj->setName(name);
    if (!ok) {
        AppFrame::FITKMessageError("Set Name failed !");
    }
    updateGraphAndTreeEvent(obj->_modelSetData->getDataObjectID());
}

void ModelSetWapper::updateGraphAndTreeEvent(int dataObjId)
{
    //更新图形和树形事件
    bool isGrapFitView = true;
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

ModelSet::ModelSet(const QString& name) : ModelSet()
{
    //通过名称获取模型集数据
    if (_componentManager)
        _modelSetData = dynamic_cast<Interface::FITKModelSet*>(_componentManager->getDataByName(name));
}

ModelSet::ModelSet()
{
    //初始化成员变量
    Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr)  return;
    _meshModel = caseData->getMeshModel();
    if (_meshModel == nullptr)  return;
    _componentManager = _meshModel->getComponentManager();
}

bool ModelSet::createModelSet(int type, const QVariantList& member)
{
    //创建模型集合
    if (!_componentManager || !_meshModel || member.isEmpty()) return false;
    switch (type)
    {
    case 0: //节点集合
        _modelSetData = new Radioss::FITKRadiossNodeGroup();
        break;
    case 1: {
        _modelSetData = new Radioss::FITKRadiossElementGroup();
        Radioss::FITKRadiossElementGroup* radSet = dynamic_cast<Radioss::FITKRadiossElementGroup*>(_modelSetData);
        radSet->setElementGroupType(Radioss::FITKRadiossElementGroup::ElementGroupType::GRSHEL);
        break;
    }
    default:
        break;
    }
    if (!_modelSetData) return false;
    //检查名称是否重复，获取一个不重复的名称
    QString name =_componentManager->checkName("Set-1");
    _modelSetData->setDataObjectName(name);
    //将集合添加到组件管理器中，并设置模型ID和内部标志
    _componentManager->appendDataObj(_modelSetData);
    _modelSetData->setModel(_meshModel->getDataObjectID());
    _modelSetData->setInternal(false);
    //将成员添加到集合中
    for (int i = 0; i < member.size(); i++)
    {
        bool ok = false;
        int id = member[i].toInt(&ok);
        if (!ok) continue;
        _modelSetData->appendMember(id);
    }
    return true;
}

QString ModelSet::getName()
{
    //获取名称
    if (!_modelSetData)
        return QString();
    return _modelSetData->getDataObjectName();
}

bool ModelSet::setName(const QString& name)
{
    //设置名称
    if (!_modelSetData || !_componentManager)
        return false;
    if (_modelSetData->getDataObjectName() == name) return true;
    if (_componentManager->getDataByName(name)) {
        return false;
    }
    _modelSetData->setDataObjectName(name);
    return true;
}