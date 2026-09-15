/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PickedDataCalculator.h
 * @brief  拾取数据计算生成类
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-05
 *********************************************************************/

#ifndef __PICKEDDATACALCULATOR_H__
#define __PICKEDDATACALCULATOR_H__

#include <QObject>

#include "PickDataProviderAPI.h"

 // GUI
#include "GUIPickInfo.h"

// 前置声明
class vtkCell;
class vtkIdList;
class vtkIntArray;
class vtkUnstructuredGrid;

namespace GraphData
{
    class PickedData;
}

namespace GraphData
{
    /**
     * @brief  拾取数据计算类
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-05
     */
    class PickDataProviderAPI PickedDataCalculator : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] pickedInfo                 当前拾取到的对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        PickedDataCalculator(GraphData::PickedData* pickedInfo);
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        ~PickedDataCalculator() = default;
        /**
         * @brief    根据拾取类型进行拾取计算
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        void calculate();
    private:
        /**
         * @brief    单独拾取数据生成
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        void Individually();
        /**
         * @brief    单独拾取网格单元面数据处理
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        void IndividuallyByMeshCellSurf();
        /**
         * @brief    角度拾取
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-06
         */
        void ByAngle();
        /**
         * @brief  角度拾取网格节点
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-06
         */
        void ByAngleMeshPoint();
        /**
         * @brief  角度拾取网格表面节点
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-06
         */
        void ByAngleMeshSurfPoint();
        /**
         * @brief  角度拾取网格单元
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-06
         */
        void ByAngleMeshCell();
        /**
         * @brief  角度拾取网格表面单元
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-06
         */
        void ByAngleMeshCellSurf();
        /**
         * @brief    框选数据生成
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        void byAreaPick();
        /**
         * @brief    框选拾取网格节点数据处理
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-20
         */
        void byAreaPickByMeshPoint();
        /**
         * @brief  框选拾取网格单元面数据处理
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        void byAreaPickByMeshCellSurf();
        /**
         * @brief    获取框选拾取到的表面节点数据集
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-20
         */
        void byAreaPickedSurfNodeDataSet();
        /**
         * @brief    获取框选拾取到的单元数据集
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-20
         */
        void byAreaPickedCellDataSet();
    private:
        /**
         * @brief    需要计算的拾取数据。
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        GraphData::PickedData* m_pickedData = nullptr;
    };
}

#endif
