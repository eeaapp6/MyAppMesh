/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorProbe.h"
#include "GUIDialog/GUIAnalysisDialog/GUIAccelDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUISectionDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"

#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

bool ModelOper::OperatorProbe::execGUI()
{
    if (_emitter == nullptr)return false;
    QString name = _emitter->objectName();
    if (name.isEmpty())return false;
    // 获取主窗口
    QWidget* mainwindow = FITKAPP->getGlobalData()->getMainWindow();
    if (mainwindow == nullptr) return false;

    if (name == "actionCreateAcceleration")
    {
        GUI::GUIAccelDialog* dlg = new GUI::GUIAccelDialog(nullptr,this,mainwindow);
        connect(dlg, &QDialog::accepted, this, &OperatorProbe::execProfession);
        dlg->show();
    }
    else if (name == "actionEditAcceleration")
    {
        Radioss::FITKProbeAcceleration* data = dynamic_cast<Radioss::FITKProbeAcceleration*>(this->getProbeData());
        if (!data) return false;
        GUI::GUIAccelDialog* dlg = new GUI::GUIAccelDialog(data, this, mainwindow);
        connect(dlg, &QDialog::accepted, this, &OperatorProbe::execProfession);
        dlg->show();

    }
    else if (name == "actionCreateSection")
    {
        GUI::GUISectionDialog* dlg = new GUI::GUISectionDialog(nullptr,this, mainwindow);
        connect(dlg, &QDialog::accepted, this, &OperatorProbe::execProfession);
        connect(dlg, &QDialog::accepted, this, &OperatorProbe::execProfession);
        dlg->show();
    }
    else if (name == "actionEditSection")
    {
        Radioss::FITKProbeSection* data = dynamic_cast<Radioss::FITKProbeSection*>(this->getProbeData());
        if (!data) return false;
         GUI::GUISectionDialog* dlg = new GUI::GUISectionDialog(data,this, mainwindow);
         connect(dlg, &QDialog::accepted, this, &OperatorProbe::execProfession);
         dlg->show();
    }
    return true;
}

bool ModelOper::OperatorProbe::execProfession()
{
    EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(FITKOPERREPO->getOperator("TreeSolutionEvent"));
    if (oper != nullptr) oper->updateTree();

    // 取消高亮
    EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph == nullptr) return false;
    operGraph->clearAllHight();

    int id = -1;
    this->argValue("DataID", id);
    operGraph->updateGraph(id, true);

    return false;
}

Radioss::FITKAbstractProbe * ModelOper::OperatorProbe::getProbeData()
{
    // 获取ID
    int id = -1;
    this->argValue("SelectedItemDataID", id);
    if (id < 0) return nullptr;
    // 获取算例数据
    Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (dataCase == nullptr) return nullptr;
    Radioss::FITKRadiossSolution * solution = dataCase->getCurrentSolution();
    if (!solution) return nullptr;
    Radioss::FITKProbeManager * mgr = solution->getProbeManager();
    if(!mgr)return nullptr;
    //获取数据
    return mgr->getDataByID(id);
}
