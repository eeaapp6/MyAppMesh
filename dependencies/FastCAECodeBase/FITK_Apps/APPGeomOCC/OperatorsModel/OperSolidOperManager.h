/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    OperSolidOperManager.h
 * @brief   体特征编辑操作器。（倒角/倒圆/移除倒角/移除倒圆/填补孔洞/去除凸包）

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-08-29
 *********************************************************************/

#ifndef OPERSOLIDOPERMANAGER_H
#define OPERSOLIDOPERMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    // 定义操作器类并注册。
    DefineGeometryOperator(actionChamferSolid, OperChamferManager);

    // 定义操作器类并注册。
    DefineGeometryOperator(actionFilletSolid, OperFilletManager);

    // 定义操作器类并注册。
    DefineGeometryOperator(actionDefeature, OperDefeatureManager);

} // namespace ModelOper

#endif // OPERSOLIDOPERMANAGER_H
