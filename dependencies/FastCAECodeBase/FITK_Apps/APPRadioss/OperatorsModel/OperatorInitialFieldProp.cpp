/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorInitialFieldProp.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIDialog/GUIAnalysisDialog/GUIInitialFieldTypeDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIInitialFieldTRADialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIInitialFieldAXISDialog.h"

namespace OperModel
{
    bool OperatorInitialFieldProp::execGUI()
    {
        // 检查触发器是否存在
        if (_emitter == nullptr) return false;

        // 获取主窗口
        QWidget* mainwindow = FITKAPP->getGlobalData()->getMainWindow();
        if (mainwindow == nullptr) return false;

        // 获取操作名称
        QString name = _emitter->objectName();
        // 创建初始化场
        if (name == "actionInitialFieldCreate")
        {
            // 选择初始场类型
            GUI::GUIInitialFieldTypeDialog dialogType(mainwindow);
            if (dialogType.exec() != QDialog::Accepted) return false;

            // 设置参数
            this->setArgs("Model", 0);
            this->setArgs("DataName", dialogType.getDataName());
            this->setArgs("DataKeyWord", dialogType.getDataKeyWord());
            
            // 根据类型调用对话框
            QDialog* dialog = nullptr;
            Radioss::FITKAbstractInitialField::InitialFieldType type = Radioss::FITKAbstractInitialField::InitialFieldType(dialogType.getDataType());
            switch (type)
            {
            case Radioss::FITKAbstractInitialField::TRA: dialog = new GUI::GUIInitialFieldTRADialog(this, mainwindow);
                break;
            case Radioss::FITKAbstractInitialField::ROT:
                break;
            case Radioss::FITKAbstractInitialField::TG:
                break;
            case Radioss::FITKAbstractInitialField::GRID:
                break;
            case Radioss::FITKAbstractInitialField::AXIS: dialog = new GUI::GUIInitialFieldAXISDialog(this, mainwindow);
                break;
            default:
                break;
            }
            if (dialog != nullptr) dialog->show();
        }
        // 编辑初始化场
        else if (name == "actionInitialFieldEdit")
        {
            // 获取数据ID
            int id = -1;
            this->argValue("SelectedItemDataID", id);
            if (id < 1) return false;

            // 获取初始化场数据
            Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (dataCase == nullptr) return false;
            Radioss::FITKInitialFieldManager* manager = dataCase->getInitialFieldManager();
            if (manager == nullptr) return false;
            Radioss::FITKAbstractInitialField* data = manager->getDataByID(id);
            if (data == nullptr) return false;

            // 设置参数
            this->setArgs("Model", 1);
            this->setArgs("InitialFieldData", data->FITKObj2QVarient());

            // 根据类型调用对话框
            QDialog* dialog = nullptr;
            Radioss::FITKAbstractInitialField::InitialFieldType type = data->getType();
            switch (type)
            {
            case Radioss::FITKAbstractInitialField::TRA: dialog = new GUI::GUIInitialFieldTRADialog(this, mainwindow);
                break;
            case Radioss::FITKAbstractInitialField::ROT:
                break;
            case Radioss::FITKAbstractInitialField::TG:
                break;
            case Radioss::FITKAbstractInitialField::GRID:
                break;
            case Radioss::FITKAbstractInitialField::AXIS: dialog = new GUI::GUIInitialFieldAXISDialog(dynamic_cast<Radioss::FITKInitialFieldAxis*>(data), this, mainwindow);
                break;
            default:
                break;
            }
            if (dialog != nullptr) dialog->show();
        }
        return false;
    }

    bool OperatorInitialFieldProp::execProfession()
    {
        // 通过树形菜单事件处理器刷新树
        QStringList listOperator = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOperator.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(FITKOPERREPO->getOperator(listOperator[i]));
            if (oper != nullptr) oper->updateTree();
        }

        // 取消高亮
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph == nullptr) return false;
        operGraph->clearAllHight();

        int id = -1;
        this->argValue("DataID", id);
        operGraph->updateGraph(id, true);

        return true;
    }
}