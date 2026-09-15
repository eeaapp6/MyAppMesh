/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectGravity.h
 * @brief  重力载荷渲染对象
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-28
 *********************************************************************/
#ifndef _GRAPHVTKOBJECTGRAVITY_H__
#define _GRAPHVTKOBJECTGRAVITY_H__

#include "GraphVTKObjectModelBase.h"
#include "GraphDataVTKAdaptorAPI.h"

class vtkArrowSource;
class vtkGlyph3D;
class vtkPolyData;
class vtkPoints;
class vtkFloatArray;
class FITKGraphActor;
namespace Radioss
{
    class FITKGravity;
}
namespace Interface
{
    class FITKModelSet;
}
namespace Exchange
{
    /**
     * @brief  重力载荷渲染对象
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-28
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectGravity : public GraphVTKObjectModelBase
    {
        // Regist
        FITKGraphObjectRegist(GraphVTKObjectGravity, GraphVTKObjectModelBase);
        FITKCLASS(Exchange, GraphVTKObjectGravity);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        GraphVTKObjectGravity(Radioss::FITKGravity* dataObj);
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        ~GraphVTKObjectGravity() override;

        /**
         * @brief    更新
         * @param[i] forceUpdate 是否强制刷新
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        virtual void update(bool forceUpdate = false) override;

        /**
         * @brief    获取数据可见
         * @return   bool 是否可见
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        virtual bool getVisibility() override;

        /**
         * @brief  更新可见
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-27
         */
        virtual void updateVisibility() override;
        /**
         * @brief    设置可见
         * @param[i] isVisible 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-29
         */
        virtual void setVisible(bool isVisible) override;

    private:
        /**
         * @brief    初始化
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        void init();
        /**
         * @brief    初始化演员
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-28
         */
        void initActor();
        /**
         * @brief    生成渲染数据
         * @return   bool               是否成功
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        bool generateDataSet();

        /**
         * @brief    生成渲染对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        void generateGraph();
        /**
         * @brief    Insert a new point and its value to points and data.
         * @param[i] pos             节点位置
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-28
         */
        void insertNextPoint(double* pos);

        /**
         * @brief    创建重力方向向量
         * @return   方向向量 [x, y, z]
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-28
         */
        void getGravityDirectionVector(double direction[3]);

        /**
         * @brief    计算箭头的合适缩放因子
         * @return   缩放因子
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-28
         */
        double calculateArrowScale();
        /**
         * @brief    更新渲染属性
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-01
         */
        void updateRenderingProperties();
        /**
         * @brief    检查包围盒中心是否更改
         * @return   bool
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-01
         */
        bool checkAndUpdateGlobalCenter();
    private:
        /**
         * @brief  箭头演员
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        FITKGraphActor* _arrowActor = nullptr;

        /**
         * @brief  VTK数据对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        vtkPolyData* _arrowData = nullptr;
        vtkPoints* _points = nullptr;
        vtkFloatArray* _vectors = nullptr;
        vtkArrowSource* _arrowSource = nullptr;
        vtkGlyph3D* _glyph3D = nullptr;

        /**
         * @brief  载荷点列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        QList<double*> _loadPoints;
        /**
         * @brief  上一个载荷对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-01
         */
        Interface::FITKModelSet* _prevLoadSet = nullptr;
    };
}

#endif // !_GRAPHVTKOBJECTGRAVITY_H__