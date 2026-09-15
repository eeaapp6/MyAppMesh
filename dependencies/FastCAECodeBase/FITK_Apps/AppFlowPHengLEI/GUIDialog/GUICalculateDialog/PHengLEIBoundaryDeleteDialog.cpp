/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PHengLEIBoundaryDeleteDialog.h"
#include "ui_PHengLEIBoundaryDeleteDialog.h"
#include "PHengLEIBoundaryWidget.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIBoundaryTypeManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"

namespace GUI 
{
    PHengLEIBoundaryDeleteDialog::PHengLEIBoundaryDeleteDialog(Interface::FITKPHengLEIBoundary* obj, EventOper::ParaWidgetInterfaceOperator * oper) :
        GUIDialogBase(FITKAPP->getGlobalData()->getMainWindow()), _obj(obj), _oper(oper)
    {
        _ui = new Ui::PHengLEIBoundaryDeleteDialog();
        _ui->setupUi(this);

        if (_obj) {
            QString text = tr("Delete %1").arg(_obj->getDataObjectName());
            _ui->label_Name->setText(text);
        }

        setWindowTitle(tr("Boundary Condition Delete"));
    }

    PHengLEIBoundaryDeleteDialog::~PHengLEIBoundaryDeleteDialog()
    {
        if (_ui)delete _ui;
    }

    void PHengLEIBoundaryDeleteDialog::on_pushButton_OK_clicked()
    {
        if (_obj == nullptr)return;
        Interface::FITKPHengLEIData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();
        if (physicsData == nullptr)return;
        Interface::FITKPHengLEIBoundaryManager* manager = physicsData->getBoundaryManager();
        if (manager == nullptr)return;
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr)return;
        auto propertyWidget = mainWin->getPropertyWidget();
        if (propertyWidget == nullptr)return;

        //清除当前数据对应的界面
        PHengLEIBoundaryWidget* boundWidget = dynamic_cast<PHengLEIBoundaryWidget*>(propertyWidget->getCurrentWidget());
        if (boundWidget) {
            if (_obj == boundWidget->getCurrentObj()) {
                propertyWidget->init();
            }
        }

        //清除数据
        manager->removeDataByID(_obj->getDataObjectID());

        //执行操作器进行刷新
        if (_oper) {
            _oper->execProfession();
        }
        this->accept();
    }

    void PHengLEIBoundaryDeleteDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }
}

