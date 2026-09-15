/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectSet.h
 * @brief  集合渲染数据
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-21
 *********************************************************************/
#ifndef __GraphVTKObjectSet_H__
#define __GraphVTKObjectSet_H__

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
    class FITKModelSet;
}

namespace Exchange
{
    /**
     * @brief  集合渲染数据
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-21
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectSet : public GraphVTKObject3D
    {
        // Regist
        FITKGraphObjectRegist(GraphVTKObjectSet, GraphVTKObject3D);
        FITKCLASS(Exchange, GraphVTKObjectSet);
    public:
        /**
         * @brief    构造函数
         * @param[i] setData                   集合数据对象
         * @param[i] modeId                    集合所属对象id
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        GraphVTKObjectSet(Interface::FITKModelSet* setData, int modeId);
        /**
         * @brief    析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        virtual ~GraphVTKObjectSet();
        /**
         * @brief    高亮
         * @param[i] color           颜色
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        void highlight(QColor color = QColor()) override;
        /**
         * @brief    取消高亮
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        void disHighlight() override;
        /**
         * @brief    获取高亮图层
         * @return   int            渲染图层
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        virtual int getHightlightRenderLayer() override;
    private:
        /**
         * @brief    初始化
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        void init();
        /**
         * @brief    生成渲染数据
         * @return   bool               是否成功
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        bool generateDataSet();
        /**
         * @brief    生成渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        void generateGraph();
    private:
        /**
         * @brief  渲染数据
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        vtkUnstructuredGrid* _grid = nullptr;
        /**
         * @brief  演员
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        FITKGraphActor* _actor{ nullptr };
        /**
         * @brief  集合所属模型对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-21
         */
        int _modeId = -1;
    };
}   // namespace Exchange

#endif // __GraphVTKObjectSet_H__
