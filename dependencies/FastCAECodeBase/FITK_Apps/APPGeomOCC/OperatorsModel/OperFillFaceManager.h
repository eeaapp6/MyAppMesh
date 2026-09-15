/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperFillFaceManager.h
 * @brief   填补面操作器。

 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-08-30
 *********************************************************************/

#ifndef OperFillFaceManager_H
#define OperFillFaceManager_H

#include "OperCreateGeoManagerBase.h"
#include "OperatorsModelAPI.h"

namespace ModelOper
{
    DefineGeometryOperator(actionFillGapsFace, OperFillFaceManager);


    // 注册操作器。
    //Register2FITKOperatorRepo(OperFillFaceManager, OperFillFaceManager);
    //Register2FITKOperatorRepo(actionFillGapsFaceCreate, OperFillFaceManager);
    //Register2FITKOperatorRepo(actionFillGapsFaceEdit, OperFillFaceManager);
    //Register2FITKOperatorRepo(actionFillGapsFaceDelete, OperFillFaceManager);
    //Register2FITKOperatorRepo(actionFillGapsFaceHide, OperFillFaceManager);
    //Register2FITKOperatorRepo(actionFillGapsFaceShow, OperFillFaceManager);
} // namespace ModelOper

#endif // OperFillFaceManager_H
