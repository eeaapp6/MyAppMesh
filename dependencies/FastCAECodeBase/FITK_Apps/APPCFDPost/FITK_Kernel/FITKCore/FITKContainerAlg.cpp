/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKContainerAlg.h"

namespace Core
{
    QList<int> FITKCoreAPI intersection(const QList<QList<int>>& lists)
    {
        if (lists.isEmpty()) return QList<int>();

        // 使用第一个列表作为基准
        QList<int> result = lists.first();

        for (int i = 1; i < lists.size(); ++i) {
            QList<int> temp;
            const QList<int>& currentList = lists[i];

            for (const int& value : result) {
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