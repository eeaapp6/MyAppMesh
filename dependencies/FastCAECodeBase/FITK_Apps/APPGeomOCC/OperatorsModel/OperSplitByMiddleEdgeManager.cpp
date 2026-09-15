/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperSplitByMiddleEdgeManager.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperLine.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

// GUI
#include "GUIDialog/SplitEdgeMidpointDialog.h"

namespace ModelOper
{
    void OperSplitByMiddleEdgeManager::showDialog()
    {
        initDialog<GUI::SplitEdgeMidpointDialog>();
    }

    bool OperSplitByMiddleEdgeManager::execProfession()
    {
        // 刷新模型树。
        updateTree();

        return true;
    }
} // namespace ModelOper
