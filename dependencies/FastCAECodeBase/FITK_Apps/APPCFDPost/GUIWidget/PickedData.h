/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PickedData.h
 * @brief  拾取数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-21
 *********************************************************************/
#ifndef __PICKEDDATA_H__
#define __PICKEDDATA_H__

#include <QObject>
#include "GUIWidgetAPI.h"

class vtkPolyData;
class vtkUnstructuredGrid;

namespace Interface {
    class PostGraphObjectPick;
}

namespace GraphData
{
    /**
     * @brief  拾取数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-21
     */
    class GUIWidgetAPI PickedData : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-23
         */
        PickedData();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        ~PickedData();
        /**
         * @brief    插入点ID
         * @param[i] pointID     点ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void insertPointID(int pointID);
        /**
         * @brief    获取所有的点id
         * @return   QList<int>       所有的点id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        QList<int> getPointIDs();
        /**
         * @brief    插入单元id
         * @param[i] cellID           单元id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void insertCellID(int cellID);
        /**
         * @brief    获取所有的单元id
         * @return   QList<int>       单元id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        QList<int> getCellIDs();
        /**
         * @brief    获取VTK数据
         * @return   vtkUnstructuredGrid*   VTK数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        vtkUnstructuredGrid* getGrid();
        /**
         * @brief    获取渲染数据对象
         * @return   Interface::PostGraphObjectPick*   渲染数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        Interface::PostGraphObjectPick* getGraphPickObject();
        /**
         * @brief    清空
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void clear();
    private:
        /**
         * @brief  VTK数据（用于传递VTK数据至可视化）
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-23
         */
        vtkUnstructuredGrid* _resultGrid = nullptr;
        /**
         * @brief  可视化数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-23
         */
        Interface::PostGraphObjectPick* _graphPick = nullptr;
        /**
         * @brief  点ID（原始数据中的ID）
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-23
         */
        QList<int> _pointsID = {};
        /**
         * @brief  单元ID（原始数据中的ID）
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-23
         */
        QList<int> _cellsID = {};
    };
}

#endif // __PICKEDDATA_H__
