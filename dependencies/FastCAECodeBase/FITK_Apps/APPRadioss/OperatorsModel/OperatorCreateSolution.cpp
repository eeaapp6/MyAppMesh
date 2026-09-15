/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorCreateSolution.h"
#include "GUIDialog/GUIAnalysisDialog/GUICreateSolutionDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "GUIWidget/TreeWidgetSolution.h"

bool ModelOper::OperatorCreateSolution::execGUI()
{
    GUI::GUICreateSolutionDialog dlg;
    if (dlg.exec() != QDialog::Accepted)return false;
    QString name = dlg.getSolutionName();
    Radioss::FITKRadiossCase* caseObj = FITKAPP->getGlobalData()->
        getPhysicsData<Radioss::FITKRadiossCase>();
    if (!caseObj) return false;
    auto matMgr = caseObj->getSolutionManager();
    if(!matMgr) return false;

    Radioss::FITKRadiossSolution* solution = new Radioss::FITKRadiossSolution;
    solution->setDataObjectName(name);
    matMgr->appendDataObj(solution);

    return true;
}

bool ModelOper::OperatorCreateSolution::execProfession()
{
    //更新树的操作
    GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
    if (!mainWindow) return false;
    GUI::ControlPanel* controlPanel = mainWindow->getControlPanel();
    if (!controlPanel) return false;
    GUI::TreeWidgetSolution* TreeWidgetSolution = controlPanel->getSolutionTree();
    if (!TreeWidgetSolution) return false;
    TreeWidgetSolution->updateTreeWidget();

    return false;
}
