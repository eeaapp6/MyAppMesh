/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectModel.h
 * @brief  几何模型渲染对象
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-18
 *********************************************************************/
#ifndef __GraphVTKObjectModel_H__
#define __GraphVTKObjectModel_H__

#include "GraphVTKObjectModelBase.h"

#include "GraphDataVTKAdaptorAPI.h"

#include <QVector>

 // Forward declaration
class vtkPolyData;
class vtkUnstructuredGrid;
class vtkDataSet;
class vtkDoubleArray;
class QVariant;

namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKGeometryMeshVS;
    class FITKVirtualTopoManager;
}

namespace Exchange
{
    class FITKFluidVTKGraphObjectSelect;
}

namespace Exchange
{
    /**
     * @brief  几何模型渲染对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-18
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectModel : public GraphVTKObjectModelBase
    {
        // Regist
        FITKGraphObjectRegist(GraphVTKObjectModel, GraphVTKObjectModelBase);
        FITKCLASS(Exchange, GraphVTKObjectModel);
    public:
        /**
         * @brief    构造函数
         * @param[i] geoCommand                  几何命令对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        GraphVTKObjectModel(Interface::FITKAbsGeoCommand* geoCommand);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        ~GraphVTKObjectModel();
        /**
         * @brief    设置拾取模式
         * @param[i] mode                        拾取模式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickMode(GraphVTKCommons::ShapePickMode mode) override;
        /**
         * @brief    根据给定的形状类型与VTK单元id获取形状ID
         * @param[i] vtkCellId                      VTK单元ID
         * @param[i] pickType                       拾取类型
         * @return   int                            形状ID
         * @return   otherInfo                      缺省参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual int getShapeIdByVTKCellId(int vtkCellId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo = QVariant()) override;
        /**
         * @brief    更具给定的形状类型与形状id，获取VTK单元id
         * @param[i] shapeId                        形状id
         * @param[i] pickType                       拾取类型
         * @return   const QVector<int>             单元id
         * @return   otherInfo                      缺省参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual const QVector<int> getVTKCellIdsByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType, QVariant otherInfo = QVariant()) override;
        /**
         * @brief    通过几何形状id获取虚拓扑索引
         * @param[i] shapeId                                 几何形状id（几何虚拓扑id）
         * @param[i] topAbsShapeType                         几何形状类型
         * @return   int                                     虚拓扑索引
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        int getVirTopoIndexByShapeId(int shapeId, GraphVTKCommons::ShapePickMode pickType) override;
        /**
         * @brief    根据形状网格类型获取网格数据
         * @param[i] type                           形状网格类型
         * @return   vtkDataSet*                    拾取类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual vtkDataSet* getMesh(GraphVTKCommons::ShapePickMode pickType) override;
        /**
         * @brief    更新[重写]
         * @param[i] forceUpdate           是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-18
         */
        void update(bool forceUpdate = false) override;
        /**
         * @brief    设置对象显示或隐藏[重写]
         * @param[i] visibility            显示或隐藏
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-01-06
         */
        void setVisible(bool visibility) override;
        /**
         * @brief    设置面显示隐藏
         * @param[i] visibility 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-24
         */
        void setCellVisible(bool visibility) override;
        /**
         * @brief    设置点显示隐藏
         * @param[i] visibility 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setPointVisibility(bool visibility) override;
        /**
         * @brief    设置线显示隐藏
         * @param[i] visibility 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void setLineVisibility(bool visibility) override;
        /**
         * @brief    获取显示隐藏状态
         * @return   bool
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        bool getVisibility() override;
        /**
         * @brief    更新可见性
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        void updateVisibility() override;
        /**
         * @brief    更新子模型颜色
         * @param[i] color
         * @param[i] otherPar    0: 点颜色; 1: 线颜色;
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        virtual void updateSubColor(QColor color, QVariant otherPar = QVariant()) override;
        /**
         * @brief    更新模型颜色
         * @param[i] color 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-30
         */
        virtual void updateModelColor(QColor color = QColor()) override;
        /**
         * @brief    设置点大小
         * @param[i] size 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        virtual void updatePointSize(int size) override;
        /**
         * @brief    设置线宽
         * @param[i] width
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        virtual void updateLineWidth(int width) override;
    private:
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
        /**
         * @brief    更新渲染属性
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        void updateGraphProperty();
    private:
        /**
         * @brief  点演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        FITKGraphActor* _pointActor{ nullptr };
        /**
         * @brief  线演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        FITKGraphActor* _lineActor{ nullptr };
        /**
         * @brief  面演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-01-14
         */
        FITKGraphActor* _faceActor{ nullptr };
        /**
         * @brief  体演员
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-15
         */
        FITKGraphActor* _solidActor{ nullptr };
    };
}   // namespace Exchange

#endif // __GraphVTKObjectModel_H__
