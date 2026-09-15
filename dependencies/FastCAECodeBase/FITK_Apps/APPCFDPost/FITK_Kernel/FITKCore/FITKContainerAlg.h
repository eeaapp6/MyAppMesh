/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKContainerAlg.h
 * @brief  容器算法
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-12-09
 *********************************************************************/
#ifndef _FITKContainerAlg_H_
#define _FITKContainerAlg_H_

#include "FITKCoreAPI.h"

#include <QList>

namespace Core
{
    /**
     * @brief    计算交集
     * @param[i] lists                      原始数据
     * @return   QList<int>                 结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-11-21
     */
    QList<int> FITKCoreAPI intersection(const QList<QList<int>>& lists);
}

#endif
