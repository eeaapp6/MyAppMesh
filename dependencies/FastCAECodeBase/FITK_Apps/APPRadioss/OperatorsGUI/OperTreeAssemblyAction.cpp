/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeAssemblyAction.h"
#include "GUIWidget/GUIEnumType.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandProp.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIDialog/GUIToolDialog/GUIBatchSettingDialog.h"
#include <QTreeWidgetItem>
#include <QColorDialog>

namespace GUIOper
{
    OperTreeAssemblyAction::~OperTreeAssemblyAction()
    {
        // 清空记录的选中项信息
        _selectedGeometryIDs.clear();
        _selectedPartIDs.clear();
    }

    bool OperTreeAssemblyAction::execGUI()
    {
        // 检查触发器是否为空，如果为空则返回false
        if (_emitter == nullptr) return false;

        // 获取触发器的对象名称
        QString name = _emitter->objectName();
        // 如果名称为空，则返回false
        if (name.isEmpty()) return false;

        // 根据不同的对象名称调用相应的操作函数
        if (name == "actionAssemblyTreeCreateBox") callOperator("actionCreateBox");
        else if (name == "actionAssemblyTreeCreateCylinder") callOperator("actionCreateCylinder");
        else if (name == "actionAssemblyTreeCreateSphere") callOperator("actionCreateSphere");
        else if (name == "actionAssemblyTreeCreatePart") callOperator("actionPartCreate");
        else if (name == "actionAssemblyTreeSetColor") setColor();
        else if (name == "actionAssemblyTreeSetMaterial") setMaterialProperty(0);
        else if (name == "actionAssemblyTreeSetProperty") setMaterialProperty(1);
        else if (name == "actionAssemblyTreeSPHTrans") modelSPHTransformation();
        else if (name == "actionAssemblyTreeShowModel") showModel(true);
        else if (name == "actionAssemblyTreeHideModel") showModel(false);
        else if (name == "actionAssemblyTreeRename") renameModel();
        else if (name == "actionAssemblyTreeDelete") deleteModel();
        else return false;

        // 清除高亮显示
        clearAllHighLight();
        // 更新树
        updateTree();

        return true;
    }

    void OperTreeAssemblyAction::preArgs()
    {
        // 清空之前保存的选中项名称
        _selectedGeometryIDs.clear();
        _selectedPartIDs.clear();

        // 获取选中项列表参数
        QVariant selectedItems;
        this->argValue("SelectedItems", selectedItems);
        if (selectedItems.isNull()) return;

        // 将参数转换为TreeWidgetItem列表指针并检查有效性
        QList<QTreeWidgetItem*>* listSelectedItems = static_cast<QList<QTreeWidgetItem*>*>(selectedItems.value<void*>());
        if (listSelectedItems == nullptr || listSelectedItems->isEmpty()) return;

        // 遍历所有选中项
        for (QTreeWidgetItem* item : *listSelectedItems)
        {
            // 检查项有效性
            if (item == nullptr) continue;

            // 获取项类型
            int type = item->type();
            // 如果是数据则根据类型保存其ID
            if (type == GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild) _selectedGeometryIDs.append(item->data(1, 0).toInt());
            else if (type == GUI::TreeWidgetAssemblyType::TWAssemblyPartChild) _selectedPartIDs.append(item->data(1, 0).toInt());
        }
    }

    void OperTreeAssemblyAction::setColor()
    {
        // 选取颜色
        QColor color = QColorDialog::getColor(QColor(255, 255, 255), nullptr, QObject::tr("Select Color"));
        if (!color.isValid()) return;

        // 获取几何管理器
        Interface::FITKGeoCommandList* managerGeometry = getGeometryManager();
        if (managerGeometry == nullptr) return;

        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;

        // 遍历所有选中的几何ID
        for (int id : _selectedGeometryIDs)
        {
            // 获取数据对象
            Interface::FITKAbsGeoCommand* data = managerGeometry->getDataByID(id);
            if (data == nullptr) continue;

            // 获取属性
            Interface::FITKGeoCommandProp* dataProperty = data->getCommandProp();
            if (dataProperty == nullptr) return;

            dataProperty->setColor(color);

            oper->updateAllColor(id, color);
        }

        // 获取部件管理器
        Radioss::FITKRadiossPartManager* managerPart = getPartManager();
        if (managerPart == nullptr) return;

        // 遍历所有选中的部件ID
        for (int id : _selectedPartIDs)
        {
            // 获取数据对象
            Radioss::FITKRadiossPart* data = managerPart->getDataByID(id);
            if (data == nullptr) continue;

            data->setColor(color);

            oper->updateAllColor(id, color);
        }
    }

