/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUICalculateWidgetBase.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"

#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEISolverManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIDataManager.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"

namespace GUI
{
    GUICalculateWidgetBase::GUICalculateWidgetBase(EventOper::ParaWidgetInterfaceOperator * oper, QWidget * parent) :
        GUIWidgetBase(parent), _oper(oper)
    {

		//风雷的实例化
		_dataManager_PHengLEI= Interface::FITKPHengLEIDataManager::getInstance();//物理场数据管理器 总的
		_solverManager_PHengLEI = _dataManager_PHengLEI->getSolverManager();//求解器
		_data_PHengLEI = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKPHengLEIData>();//数据
		_physicHandleFactory_PHengLEI = FITKAPP->getComponents()->getComponentTByName<Interface::FITKPhysicsHandlerFactory>("FITKPHengLEIPhysicsHandlerFactory");
		//物理场数据搬运工厂 更新设置具体参数
	}

    GUICalculateWidgetBase::~GUICalculateWidgetBase()
    {

    }

    //-----------------------------------------------------------------------------------------------------
    
    GUICalculateSubWidgetBase::GUICalculateSubWidgetBase(QWidget* parent)
    {
        if (parent == nullptr)return;
        _calculateWdiget = dynamic_cast<GUICalculateWidgetBase*>(parent->parent());
    }

    GUICalculateSubWidgetBase::~GUICalculateSubWidgetBase()
    {

    }
}
