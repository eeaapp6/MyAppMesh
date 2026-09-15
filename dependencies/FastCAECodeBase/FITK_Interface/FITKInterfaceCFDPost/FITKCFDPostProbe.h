/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostProbe.h
 * @brief  后处理图像数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostProbe_H
#define _FITKCFDPostProbe_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"
#include <QPair>

class vtkProbeFilter;
class vtkUnstructuredGrid;
class vtkCell;
class vtkPolyData;

namespace Interface
{
    /**
     * @brief  后处理图像数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostProbe :public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPostProbe(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostProbe();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType();
        /**
         * @brief    获取数据
         * @return   vtkDataSet*    数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual vtkDataSet* getOutput();
        /**
         * @brief    获取数据
         * @return   vtkAlgorithmOutput*   算法
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual vtkAlgorithmOutput* getOutputPort();
        /**
         * @brief    更新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        virtual void update() override;
        /**
         * @brief    插入点
         * @param[i] point       点数据
         * @param[i] pointID     点ID（点在父对象中的id，自定义点id = -1）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void appendPoint(double* point, int pointID);
        /**
         * @brief    添加单元
         * @param[i] cellID      单元ID（单元在父对象中的id）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void appendCell(int cellID);
        /**
         * @brief    获取点的数量
         * @return   int       点的数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-22
         */
        int getPointCount();
        /**
         * @brief    获取所有的点
         * @param[i] points     所有的点
         * @return   int        点的id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        int getPointAtInter(double* point,int index);
        /**
         * @brief    获取所有的单元
         * @return   QList<int>     单元在父对象中的id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        QList<int> getCellAll();
        /**
         * @brief    重置清空
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        void reset();
    private:
        /**
         * @brief    添加点至数据源
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void appendPointToSource();
        /**
         * @brief    添加单元至数据源
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void appendCellToSource();
    private:
        /**
         * @brief  探针过滤器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-18
         */
        vtkProbeFilter* _probeFilter = nullptr;
        /**
         * @brief  数据源
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-18
         */
        vtkUnstructuredGrid* _gridData = nullptr;
        QList<QPair<int, double[3]>> _points = {};
        /**
         * @brief  单元（单元在父对象中的id）
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-28
         */
        QList<int> _cells = {};
    };
}

#endif
