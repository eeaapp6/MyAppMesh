/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsNewProject.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

namespace ModelOper
{
    bool OperatorsNewProject::execGUI()
    {
        // 是否保存工程
        QMessageBox msgBox;
        msgBox.setWindowTitle(QObject::tr("New Project"));
        msgBox.setText(QObject::tr("This session contains changes that will be lost if you continue. How would you like to proceed?"));
        msgBox.setIcon(QMessageBox::Warning);
        QPushButton* saveBtn = msgBox.addButton(QObject::tr("Save"), QMessageBox::AcceptRole);
        QPushButton* saveAsBtn = msgBox.addButton(QObject::tr("Save As"), QMessageBox::AcceptRole);
        QPushButton* discardBtn = msgBox.addButton(QObject::tr("Discard"), QMessageBox::AcceptRole);
        QPushButton* cancelBtn = msgBox.addButton(QObject::tr("Cancel"), QMessageBox::RejectRole);
        msgBox.exec();
        QAbstractButton* baseClickedBtn = msgBox.clickedButton();
        QPushButton* clickedBtn = dynamic_cast<QPushButton*>(baseClickedBtn);
        if (clickedBtn == nullptr) return false;
        if (clickedBtn == saveBtn)
        {
            Core::FITKActionOperator* graphOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSave");
            if(!graphOper) return false;
            graphOper->actionTriggered();
            return true;
        }
        else if (clickedBtn == saveAsBtn)
        {
            Core::FITKActionOperator* graphOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSaveAs");
            if (!graphOper) return false;
            graphOper->actionTriggered();
            return true;
        }
        else if (clickedBtn == discardBtn)
        {
            return true;
        }
        else if (clickedBtn == cancelBtn)
        {
            return false;
        }
        return true;
    }

    bool OperatorsNewProject::execProfession()
    {
        clearProject();
        return  true;
    }
  
    void OperatorsNewProject::clearProject()
    {
        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return;
        cmdList->clear();

        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (!caseData) return;
        caseData->clearData();

        Radioss::FITKRadiossSolution* solution = new Radioss::FITKRadiossSolution();
        solution->setDataObjectName("Solution-1");
        Radioss::FITKRadiossSolutionManager* solutionManager = caseData->getSolutionManager();
        if (!solutionManager) return;
        solutionManager->appendDataObj(solution);

        //刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" ,"ResultTreeEvent"};
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }

        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphOper)
        {
            graphOper->updateAll();
        }

    }
}

