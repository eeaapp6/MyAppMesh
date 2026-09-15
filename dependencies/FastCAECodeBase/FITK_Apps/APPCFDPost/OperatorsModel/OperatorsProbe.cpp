/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorsProbe.h"

#include "GUIDialog/ProbeDialog.h"
#include "GUIDialog/GUIDialogManager.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/MainTreeWidget.h"
#include "GUIWidget/TreeWidget.h"
#include "GUIWidget/GUIPickInfo.h"
#include "GUIWidget/PickedData.h"
#include "GUIWidget/PickedDataProvider.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostIsosurface.h"

#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>

namespace OperModel
{
    bool OperatorsProbe::execGUI()
    {
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr)return false;
        GUI::TreeWidget* treeWidget = mainWin->getTreeWidget()->getTreeWidget();
        if (treeWidget == nullptr)return false;
        //获取树节点当前数据对象id
        if (_senderName == "actionProbeCreate") {
            GUI::ProbeDialog* dialog = new GUI::ProbeDialog(treeWidget->getCurrentDataID(), this, true);
            dialog->show();
        }
        else if (_senderName == "actionProbeEdit") {
            int objID = -1;
            argValue(objID_String, objID);
            if (objID <= 0)return false;

            GUI::ProbeDialog* dialog = new GUI::ProbeDialog(objID, this, false);
            dialog->show();
        }
        return false;
    }

    bool OperatorsProbe::execProfession()
    {
        Interface::FITKCFDPost3DManager* manager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (manager == nullptr)return false;
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return false;
        EventOper::ParaWidgetInterfaceOperator* treeOper = FITKOPERREPO->getOperatorT<EventOper::ParaWidgetInterfaceOperator>("ModelTreeEvent");
        if (treeOper == nullptr)return false;

        int objID = -1;
        argValue(objID_String, objID);
        Interface::FITKAbstractCFDPostData* data = manager->getDataByID(objID);
        if (data) {
            graphOper->updateSingle(objID);
        }

        treeOper->updateTree();
        return false;
    }

    void OperatorsProbe::eventProcess(int index, QHash<int, QVariant> value)
    {
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->reRender();

        //点选择事件
        if (index == 0) {
            //拾取对象获取事件绑定
            GraphData::PickedDataProvider* pickD = GraphData::PickedDataProvider::getInstance();
            if (pickD == nullptr) return;
            //清空旧数据
            GUI::GUIPickInfoStru oldPinfo;
            oldPinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJNone;
            oldPinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
            GUI::GUIPickInfo::SetPickInfo(oldPinfo);
            pickD->clearAll();

            //拾取信息设置
            GUI::GUIPickInfoStru newPinfo;
            newPinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJVert;
            newPinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
            //保存参数
            GUI::GUIPickInfo::SetPickInfo(newPinfo);
            connect(pickD, SIGNAL(sigPickDataOver()), this, SLOT(slotPickPointDataOver()));
        }
        //单元选取事件
        else if (index == 1) {
            //拾取对象获取事件绑定
            GraphData::PickedDataProvider* pickD = GraphData::PickedDataProvider::getInstance();
            if (pickD == nullptr) return;
            //清空旧数据
            GUI::GUIPickInfoStru oldPinfo;
            oldPinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJNone;
            oldPinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
            GUI::GUIPickInfo::SetPickInfo(oldPinfo);
            pickD->clearAll();

            //拾取信息设置
            GUI::GUIPickInfoStru newPinfo;
            newPinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJEdge;
            newPinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMSingle;
            //保存参数
            GUI::GUIPickInfo::SetPickInfo(newPinfo);
            connect(pickD, SIGNAL(sigPickDataOver()), this, SLOT(slotPickCellDataOver()));
        }
        //点选取取消事件
        else if (index == 2) {
            //拾取对象获取事件绑定
            GraphData::PickedDataProvider* pickD = GraphData::PickedDataProvider::getInstance();
            if (pickD == nullptr) return;
            //清空旧数据
            GUI::GUIPickInfoStru oldPinfo;
            oldPinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJNone;
            oldPinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
            GUI::GUIPickInfo::SetPickInfo(oldPinfo);
            pickD->clearAll();
            disconnect(pickD, SIGNAL(sigPickDataOver()), this, SLOT(slotPickPointDataOver()));
            graphOper->reRender();
        }
        //单元选取取消事件
        else if (index == 3) {
            //拾取对象获取事件绑定
            GraphData::PickedDataProvider* pickD = GraphData::PickedDataProvider::getInstance();
            if (pickD == nullptr) return;
            //清空旧数据
            GUI::GUIPickInfoStru oldPinfo;
            oldPinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJNone;
            oldPinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
            GUI::GUIPickInfo::SetPickInfo(oldPinfo);
            pickD->clearAll();
            disconnect(pickD, SIGNAL(sigPickDataOver()), this, SLOT(slotPickCellDataOver()));
            graphOper->reRender();
        }
    }

    void OperatorsProbe::slotPickPointDataOver()
    {
        //拾取对象获取事件绑定
        GraphData::PickedDataProvider* pickD = GraphData::PickedDataProvider::getInstance();
        if (pickD == nullptr) return;
        disconnect(pickD, SIGNAL(sigPickDataOver()), this, SLOT(slotPickPointDataOver()));

        //拾取信息设置
        GUI::GUIPickInfoStru pinfo;
        pinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJNone;
        pinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
        //保存参数
        GUI::GUIPickInfo::SetPickInfo(pinfo);

        GraphData::PickedData* pickData = pickD->getPickDatas();;
        if (pickData == nullptr)return;
        vtkUnstructuredGrid* grid = pickData->getGrid();
        if (grid == nullptr)return;

        //获取极值界面
        GUI::GUIDialogManager* dialogManager = GUI::GUIDialogManager::getInstance();
        QList<GUI::ProbeDialog*> probeDialogs = dialogManager->getDialog<GUI::ProbeDialog>();
        for (GUI::ProbeDialog* pD : probeDialogs) {
            if (pD == nullptr)continue;
            for(int i = 0;i<pickData->getPointIDs().size();i++){
                if (grid->GetPoints() == nullptr)return;
                double* point = grid->GetPoints()->GetPoint(i);
                pD->insertPoint(point, pickData->getPointIDs()[i]);
            }
        }

        pickD->clearAll();
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->reRender();
    }

    void OperatorsProbe::slotPickCellDataOver()
    {
        //拾取对象获取事件绑定
        GraphData::PickedDataProvider* pickD = GraphData::PickedDataProvider::getInstance();
        if (pickD == nullptr) return;
        disconnect(pickD, SIGNAL(sigPickDataOver()), this, SLOT(slotPickCellDataOver()));

        //拾取信息设置
        GUI::GUIPickInfoStru pinfo;
        pinfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJNone;
        pinfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMNone;
        //保存参数
        GUI::GUIPickInfo::SetPickInfo(pinfo);

        GraphData::PickedData* pickData = pickD->getPickDatas();
        if (pickData == nullptr)return;
        //获取极值界面
        GUI::GUIDialogManager* dialogManager = GUI::GUIDialogManager::getInstance();
        QList<GUI::ProbeDialog*> probeDialogs = dialogManager->getDialog<GUI::ProbeDialog>();
        for (GUI::ProbeDialog* pD : probeDialogs) {
            if (pD == nullptr)continue;
            QList<int> cellIds = pickData->getCellIDs();
            for (int cellID : cellIds) {
                pD->insertCell(cellID);
            }
        }
        pickD->clearAll();
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->reRender();
    }
}