    void OperTreeAssemblyAction::setMaterialProperty(int model)
    {
        if (model != 0 && model != 1) return;

        this->setArgs("ObjectDataID", -2);

        GUI::GUIBatchSettingDialog dialog(this);
        dialog.setModel(model);
        dialog.exec();

        int dataID = -2;
        this->argValue("ObjectDataID", dataID);
        if (dataID < -1) return;

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;
        // 获取网格模型
        Radioss::FITKRadiossMeshModel* mesh = dataCase->getMeshModel();
        if (mesh == nullptr) return;

        // 获取部件管理器
        Radioss::FITKRadiossPartManager* managerPart = mesh->getPartsManager();
        if (managerPart == nullptr) return;

        for (int id : _selectedPartIDs)
        {
            // 获取部件数据
            Radioss::FITKRadiossPart* data = managerPart->getDataByID(id);
            if (data == nullptr) continue;

            if (model == 0) data->setMaterialID(dataID);
            else if (model == 1) data->setPropertyID(dataID);
        }
    }

    void OperTreeAssemblyAction::modelSPHTransformation()
    {
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;
        // 获取部件管理器
        Radioss::FITKRadiossPartManager* managerPart = getPartManager();
        if (managerPart == nullptr) return;
        // 遍历所有选中的部件ID
        for (int id : _selectedPartIDs)
        {
            // 获取数据对象
            Radioss::FITKRadiossPart* data = managerPart->getDataByID(id);
            if (data == nullptr) continue;
            data->transformationSPHModel();
            oper->updateGraph(id, true);
        }
    }

    void OperTreeAssemblyAction::showModel(bool isShow)
    {
        // 通过模拟双击事件删除数据
        EventOper::TreeEventOperator* operEvent = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator("PartTreeEvent"));
        if (operEvent != nullptr) operEvent->on_itemDoubleClicked(nullptr, nullptr, 0);

        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;

        // 获取几何管理器
        Interface::FITKGeoCommandList* managerGeometry = getGeometryManager();
        if (managerGeometry == nullptr) return;

        // 遍历所有选中的几何ID
        for (int id : _selectedGeometryIDs)
        {
            // 获取数据对象
            Interface::FITKAbsGeoCommand* data = managerGeometry->getDataByID(id);
            if (data == nullptr) continue;
            // 设置数据对象是否可见
            data->enable(isShow);
            oper->setModelVisible(id, isShow, true);
        }

        // 获取部件管理器
        Radioss::FITKRadiossPartManager* managerPart = getPartManager();
        if (managerPart == nullptr) return;

        // 遍历所有选中的部件ID
        for (int id : _selectedPartIDs)
        {
            // 获取数据对象
            Radioss::FITKRadiossPart* data = managerPart->getDataByID(id);
            if (data == nullptr) continue;
            // 设置数据对象是否可见
            data->enable(isShow);
            oper->setModelVisible(id, isShow, false);
        }
    }

    void OperTreeAssemblyAction::renameModel()
    {
        // 通过模拟双击事件删除数据
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator("PartTreeEvent"));
        if (oper != nullptr) oper->on_itemDoubleClicked(nullptr, nullptr, 0);

        // 重命名数据
        if (_selectedGeometryIDs.size() == 1) renameData<Interface::FITKGeoCommandList*>(this, getGeometryManager(), _selectedGeometryIDs[0]);
        else if (_selectedPartIDs.size() == 1) renameData<Radioss::FITKRadiossPartManager*>(this, getPartManager(), _selectedPartIDs[0]);
    }

    void OperTreeAssemblyAction::deleteModel()
    {
        // 通过模拟双击事件删除数据
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator("PartTreeEvent"));
        if (oper != nullptr) oper->on_itemDoubleClicked(nullptr, nullptr, 0);

        // 获取几何管理器
        Interface::FITKGeoCommandList* managerGeometry = getGeometryManager();
        if (managerGeometry == nullptr) return;

        // 获取部件管理器
        Radioss::FITKRadiossPartManager* managerPart = getPartManager();
        if (managerPart == nullptr) return;
        QList<int> ids;

        // 删除数据
        for (int id : _selectedGeometryIDs)
        {
            Interface::FITKAbsGeoCommand* data = managerGeometry->getDataByID(id);
            int count = data->getReferenceCmdCount();
            for (int i = 0; i < count; i++)
            {
                Interface::FITKAbsGeoCommand* cmd = data->getReferenceCmdByIndex(i);
                
                ids.append(cmd->getDataObjectID());
            }
            managerGeometry->removeDataByID(id);
        }
        for (int id : ids) show(id, true, false);
        for (int id : _selectedPartIDs) managerPart->removeDataByID(id);
    }

    Interface::FITKGeoCommandList* OperTreeAssemblyAction::getGeometryManager()
    {
        // 获取几何管理器
        return FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
    }

    Radioss::FITKRadiossPartManager* OperTreeAssemblyAction::getPartManager()
    {
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;
        // 获取网格模型
        Radioss::FITKRadiossMeshModel* mesh = dataCase->getMeshModel();
        if (mesh == nullptr) return nullptr;
        // 获取部件管理器
        return mesh->getPartsManager();
    }
}
