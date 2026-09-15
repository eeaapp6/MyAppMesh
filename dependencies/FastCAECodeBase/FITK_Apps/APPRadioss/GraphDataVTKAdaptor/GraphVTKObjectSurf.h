/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectSurf.h
 * @brief  表面渲染数据
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef __GraphVTKObjectSurf_H__
#define __GraphVTKObjectSurf_H__

#include "GraphVTKObject3D.h"

#include "GraphDataVTKAdaptorAPI.h"

#include <QVector>

 // Forward declaration
class vtkPolyData;
class vtkUnstructuredGrid;
class vtkDataSet;
class vtkDoubleArray;
class QVariant;

class FITKGraphActor;

namespace Interface
{
    class FITKMeshSurface;
}

namespace Exchange
{
    /**
     * @brief  表面渲染数据
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-18
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectSurf : public GraphVTKObject3D
    {
        // Regist
        FITKGraphObjectRegist(GraphVTKObjectSurf, GraphVTKObject3D);
        FITKCLASS(Exchange, GraphVTKObjectSurf);
    public:
        /**
         * @brief    构造函数
         * @param[i] surfData                  表面数据对象
         * @param[i] modeId                    表面所属对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        GraphVTKObjectSurf(Interface::FITKMeshSurface* surfData, int modeId);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        virtual ~GraphVTKObjectSurf();
        /**
         * @brief    高亮
         * @param[i] color           颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void highlight(QColor color = QColor()) override;
        /**
         * @brief    取消高亮
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void disHighlight() override;
        /**
         * @brief    获取高亮图层
         * @return   int            渲染图层
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-21
         */
        virtual int getHightlightRenderLayer() override;
    private:
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-28
         */
        void init();
        /**
         * @brief    生成渲染数据
         * @return   bool               是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-06
         */
        bool generateDataSet();
        /**
         * @brief    生成渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-06
         */
        void generateGraph();
    private:
        /**
         * @brief  渲染数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-28
         */
        vtkUnstructuredGrid* _grid = nullptr;
        /**
         * @brief  演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        FITKGraphActor* _actor{ nullptr };
        /**
         * @brief  集合所属模型对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-28
         */
        int _modeId = -1;
    };
}   // namespace Exchange

#endif
