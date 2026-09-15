/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsSet.h"
#include "GUIFrame/MainWindow.h"
#include "GUIDialog/GUIMeshDialog/GUIPartPickDialog.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "PickDataProvider/GUIPickInfo.h"

namespace OperModel
{
    bool OperatorsSet::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QString name = _emitter->objectName();
        if (name.isEmpty()) return false;
        GUI::GUIPartPickDialog* dlg = nullptr;
        GUI::MainWindow* mw = GUI::MainWindow::GetMainWindowFromFramework();
        if (mw == nullptr) return false;
        EventOper::GraphEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (oper)
        {
            oper->clearAllHight();
        }
        if (name == "actionSetNodesCreate")
        {
            dlg = new GUI::GUIPartPickDialog(this, mw, GUI::GUIPickInfo::PickObjType::POBMeshVert);
            dlg->setWindowTitle("Pick Nodes");
        }
        if (name == "actionSetElementCreate")
        {
            dlg = new GUI::GUIPartPickDialog(this, mw, GUI::GUIPickInfo::PickObjType::POBMeshCell);
            dlg->setWindowTitle("Pick Elements");
        }
        if (dlg != nullptr)
        {
            dlg->show();
        }
        return false;
    }

    bool OperatorsSet::execProfession()
    {
         //通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
        emit operatorAbstractSig1(this);
        //EventOper::TreeEventOperator* oper = FITKOPERREPO->getOperatorT<EventOper::TreeEventOperator>("TreeGroupEvent");
        //if (oper == nullptr) return false;
        //oper->updateTree();
        return false;
    }

}
