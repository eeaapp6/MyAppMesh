/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreePropertyAction.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include <QTreeWidgetItem>

namespace GUIOper
{
    OperTreePropertyAction::~OperTreePropertyAction()
    {
        // 重置选中节点的数据ID
        _selectedItemDataID = -1;
    }

    bool OperTreePropertyAction::execGUI()
    {
        // 检查触发器是否为空，如果为空则返回false
        if (_emitter == nullptr) return false;

        // 获取触发器的对象名称
        QString name = _emitter->objectName();
        if (name.isEmpty()) return false;

        // 获取Radioss案例数据对象
        Radioss::FITKRadiossCase* dataCase = getRadiossCase();
        if (dataCase == nullptr) return false;

        // 初始化参数哈希表
        QHash<QString, QVariant> args = QHash<QString, QVariant>();
        args.insert("SelectedItemDataID", _selectedItemDataID);
        
        // 根据不同的对象名称调用相应的操作函数
        // Material
        if (name == "actionPropertyTreeCreateMaterial") callOperator("actionMaterialPropCreate");
        else if (name == "actionPropertyTreeRenameMaterial") renameData<Interface::FITKMaterialManager*>(this, dataCase->getMaterialManager(), _selectedItemDataID);
        else if (name == "actionPropertyTreeEditMaterial") callOperator("actionMaterialPropEdit", args);
        else if (name == "actionPropertyTreeDeleteMaterial") deleteData(dataCase->getMaterialManager(), _selectedItemDataID);
        // Property
        else if (name == "actionPropertyTreeCreateProperty") callOperator("actionPropertyCreate");
        else if (name == "actionPropertyTreeRenameProperty") renameData<Interface::FITKAbstractSectionManager*>(this, dataCase->getSectionManager(), _selectedItemDataID);
        else if (name == "actionPropertyTreeEditProperty") callOperator("actionPropertyEdit", args);
        else if (name == "actionPropertyTreeDeleteProperty") deleteData(dataCase->getSectionManager(), _selectedItemDataID);
        // Curve
        else if (name == "actionPropertyTreeCreateCurve") callOperator("actionCurveCreate");
        else if (name == "actionPropertyTreeRenameCurve") renameData<Radioss::FITKNonLinerCurveManager*>(this, dataCase->getNonLinerCurveManager(), _selectedItemDataID);
        else if (name == "actionPropertyTreeEditCurve") callOperator("actionCurveEdit", args);
        else if (name == "actionPropertyTreeDeleteCurve") deleteData(dataCase->getNonLinerCurveManager(), _selectedItemDataID);
        // EOS
        else if (name == "actionPropertyTreeCreateEOS") callOperator("actionEOSCreate");
        else if (name == "actionPropertyTreeRenameEOS") renameData<Radioss::FITKEquationOfStateManager*>(this, dataCase->getEquationOfState(), _selectedItemDataID);
        else if (name == "actionPropertyTreeEditEOS") callOperator("actionEOSEdit", args);
        else if (name == "actionPropertyTreeDeleteEOS") deleteData(dataCase->getEquationOfState(), _selectedItemDataID);
        // Failure Model
        else if (name == "actionPropertyTreeCreateFailureModel") callOperator("actionFailureModelCreate");
        else if (name == "actionPropertyTreeRenameFailureModel") renameData<Radioss::FITKRadiossFailureModelManager*>(this, dataCase->getFailureModelManager(), _selectedItemDataID);
        else if (name == "actionPropertyTreeEditFailureModel") callOperator("actionFailureModelEdit", args);
        else if (name == "actionPropertyTreeDeleteFailureModel") deleteData(dataCase->getFailureModelManager(), _selectedItemDataID);
        // Other
        else return false;

        // 清除高亮显示
        clearAllHighLight();

        return true;
    }

    void OperTreePropertyAction::preArgs()
    {
        // 初始化选中节点的数据ID
        _selectedItemDataID = -1;

        // 获取选中项列表参数
        QVariant selectedItems;
        this->argValue("SelectedItems", selectedItems);
        if (selectedItems.isNull()) return;

        // 将参数转换为TreeWidgetItem列表指针并检查有效性
        QList<QTreeWidgetItem*>* listSelectedItems = static_cast<QList<QTreeWidgetItem*>*>(selectedItems.value<void*>());
        if (listSelectedItems == nullptr || listSelectedItems->isEmpty()) return;

        // 获取第一个选中的项的ID
        _selectedItemDataID = listSelectedItems->first()->data(1, 0).toInt();
    }
}