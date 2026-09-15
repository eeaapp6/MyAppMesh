/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeWidgetAssembly.h"
#include "GUIWidget/GUIEnumType.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandProp.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossGeomPart.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include <QHeaderView>
#include <QPushButton>
#include <QColorDialog>
#include <QComboBox>

namespace GUI
{
    TreeWidgetAssembly::TreeWidgetAssembly(QWidget* parent) : GUITreeWidgetBase(parent)
    {
        // 设置列数与头部名称
        this->setColumnCount(4);
        this->setHeaderLabels(QStringList() << tr("Name") << tr("Color") << tr("Materials") << tr("Property"));
        this->header()->show();

        //设置每一栏的宽度
        this->header()->resizeSection(0, 260);
        this->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        this->header()->resizeSection(1, 50);
        this->header()->setSectionResizeMode(1, QHeaderView::Fixed);
        this->header()->resizeSection(2, 100);
        this->header()->setSectionResizeMode(2, QHeaderView::Stretch);
        this->header()->resizeSection(3, 100);
        this->header()->setSectionResizeMode(3, QHeaderView::Stretch);
        //表头字符居中
        this->header()->setDefaultAlignment(Qt::AlignHCenter);

        // 树形菜单按住ctr可以多选
        this->setSelectionMode(QAbstractItemView::ExtendedSelection);

        // 更新树形菜单
        updateTreeWidget();
    }

    Core::FITKTreeWidgetOperator* TreeWidgetAssembly::getOperator()
    {
        // 装配树事件操作器
        return this->getOperatorT<Core::FITKTreeWidgetOperator>("TreeAssemblyEvent");
    }

    void TreeWidgetAssembly::updateTreeWidget()
    {
        // 锁定信号
        this->blockSignals(true);
        // 清空树
        this->clear();

        // 更新树
        updateTreeGeometry(createRootItem(this, tr("Geometry"), GUI::TreeWidgetAssemblyType::TWAssemblyGeometry));
        updateTreePart(createRootItem(this, tr("Part"), GUI::TreeWidgetAssemblyType::TWAssemblyPart));

        // 展开全部节点
        this->expandAll();
        // 解锁信号
        this->blockSignals(false);
    }

    void TreeWidgetAssembly::mouseReleaseEvent(QMouseEvent* event)
    {
        // 调用基类方法
        GUITreeWidgetBase::mouseReleaseEvent(event);
        
        // 获取可视对象数据操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;
        // 清除高亮
        oper->clearAllHight();

        // 获取选中的树节点
        QList<QTreeWidgetItem*> listItem = selectedItems();
        
        // 初始化需要高亮的数据模型ID列表
        QList<int> listModelID;
        
        // 遍历选中的树节点
        for (QTreeWidgetItem* item : listItem)
        {
            // 当前节点无效时，跳过
            if (item == nullptr) continue;
            // 获取节点类型
            int type = item->type();
            // 当前节点不是模型数据时，跳过
            if (type != GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild && type != GUI::TreeWidgetAssemblyType::TWAssemblyPartChild) continue;
            // 获取模型数据ID
            int idModel = item->data(1, 0).toInt();
            // 仅记录用户可见且有效的模型数据ID
            if (item->checkState(0) == Qt::CheckState::Checked && idModel > 0) listModelID.append(idModel);
        }

        // 高亮选中的数据模型
        for (int id : listModelID) oper->hightObject(id);
    }

    void TreeWidgetAssembly::on_itemStateChanged(QTreeWidgetItem* item, bool state, int column)
    {
        // 获取装配树事件操作器
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(this->getOperator());
        if (oper == nullptr) return;

        // 手动出发节点状态改变事件
        oper->on_itemStateChanged(this, item, state, column);
    }

