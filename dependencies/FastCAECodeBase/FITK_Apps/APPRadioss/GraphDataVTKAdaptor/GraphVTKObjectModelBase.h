/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectModelBase.h
 * @brief  模型渲染对象基类
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-09-17
 *********************************************************************/
#ifndef _GRAPHVTKOBJECTMODELBASE_H__
#define _GRAPHVTKOBJECTMODELBASE_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObject3D.h"
#include "GraphVTKCommons.h"

class vtkActor;
class vtkMapper;
class vtkThreshold;
class vtkAlgorithm;

namespace Exchange {
    class GraphVTKObjectHighlighting;

    /**
     * @brief  模型渲染对象基类
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-09-17
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectModelBase : public GraphVTKObject3D
    {
        FITKGraphObjectRegist(GraphVTKObjectModelBase, GraphVTKObject3D);
        FITKCLASS(Exchange, GraphVTKObjectModelBase);
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-17
         */
        GraphVTKObjectModelBase(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-17
         */
        virtual ~GraphVTKObjectModelBase() = default;

        /**
         * @brief    高亮显示
         * @param[i] color 颜色
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        virtual void highlight(QColor color = QColor()) override;

        /**
         * @brief    取消高亮
         * @param[i] details 细节信息
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        virtual void disHighlight() override;
        /**
         * @brief    创建孤立网格阈值过滤器
         * @param[i] type 
         * @param[i] inputConn 
         * @param[i] saved 
         * @return   vtkThreshold *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-19
         */
        vtkThreshold* createOrphanMeshThreshold(int type, vtkAlgorithmOutput* inputConn = nullptr, bool saved = true);
    protected:
        /**
         * @brief  初始化
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-17
         */
        virtual void init();

    protected:
        GraphVTKObjectHighlighting* m_graphObjHighlight{ nullptr }; ///< 高亮对象
        /**
         * @brief  孤立网格阈值过滤器列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-11-19
         */
        QList<vtkThreshold*> m_orphanThresholds;
        /**
         * @brief  特征边过滤器
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-11-19
         */
        vtkAlgorithm* _modelEdgeFilter{ nullptr };
        /**
         * @brief  表面过滤器
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-11-19
         */
        vtkAlgorithm* _modelFilter{ nullptr };
        /**
         * @brief  关联网格可见性
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-11-19
         */
        bool m_nativeMeshVisibility = false;
    };
};

#endif // !_GRAPHVTKOBJECTMODELBASE_H__