/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKComponentFactory.h"
#include "FITKAppFramework.h"
#include "FITKComponents.h"
#include "FITKComponentInterface.h"

namespace AppFrame
{
    void FITKComponentFactory::create()
    {
        //错误判断
        auto comps = FITKAPP->getComponents();
        if (comps == nullptr) return;
       //虚函数执行
        QList<FITKComponentInterface*> compList = this->createComponents();
       //压入管理类
        for (FITKComponentInterface* ci : compList)
        {
            ci->init();
            comps->addComponent(ci);
        }
            
    }

    QList<FITKComponentInterface*> FITKComponentFactory::createComponents()
    {
        //抽象类返回错误值
        return QList<FITKComponentInterface*>();
    }

}


