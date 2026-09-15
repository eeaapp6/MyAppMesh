/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGeometryModel.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"

#include "GUIWidget/TreeWidgetProperty.h"
#include "GUIWidget/TreeWidgetSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

#include "GUIDialog/GUIGeometryDialog/GUIGeometryBoxDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryCylinderDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometrySphereDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryBoolDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryChamferDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryFilletDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryDefeatureDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryMirrorDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryRecPatternDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryCirPatternDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryScaleDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryRotationDialog.h"
#include "GUIDialog/GUIGeometryDialog/GUIGeometryTransDialog.h"

namespace OperModel
{
    bool OperGeometryModel::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        QString name = _emitter->objectName();
        if (name.isEmpty())return false;
        if (name == "actionCreateBox")
        {
            GUI::GUIGeometryBoxDialog* dialog = new GUI::GUIGeometryBoxDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateCylinder")
        {
            GUI::GUIGeometryCylinderDialog* dialog = new GUI::GUIGeometryCylinderDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateSphere")
        {
            GUI::GUIGeometrySphereDialog* dialog = new GUI::GUIGeometrySphereDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateBool")
        {
            GUI::GUIGeometryBoolDialog* dialog = new GUI::GUIGeometryBoolDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateChamfer")
        {
            GUI::GUIGeometryChamferDialog* dialog = new GUI::GUIGeometryChamferDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateFillet")
        {
            GUI::GUIGeometryFilletDialog* dialog = new GUI::GUIGeometryFilletDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateDefeature")
        {
            GUI::GUIGeometryDefeatureDialog* dialog = new GUI::GUIGeometryDefeatureDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateMirror")
        {
            GUI::GUIGeometryMirrorDialog* dialog = new GUI::GUIGeometryMirrorDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateRectangularPattern")
        {
            GUI::GUIGeometryRecPatternDialog* dialog = new GUI::GUIGeometryRecPatternDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateCircularPattern")
        {
            GUI::GUIGeometryCirPatternDialog* dialog = new GUI::GUIGeometryCirPatternDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateScale")
        {
            GUI::GUIGeometryScaleDialog* dialog = new GUI::GUIGeometryScaleDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateRotate")
        {
            GUI::GUIGeometryRotationDialog* dialog = new GUI::GUIGeometryRotationDialog(this, mw);
            dialog->show();
        }
        else if (name == "actionCreateTrans")
        {
            GUI::GUIGeometryTransDialog* dialog = new GUI::GUIGeometryTransDialog(this, mw);
            dialog->show();
        }
        return false;
    }
    bool OperGeometryModel::execProfession()
    {
        int objID = -1;
        this->argValue("objID", objID);
        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return false;
        //更新渲染
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph)
        {
            if (cmdList->getDataCount() == 1)
                operGraph->updateGraph(objID, true, true);
            else
                operGraph->updateGraph(objID, true, false);
        }
        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
        return true;
    }
}

