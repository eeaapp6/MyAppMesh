/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperDeleteFloatingEdgeManager.h
 * @brief   删除悬浮边操作器。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-26
 *********************************************************************/

#ifndef OPERDELETEFLOATINGEDGEMANAGER_H
#define OPERDELETEFLOATINGEDGEMANAGER_H

#include "OperCreateGeoManagerBase.h"
#include "OperatorsModelAPI.h"

namespace ModelOper
{
    // 注册操作器。
    DefineGeometryOperator(actionDeleteFloatingEdge, OperDeleteFloatingEdgeManager);

    //Register2FITKOperatorRepo(OperDeleteFloatingEdgeManager, OperDeleteFloatingEdgeManager);
    //Register2FITKOperatorRepo(actionDeleteFloatingEdgeCreate, OperDeleteFloatingEdgeManager);
    //Register2FITKOperatorRepo(actionDeleteFloatingEdgeEdit, OperDeleteFloatingEdgeManager);
    //Register2FITKOperatorRepo(actionDeleteFloatingEdgeDelete, OperDeleteFloatingEdgeManager);
    //Register2FITKOperatorRepo(actionDeleteFloatingEdgeHide, OperDeleteFloatingEdgeManager);
    //Register2FITKOperatorRepo(actionDeleteFloatingEdgeShow, OperDeleteFloatingEdgeManager);
} // namespace ModelOper

#endif // OperDeleteFloatingEdgeManager_H
