/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectPartMesh.h
 * @brief  part网格渲染对象
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-07-18
 *********************************************************************/
#ifndef _GRAPHVTKOBJECTPARTMESH_H__
#define _GRAPHVTKOBJECTPARTMESH_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObjectModelBase.h"

namespace Radioss
{
    class FITKRadiossPart;
}
namespace Interface {
    class FITKMeshFeatureVTK;
}
class vtkUnstructuredGrid;
class vtkDataSetSurfaceFilter;
class FITKGraphActor;
namespace Exchange {

    /**
     * @brief  part网格渲染对象
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-07-18
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectPartMesh : public GraphVTKObjectModelBase
    {
        Q_OBJECT;
        FITKGraphObjectRegist(GraphVTKObjectPartMesh, GraphVTKObjectModelBase);
        FITKCLASS(Exchange, GraphVTKObjectPartMesh);
    public:
        /**
         * @brief  构造函数 - 存在网格Part数据对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        GraphVTKObjectPartMesh(Radioss::FITKRadiossPart* dataObj);


        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-18
         */
        virtual ~GraphVTKObjectPartMesh();
        /**
         * @brief    更新
         * @param[i] forceUpdate 是否强制刷新
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-21
         */
        virtual void update(bool forceUpdate = false) override;
        /**
         * @brief    获取数据可见
         * @return   bool 是否可见
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-21
         */
        virtual bool getVisibility() override;
        /**
         * @brief  更新可见
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-21
         */
        virtual void updateVisibility() override;
        /**
         * @brief    更新子模型颜色
         * @param[i] color 
         * @param[i] otherPar   0:点 1:线
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        virtual void updateSubColor(QColor color, QVariant otherPar) override;
        /**
         * @brief    更新模型颜色
         * @param[i] color                    颜色
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-07-21
         */
        virtual void updateModelColor(QColor color) override;
        /**
         * @brief    设置点大小
         * @param[i] size 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updatePointSize(int size) override;
        /**
         * @brief    设置线宽
         * @param[i] width 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void updateLineWidth(int width) override;
        /**
         * @brief    设置SHP粒子大小
         * @param[i] size
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-01-16
         */
        virtual void updateSHPSize(int size) override;
        /**
         * @brief    模型显示隐藏
         * @param[i] visibility 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-25
         */
        virtual void setVisible(bool visibility) override;
        /**
         * @brief    设置单元显示隐藏
         * @param[i] visibility 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-24
         */
        virtual void setCellVisible(bool visibility) override;
        /**
         * @brief    设置点演员隐藏还是显示
         * @param[i] visibility 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        virtual void setPointVisibility(bool visibility)override;
        /**
         * @brief    设置线演员隐藏还是显示
         * @param[i] visibility 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-22
         */
        virtual void setLineVisibility(bool visibility)override;
        /**
         * @brief    设置拾取模式
         * @param[i] mode                        拾取模式
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        virtual void setPickMode(GraphVTKCommons::ShapePickMode mode) override;
        /**
         * @brief    根据给定的形状类型与VTK单元id获取形状ID
         * @param[i] vtkCellId                      VTK单元ID
         * @param[i] pickType                       拾取类型
         * @return   int                            形状ID
         * @return   otherInfo                      缺省参数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
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

    protected:
        /**
         * @brief    生成渲染数据
         * @param[i] part 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-19
         */
        void generateGraph(Radioss::FITKRadiossPart* part);
        /**
         * @brief  初始化演员
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-21
         */
        void initActor();
        /**
         * @brief  更新数据
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-07-21
         */
        bool updateGraphData(bool forceUpdate);
        /**
         * @brief    更新渲染
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        void updateGraphProperty();

    private:

    private:
        FITKGraphActor* _faceActor{ nullptr };                          ///< 面演员
        FITKGraphActor* _pointActor{ nullptr };                         ///< 点演员
        FITKGraphActor* _surfPointActor{ nullptr };                       ///< 表面单元点演员
    };
}

#endif // !_GRAPHVTKOBJECTPARTMESH_H__
