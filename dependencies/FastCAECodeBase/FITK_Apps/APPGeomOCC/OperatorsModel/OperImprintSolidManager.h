/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperImprintSolidManager.h
 * @brief   体压印操作器。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-09-05
 *********************************************************************/

#ifndef OPERIMPRINTSOLIDMANAGER_H
#define OPERIMPRINTSOLIDMANAGER_H

#include "OperCreateGeoManagerBase.h"
#include "OperatorsModelAPI.h"

namespace ModelOper
{

    DefineGeometryOperator(actionImprintSolid, OperImprintSolidManager);


    // 注册操作器。
    //Register2FITKOperatorRepo(OperImprintSolidManager, OperImprintSolidManager);
    //Register2FITKOperatorRepo(actionImprintSolidCreate, OperImprintSolidManager);
    //Register2FITKOperatorRepo(actionImprintSolidEdit, OperImprintSolidManager);
    //Register2FITKOperatorRepo(actionImprintSolidDelete, OperImprintSolidManager);
    //Register2FITKOperatorRepo(actionImprintSolidHide, OperImprintSolidManager);
    //Register2FITKOperatorRepo(actionImprintSolidShow, OperImprintSolidManager);
} // namespace ModelOper

#endif // OperImprintSolidManager_H
