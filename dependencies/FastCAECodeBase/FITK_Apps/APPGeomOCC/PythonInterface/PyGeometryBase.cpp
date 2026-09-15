/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PyGeometryBase.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "GUIFrame/TreeWidget.h"
#include "GUIFrame/MainWindow.h"

PyGeometryBase::PyGeometryBase()
{
    _cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
}

PyGeometryBase::PyGeometryBase(const QString& name) : PyGeometryBase()
{
    if (name.isEmpty()||!_cmdList) 
    {
        _command = nullptr;
        return;
    }
    _command = _cmdList->getDataByName(name);

}

QString PyGeometryBase::getName() const
{
    if (!_command) {
        return QString();
    }
    return _command->getDataObjectName();
}


bool PyGeometryBase::setName(const QString& name)
{
    if (!_command) {
        return false;
    }
    Interface::FITKAbsGeoCommand* command = _cmdList->getDataByName(name);
    if (command && _command->getDataObjectName() != name)
    {
         return false;
    }
    _command->setDataObjectName(name);
    
    updateGraphAndTreeEvent();

    return true;
}

void PyGeometryBase::updateGraphAndTreeEvent()
{
    // 刷新树。
    GUI::MainWindow * mainWindow = dynamic_cast<GUI::MainWindow *>(FITKGLODATA->getMainWindow());
    if (mainWindow)
    {
        mainWindow->updateGeometryTree();
    }

    // 刷新可视化窗口。
    auto operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph)
    {
        operGraph->updateGraphs();
    }
}