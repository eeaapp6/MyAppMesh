/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ApplicationCAD.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

ApplicationCAD::ApplicationCAD(int &argc, char **argv) : 
    AppFrame::FITKApplication(argc, argv)
{
    
}

bool ApplicationCAD::init()
{
    // 执行父类初始化。
    bool flag = AppFrame::FITKApplication::init();

    if (flag)
    {
        // 执行程序初始化。
        //@{      
        EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");

        // 显示初始基准元素。
        if (graphOper)
        {
            graphOper->setDatumsVisibility(true);
        }

        // 初始化三维。
        if (graphOper)
        {
            graphOper->initialize3D();
        }
        //@}
    }

    return flag;
}
