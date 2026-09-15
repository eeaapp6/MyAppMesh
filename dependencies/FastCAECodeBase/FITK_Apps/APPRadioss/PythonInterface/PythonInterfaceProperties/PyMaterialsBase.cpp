/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyMaterialsBase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"


namespace PyInterface
{
    PyMaterialsBase::PyMaterialsBase(const QString& name)
    {
         // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;
        // 获取材料管理器
        _materialManager = dataCase->getMaterialManager();
        if (_materialManager == nullptr) return;

        Interface::FITKAbstractMaterial* material = _materialManager->getDataByName(name);
        if (!material)return;
        //获取对象
        _material = dynamic_cast<Radioss::FITKAbstractRadMaterial*>(material);
    }

    PyMaterialsBase::PyMaterialsBase()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;
        _materialManager = dataCase->getMaterialManager();
    }

    QString PyMaterialsBase::getName()
    {
        if (_material)
            return _material->getDataObjectName();
        return QString();
    }

    bool PyMaterialsBase::setName(const QString& name)
    {
        //空字符直接返回
        if (name == "" || !_material || !_materialManager)
            return false;
        //获取名称-查重处理
        if (_materialManager->getDataByName(name) && _material->getDataObjectName() != name)
            return false;
        _material->setDataObjectName(name);
        return true;
    }

    bool PyMaterialsBase::createMaterial(const QString &name, int type)
    {
        if(!_materialManager)
            return false;
        QString materialName = name;
        //生成默认名称
        if (materialName == "")
        {
            materialName = _materialManager->checkName("Material-1");
        }
        if(_materialManager->getDataByName(materialName))
            return false;


        switch(type)
        {
        case Radioss::FITKAbstractRadMaterial::LAW1:
            _material = new Radioss::FITKMaterialElastic();
            break;

        case Radioss::FITKAbstractRadMaterial::LAW6:
            _material = new Radioss::FITKMaterialHydro();
            break;

        case Radioss::FITKAbstractRadMaterial::LAW2:
            _material = new Radioss::FITKMaterialPlasJohns();
            break;

        case Radioss::FITKAbstractRadMaterial::LAW27:
            _material = new Radioss::FITKMaterialPlasBrit();
            break;

        case Radioss::FITKAbstractRadMaterial::LAW36:
            _material = new Radioss::FITKMaterialPlasTab();
            break;

        default:
			return false;
        }

        if(!_material)
            return false;
        _material->setDataObjectName(materialName);
        _materialManager->appendDataObj(_material);

		updateRender();
        return true;
    }

    Radioss::FITKAbstractRadMaterial* PyMaterialsBase::getMaterial()
    {
        return _material;
	}

    void PyMaterialsBase::updateRender()
    {
        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = {"TreePropertyEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
    }
}