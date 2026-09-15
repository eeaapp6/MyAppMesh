/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        OperNBodyUniteManager.h
 * @brief       多实体布尔求和操作器。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-08
 *********************************************************************/

#ifndef __OPERNBODYUNITEMANAGER_H__
#define __OPERNBODYUNITEMANAGER_H__

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    // 定义操作器类并注册。
    DefineGeometryOperator(actionNBodyUnite, OperNBodyUniteManager);
} // namespace ModelOper

#endif // !__OPERNBODYUNITEMANAGER_H__