    void TreeWidgetAssembly::on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu)
    {
        //右键菜单动作生成
        if (items.size() < 1 || menu == nullptr) return;
        
        // 处理单个节点
        if (items.size() == 1)
        {
            // 获取当前节点
            QTreeWidgetItem* item = items.at(0);
            if (item == nullptr) return;

            // 获取节点类型
            int type = item->type();
            // Geometry根节点
            if (type == GUI::TreeWidgetAssemblyType::TWAssemblyGeometry)
            {
                createMenuItem(menu, tr("Create Box"), "actionAssemblyTreeCreateBox");
                createMenuItem(menu, tr("Create Cylinder"), "actionAssemblyTreeCreateCylinder");
                createMenuItem(menu, tr("Create Sphere"), "actionAssemblyTreeCreateSphere");
                return;
            }
            // Geometry子节点
            else if (type == GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild) createMenuItem(menu, tr("Rename Geometry"), "actionAssemblyTreeRename");
            // Part根节点
            else if (type == GUI::TreeWidgetAssemblyType::TWAssemblyPart)
            {
                createMenuItem(menu, tr("Create Part"), "actionPartTreeCreatePart");
                return;
            }
            // Part子节点
            else if (type == GUI::TreeWidgetAssemblyType::TWAssemblyPartChild) createMenuItem(menu, tr("Rename Part"), "actionAssemblyTreeRename");
            else return;
        }

        // 初始化状态
        bool isContainData = false;
        // 遍历全部节点
        for (QTreeWidgetItem* item : items)
        {
            // 判断节点是否有效
            if (item == nullptr) continue;
            // 获取节点类型
            int type = item->type();
            // 当包含数据信息时，更新状态，之后跳出循环
            if (type == GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild || type == GUI::TreeWidgetAssemblyType::TWAssemblyPartChild)
            {
                isContainData = true;
                break;
            }
        }

        // 获取当前节点
        QTreeWidgetItem* item = currentItem();
        
        // 仅当存在数据时，展示批量处理菜单（批处理模式）
        if (isContainData)
        {
            // 获取节点类型
            int type = item->type();

            // 设置颜色
            createMenuItem(menu, tr("Set Color"), "actionAssemblyTreeSetColor");
            
            if (type == GUI::TreeWidgetAssemblyType::TWAssemblyPartChild)
            {
                // 设置材质
                createMenuItem(menu, tr("Set Material"), "actionAssemblyTreeSetMaterial");
                // 设置属性
                createMenuItem(menu, tr("Set Property"), "actionAssemblyTreeSetProperty");
                // SPH粒子转化
                createMenuItem(menu, tr("SPH Transformation"), "actionAssemblyTreeSPHTrans");
            }
            // 显示模型
            createMenuItem(menu, tr("Show Model"), "actionAssemblyTreeShowModel");
            // 隐藏模型
            createMenuItem(menu, tr("Hide Model"), "actionAssemblyTreeHideModel");
            // 删除模型
            createMenuItem(menu, tr("Delete Model"), "actionAssemblyTreeDelete");
        }
    }

    void TreeWidgetAssembly::on_buttonColor_clicked()
    {
        // 获取可视对象数据操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;
        // 清除高亮
        oper->clearAllHight();

        // 获取按钮
        QPushButton* currentButton = dynamic_cast<QPushButton*>(sender());
        if (currentButton == nullptr) return;

        // 获取颜色
        QColor currentColor = currentButton->property("color").value<QColor>();
        if (!currentColor.isValid()) return;
        // 选取颜色
        QColor color = QColorDialog::getColor(currentColor, nullptr, QObject::tr("Select Color"));
        if (!color.isValid()) return;

        // 遍历选中的节点
        for (QTreeWidgetItem* item : this->selectedItems())
        {
            // 判断节点是否有效
            if (item == nullptr) continue;

            // 获取节点类型
            int type = item->type();
            if (type != GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild && type != GUI::TreeWidgetAssemblyType::TWAssemblyPartChild) continue;

            // 获取模型数据ID
            int id = getObjectID(item);
            if (id < 1) continue;
            // 高亮节点
            oper->hightObject(id);

            // 根据节点类型更新模型颜色
            if (type == GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild)
            {
                // 获取几何管理器
                Interface::FITKGeoCommandList* manager = getCommandManagerGeo();
                if (manager == nullptr) continue;
                // 获取几何数据
                Interface::FITKAbsGeoCommand* dataGeometry = manager->getDataByID(id);
                if (dataGeometry == nullptr) continue;
                // 获取属性
                Interface::FITKGeoCommandProp* dataProperty = dataGeometry->getCommandProp();
                if (dataProperty == nullptr) continue;
                // 设置颜色
                dataProperty->setColor(color);
            }
            else if (type == GUI::TreeWidgetAssemblyType::TWAssemblyPartChild)
            {
                // 获取部件管理器
                Radioss::FITKRadiossPartManager* manager = getManagerPart();
                if (manager == nullptr) continue;
                // 获取部件数据
                Radioss::FITKRadiossPart* dataPart = manager->getDataByID(id);
                if (dataPart == nullptr) continue;
                // 设置颜色
                dataPart->setColor(color);
            }

            // 更新界面
            EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
            if (oper != nullptr)
            {
                oper->updateAllColor(id, color);
            }

            // 获取选色按钮
            QPushButton* button = dynamic_cast<QPushButton*>(this->itemWidget(item, 1));
            if (button == nullptr) continue;

            // 刷新样式
            button->setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));
            // 保存颜色
            button->setProperty("color", QVariant::fromValue(color));
        }
    }

    void TreeWidgetAssembly::on_comboBoxMaterial_activated(int index)
    {
        // 获取可视对象数据操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;
        // 清除高亮
        oper->clearAllHight();

        // 获取下拉框
        QComboBox* currentComboBox = dynamic_cast<QComboBox*>(sender());
        if (currentComboBox == nullptr) return;

        // 获取材料ID
        int materialID = currentComboBox->itemData(index).toInt();

        // 遍历选中的节点
        for (QTreeWidgetItem* item : this->selectedItems())
        {
            // 判断节点是否有效
            if (item == nullptr) continue;

            // 获取节点类型
            int type = item->type();
            if (type != GUI::TreeWidgetAssemblyType::TWAssemblyPartChild) continue;

            // 获取模型数据ID
            int id = getObjectID(item);
            if (id < 1) continue;
            // 高亮节点
            oper->hightObject(id);

            // 获取部件管理器
            Radioss::FITKRadiossPartManager* manager = getManagerPart();
            if (manager == nullptr) continue;
            // 获取部件数据
            Radioss::FITKRadiossPart* dataPart = manager->getDataByID(id);
            if (dataPart == nullptr) continue;

            // 设置材料ID
            dataPart->setMaterialID(materialID);

            // 获取材料下拉框
            QComboBox* comboBox = dynamic_cast<QComboBox*>(this->itemWidget(item, 2));
            if (comboBox == nullptr) continue;

            // 设置当前选项
            comboBox->setCurrentIndex(index);
        }
    }

    void TreeWidgetAssembly::on_comboBoxProperty_activated(int index)
    {
        // 获取可视对象数据操作器
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper == nullptr) return;
        // 清除高亮
        oper->clearAllHight();

        // 获取下拉框
        QComboBox* currentComboBox = dynamic_cast<QComboBox*>(sender());
        if (currentComboBox == nullptr) return;

        // 获取属性ID
        int propertyID = currentComboBox->itemData(index).toInt();

        // 遍历选中的节点
        for (QTreeWidgetItem* item : this->selectedItems())
        {
            // 判断节点是否有效
            if (item == nullptr) continue;

            // 获取节点类型
            int type = item->type();
            if (type != GUI::TreeWidgetAssemblyType::TWAssemblyPartChild) continue;

            // 获取模型数据ID
            int id = getObjectID(item);
            if (id < 1) continue;
            // 高亮节点
            oper->hightObject(id);

            // 获取部件管理器
            Radioss::FITKRadiossPartManager* manager = getManagerPart();
            if (manager == nullptr) continue;
            // 获取部件数据
            Radioss::FITKRadiossPart* dataPart = manager->getDataByID(id);
            if (dataPart == nullptr) continue;

            // 设置属性ID
            dataPart->setPropertyID(propertyID);

            // 获取属性下拉框
            QComboBox* comboBox = dynamic_cast<QComboBox*>(this->itemWidget(item, 3));
            if (comboBox == nullptr) continue;

            // 设置当前选项
            comboBox->setCurrentIndex(index);
        }
    }

    void TreeWidgetAssembly::updateTreeGeometry(QTreeWidgetItem* itemParent)
    {
        // 检查参数
        if (itemParent == nullptr) return;
        //获取Radioss几何管理器
        Radioss::FITKRadiossGeomPartManager* geoPartMgr = getManagerGeometry();
        if (!geoPartMgr) return;
        int count = geoPartMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            // 获取几何数据
            Radioss::FITKRadiossGeomPart* part = geoPartMgr->getDataByIndex(i);
            if (!part || part->isGeometryReferencedCmd()) continue;
            Interface::FITKAbsGeoPart* geoPart = part->getGeoPart();
            if (!geoPart) continue;
            // 创建节点
            QTreeWidgetItem* item = createChildItem(itemParent, part->getDataObjectName(), GUI::TreeWidgetAssemblyType::TWAssemblyGeometryChild);
            setObjectID(item, part->getGeoPartID());
            setItemChecked(item, 0, part->isEnable());

            // 获取属性
            Interface::FITKGeoCommandProp* dataProperty = geoPart->getCommandProp();
            if (dataProperty == nullptr) continue;
            // 填充颜色
            fillColor(item, geoPart);
        }
    }

    void TreeWidgetAssembly::updateTreePart(QTreeWidgetItem* itemParent)
    {
        // 检查参数
        if (itemParent == nullptr) return;

        //获取部件管理器
        Radioss::FITKRadiossPartManager* manager = getManagerPart();
        if (manager == nullptr) return;

        // 遍历部件数据
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            //创建part节点
            Radioss::FITKRadiossPart* data = manager->getDataByIndex(i);
            if (!data) continue;

            // 创建节点
            QTreeWidgetItem* item = new QTreeWidgetItem(itemParent, QStringList() << data->getDataObjectName(), GUI::TreeWidgetAssemblyType::TWAssemblyPartChild);
            setObjectID(item, data->getDataObjectID());
            setItemChecked(item, 0, data->isEnable());

            // 填充颜色
            fillColor(item, data);
            // 填充材料
            fillMaterial(item, data);
            // 填充属性
            fillProperty(item, data);
        }
    }

    void TreeWidgetAssembly::fillColor(QTreeWidgetItem* item, Radioss::FITKRadiossPart* data)
    {
        // 第2列：颜色
        // 检查参数
        if (item == nullptr || data == nullptr) return;

        // 获取颜色
        QColor color = data->getColor();

        // 创建按钮
        QPushButton* buttonColor = new QPushButton();
        // 设置样式
        buttonColor->setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));
        // 设置属性
        buttonColor->setProperty("color", color);
        // 绑定点击事件
        connect(buttonColor, &QPushButton::clicked, this, &TreeWidgetAssembly::on_buttonColor_clicked);

        // 加载控件
        this->setItemWidget(item, 1, buttonColor);
    }

    void TreeWidgetAssembly::fillColor(QTreeWidgetItem* item, Interface::FITKAbsGeoCommand* data)
    {
        // 第2列：颜色
        // 检查参数
        if (item == nullptr || data == nullptr) return;

        // 获取属性
        Interface::FITKGeoCommandProp* dataProperty = data->getCommandProp();
        if (dataProperty == nullptr) return;

        // 获取颜色
        QColor color = dataProperty->getColor();

        // 创建按钮
        QPushButton* buttonColor = new QPushButton();
        // 设置样式
        buttonColor->setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));
        // 设置属性
        buttonColor->setProperty("color", color);
        // 绑定点击事件
        connect(buttonColor, &QPushButton::clicked, this, &TreeWidgetAssembly::on_buttonColor_clicked);

        // 加载控件
        this->setItemWidget(item, 1, buttonColor);
    }

    void TreeWidgetAssembly::fillMaterial(QTreeWidgetItem* item, Radioss::FITKRadiossPart* data)
    {
        // 第3列：材料
        // 检查参数
        if (item == nullptr || data == nullptr) return;

        // 创建下拉框
        QComboBox* comboBoxMaterial = new QComboBox;
        // 初始化下拉选项
        comboBoxMaterial->addItem(tr("None"), 0);

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            // 加载控件
            this->setItemWidget(item, 2, comboBoxMaterial);
            return;
        }
        // 获取材料管理器
        Interface::FITKMaterialManager* manager = dataCase->getMaterialManager();
        if (manager == nullptr)
        {
            // 加载控件
            this->setItemWidget(item, 2, comboBoxMaterial);
            return;
        }

        // 初始化当前选项索引值和当前材料ID
        int currentIndex = 0, currentMaterialID = data->getMaterialID();
        // 遍历材料数据
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取材料数据
            Interface::FITKAbstractMaterial* dataMaterial = manager->getDataByIndex(i);
            if (dataMaterial == nullptr) continue;
            // 获取材料ID
            int materialID = dataMaterial->getDataObjectID();
            // 添加下拉选项
            comboBoxMaterial->addItem(dataMaterial->getDataObjectName(), materialID);
            // 获取当前选项索引值
            if (materialID == currentMaterialID && materialID > 0) currentIndex = i + 1;
        }
        // 设置当前选项
        comboBoxMaterial->setCurrentIndex(currentIndex);

        // 绑定事件
        connect(comboBoxMaterial, QOverload<int>::of(&QComboBox::activated), this, &TreeWidgetAssembly::on_comboBoxMaterial_activated);

        // 加载控件
        this->setItemWidget(item, 2, comboBoxMaterial);
    }

    void TreeWidgetAssembly::fillProperty(QTreeWidgetItem* item, Radioss::FITKRadiossPart* data)
    {
        // 第4列：属性
        // 检查参数
        if (item == nullptr || data == nullptr) return;

        // 创建下拉框
        QComboBox* comboBoxProperty = new QComboBox;
        // 初始化下拉选项
        comboBoxProperty->addItem(tr("None"), 0);

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            // 加载控件
            this->setItemWidget(item, 2, comboBoxProperty);
            return;
        }

        // 获取属性管理器
        Interface::FITKAbstractSectionManager* manager = dataCase->getSectionManager();
        if (manager == nullptr)
        {
            // 加载控件
            this->setItemWidget(item, 2, comboBoxProperty);
            return;
        }

        // 初始化当前选项索引值和当前属性ID
        int currentIndex = 0, currentPropertyID = data->getPropertyID();
        // 遍历属性数据
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取属性数据
            Interface::FITKAbstractSection* dataProperty = manager->getDataByIndex(i);
            if (dataProperty == nullptr) continue;
            // 获取属性ID
            int propertyID = dataProperty->getDataObjectID();
            // 添加下拉选项
            comboBoxProperty->addItem(dataProperty->getDataObjectName(), propertyID);
            // 获取当前选项索引值
            if (propertyID == currentPropertyID && propertyID > 0) currentIndex = i + 1;
        }
        // 设置当前选项
        comboBoxProperty->setCurrentIndex(currentIndex);

        // 绑定事件
        connect(comboBoxProperty, QOverload<int>::of(&QComboBox::activated), this, &TreeWidgetAssembly::on_comboBoxProperty_activated);

        // 加载控件
        this->setItemWidget(item, 3, comboBoxProperty);
    }

    Radioss::FITKRadiossGeomPartManager*  TreeWidgetAssembly::getManagerGeometry()
    {
        // 几何管理器
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return nullptr;
        return dataCase->getGeomPartManager();
    }

    Radioss::FITKRadiossPartManager* TreeWidgetAssembly::getManagerPart()
    {
        // 获取模型数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return nullptr;
        // 获取网格模型
        Radioss::FITKRadiossMeshModel* mesh = dataCase->getMeshModel();
        if (mesh == nullptr) return nullptr;

        // 部件管理器
        return mesh->getPartsManager();
    }

    Interface::FITKGeoCommandList* TreeWidgetAssembly::getCommandManagerGeo() {
        return FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
    }
}