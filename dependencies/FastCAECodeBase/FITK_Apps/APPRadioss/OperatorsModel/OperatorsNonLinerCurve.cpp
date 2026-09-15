/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsNonLinerCurve.h"
#include "GUIDialog/GUIAnalysisDialog/GUICurveDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIFrame/MainWindow.h"

namespace OperModel
{
    bool OperatorsNonLinerCurve::execGUI()
    {
        // 获取主窗口
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();

        if (!_emitter)return false;
        QString name = _emitter->objectName();
        if (name == "actionCurveCreate") {
            GUI::GUICurveDialog* dlg = new GUI::GUICurveDialog(nullptr,mainWindow);
            connect(dlg, &QDialog::accepted, this, &OperatorsNonLinerCurve::execProfession);
            dlg->show();
        }
        else if (name == "actionCurveEdit") {
            int ID = -1;
            this->argValue("SelectedItemDataID", ID);
            // 获取算例数据
            Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
            if (dataCase == nullptr) return false;
            Radioss::FITKNonLinerCurveManager* manager = dataCase->getNonLinerCurveManager();
            if (!manager) return false;
            Radioss::FITKNonLinerCurve* data = manager->getDataByID(ID);
            if(!data)  return false;
            GUI::GUICurveDialog* dlg = new GUI::GUICurveDialog(data, mainWindow);
            connect(dlg, &QDialog::accepted, this, &OperatorsNonLinerCurve::execProfession);
            dlg->show();
        }
        return false;
    }

    bool OperatorsNonLinerCurve::execProfession()
    {
        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
        return false;
    }
}