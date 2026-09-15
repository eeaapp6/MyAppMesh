/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
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
     * @return   QList<T>                   结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-11-21
     */
    template<typename T>
    QList<T> intersection(const QList<QList<T>>& lists)
    {
        if (lists.isEmpty()) return QList<T>();

        // 使用第一个列表作为基准
        QList<T> result = lists.first();

        for (int i = 1; i < lists.size(); ++i) {
            QList<T> temp;
            const QList<T>& currentList = lists[i];

            for (const T& value : result) {
                if (currentList.contains(value)) {
                    temp.append(value);
                }
            }

            result = temp;
            if (result.isEmpty()) break;
        }

        return result;
    }
}

#endif
