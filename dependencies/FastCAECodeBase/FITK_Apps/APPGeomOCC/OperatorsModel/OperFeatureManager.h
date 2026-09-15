/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperFeatureManager.h
 * @brief  特征操作器.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-25
 */
#ifndef OPERATORSFEATUREMANAGER_H
#define OPERATORSFEATUREMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /// 肋特征
    DefineGeometryOperator(actionStiffener, OperFeatureStiffenerManager);


} // namespace ModelOper
#endif // !OPERATORSFEATUREMANAGER_H
