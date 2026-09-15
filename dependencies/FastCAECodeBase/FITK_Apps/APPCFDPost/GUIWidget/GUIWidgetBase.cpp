/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIWidgetBase.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphProperty.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUI
{
    GUIWidgetBase::GUIWidgetBase(QWidget * parent) :
        Core::FITKWidget(parent)
    {
        _postDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        _postLightManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostLightManager();
        _postColorLibManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostColorLibManager();
        _postGraphManager = Interface::PostGraphObjectManager::getInstance();
    }

    GUIWidgetBase::~GUIWidgetBase()
    {

    }

    void GUIWidgetBase::reRender()
    {
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
        if (graphOper == nullptr)return;
        graphOper->reRender();
    }

    int GUIWidgetBase::getCurrentID()
    {
        return _currentDataID;
    }
}

