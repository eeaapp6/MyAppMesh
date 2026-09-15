/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ResultTreeWidget.h"
#include "GUIEnumType.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStaticStruPostVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKDynamicStruPostVTK.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include <QHeaderView>
#include <QMenu>
#include <QPushButton>
#include <QColorDialog>
#include <QComboBox>
#include <QTreeWidgetItem>

namespace GUI
{
    ResultTreeWidget::ResultTreeWidget(QWidget* parent)
    {
        updateTreeWidget();
    }
    
    Core::FITKTreeWidgetOperator * ResultTreeWidget::getOperator()
    {
        return this->getOperatorT<Core::FITKTreeWidgetOperator>("ResultTreeEvent");
    }
    void ResultTreeWidget::updateTreeWidget()
    {
        auto getNameByDataRepo = [&](int id)
        {
            Core::FITKAbstractNDataObject* data = FITKDATAREPO->getTDataByID<Core::FITKAbstractNDataObject>(id);
            if (!data) return QString();
            return data->getDataObjectName();
        };
        // 清空属性树
        this->clear();
        // 隐藏列标题
        this->header()->hide();
        this->blockSignals(true);

        Radioss::FITKRadiossPostData* postData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (!postData) return;
        Interface::FITKStruPost3DManager* post3DDataMgr = postData->getPost3DManager();
        Radioss::FITKRadiossPost2DManager* post2DDataMgr = postData->getPost2DManager();
        if (!post3DDataMgr || !post2DDataMgr) return;

        QTreeWidgetItem* post3DRootItem = this->CreateTreeRootItem(this, QObject::tr("Post3D"), PostTreeItemType::PITPost3DRoot);
        QTreeWidgetItem* post2DRootItem = this->CreateTreeRootItem(this, QObject::tr("Post2D"), PostTreeItemType::PITPost2DRoot);
        if (!post2DRootItem || !post3DRootItem) return;
        //3维数据
        int count = post3DDataMgr->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKAbstractStructuralPostVTK* post3DData = post3DDataMgr->getDataByIndex(i);
            if (!post3DData) continue;
            QString postNodeName = getNameByDataRepo(post3DData->getPreCaseID());
            if (postNodeName.isEmpty())
                postNodeName = post3DData->getDataObjectName();
            QTreeWidgetItem* postItem = this->CreateTreeItem(post3DRootItem, postNodeName, PostTreeItemType::PITPost3D);
            if (!postItem) continue;
            postItem->setData(0, TreeAdditionalInfo::TAIPostID, post3DData->getDataObjectID());

            QTreeWidgetItem* stepRootItem = this->CreateTreeItem(postItem, QObject::tr("Steps"), PostTreeItemType::PITStepRoot);
            if (!stepRootItem) continue;
            Interface::StruPostType type = post3DData->getPostStructualType();
            if (type == Interface::StruPostType::SPTStatic)
            {
                Interface::FITKStaticStruPostVTK* staticPost3DData = dynamic_cast<Interface::FITKStaticStruPostVTK*>(post3DData);
                if (!staticPost3DData) continue;

            }
            else if (type == Interface::StruPostType::SPTDynamicExplict)
            {
                Interface::FITKDynamicStruPostVTK* dynamicPost3DData = dynamic_cast<Interface::FITKDynamicStruPostVTK*>(post3DData);
                if (!dynamicPost3DData) continue;
                int num = dynamicPost3DData->frameNum();
                for (int i = 0; i < num; ++i)
                {
                    QTreeWidgetItem* stepItem = this->CreateTreeItem(stepRootItem, QString("%1: %2").arg(QObject::tr("Frame")).arg(i), PostTreeItemType::PITStep);
                    stepItem->setData(0, TreeAdditionalInfo::TAIStepIndex, i);
                    stepItem->setData(0, TreeAdditionalInfo::TAIPostID, post3DData->getDataObjectID());
                }
            }
        }
        //2维数据
        count = post2DDataMgr->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Radioss::FITKRadiossPost2DData* post2DData = post2DDataMgr->getDataByIndex(i);
            if (!post2DData) continue;
            QString postNodeName = getNameByDataRepo(post2DData->getSolutionID());
            if (postNodeName.isEmpty())
                postNodeName = post2DData->getDataObjectName();
            QTreeWidgetItem* postItem = this->CreateTreeItem(post2DRootItem, postNodeName, PostTreeItemType::PITPost2D);
            if (!postItem) continue;
            postItem->setData(0, TreeAdditionalInfo::TAIPostID, post2DData->getDataObjectID());

            QTreeWidgetItem* xyPlotItem = this->CreateTreeItem(postItem, QObject::tr("XYPlot"), PostTreeItemType::PITXYPlot);
            xyPlotItem->setData(0, TreeAdditionalInfo::TAIPostID, post2DData->getDataObjectID());
        }
        // 展开全部节点
        this->expandAll();
        this->blockSignals(false);
    }

    void ResultTreeWidget::on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu)
    {
        if (items.size() != 1 || menu == nullptr) return;
        // 获取当前项
        QTreeWidgetItem* item = items.at(0);
        if (item == nullptr) return;
        // 根据节点类型初始化右键菜单
        int type = item->type();
        // 根据节点类型生成菜单
        if (type == GUI::PostTreeItemType::PITPost3D)
        {
            QAction* action = menu->addAction(tr("Delete"));
            action->setObjectName("actionDelete3DPost");
        }
        else if (type == GUI::PostTreeItemType::PITPost2D)
        {
            QAction* action = menu->addAction(tr("Delete"));
            action->setObjectName("actionDelete2DPost");
        }
        else if (type == GUI::PostTreeItemType::PITXYPlot)
        {
            QAction* action = menu->addAction(tr("Show"));
            action->setObjectName("actionShowXYPlot");
        }
        else return;
    }

}
